#include "fourier_transform.hpp"

#include <cmath>
#include <cassert>

#include "tbb/parallel_for.h"

// getenv usage
#include <cstdlib>
#include <iostream>

typedef tbb::blocked_range<unsigned> tb_range_t;

namespace hpce
{
namespace yc12015{

class direct_fourier_transform_parfor_inner
  : public fourier_transform
{
protected:
  /*! We can do any size transform */
  virtual size_t calc_padded_size(size_t n) const
  {
    return n;
  }

  virtual unsigned get_chunk_size() const {
    // chunk size for inner loop
    const char *v = getenv("HPCE_DIRECT_INNER_K");
    // "wall clock time vs grainsize" 
    // @ https://www.threadingbuildingblocks.org/docs/help/tbb_userguide/Controlling_Chunking.html
    unsigned k = 100;
    if(v != NULL){
      k = atoi(v);
    }
    //std::cerr<<"Chunk size: "<<k<<std::endl;
    return k;
  }

  virtual void forwards_impl(
    size_t n,  const complex_t &wn,
    const complex_t *pIn,
    complex_t *pOut
  ) const
  {
    assert(n>0);

    const real_t PI2=2*3.1415926535897932384626433832795;

    // get tbb chunk size from env
    const unsigned K = get_chunk_size();

    // = -i*2*pi / n
    complex_t neg_im_2pi_n=-complex_t(0, 1)*PI2 / (real_t)n;

    // Fill the output array with zero
    std::fill(pOut, pOut+n, 0);

    for(size_t ii=0;ii<n;ii++){
      tbb::parallel_for( tb_range_t(0, n, K), [&](const tb_range_t &chunk){
        for(size_t kk=chunk.begin(); kk!=chunk.end(); kk++){
          pOut[kk] += pIn[ii] * exp( neg_im_2pi_n * (double)kk * (double)ii );
        }
      },
      tbb::simple_partitioner()
      );
    }
  }

  virtual void backwards_impl(
    size_t n,  const complex_t &/*wn*/,
    const complex_t *pIn,
    complex_t *pOut
  ) const
  {
    assert(n>0);

    const real_t PI2=2*3.1415926535897932384626433832795;

    const unsigned K = get_chunk_size();

    // = i*2*pi / n
    complex_t im_2pi_n=complex_t(0, 1)*PI2 / (real_t)n;
    const real_t scale=real_t(1)/n;

    // Fill the output array with zero
    std::fill(pOut, pOut+n, 0);

    for(size_t ii=0;ii<n;ii++){
      tbb::parallel_for( tb_range_t(0, n, K), [&](const tb_range_t &chunk){
        for(size_t kk=chunk.begin(); kk!=chunk.end(); kk++){
          pOut[kk] += pIn[ii] * exp( im_2pi_n * (double)kk * (double)ii ) * scale;
        }
      },
      tbb::simple_partitioner()
      );
    }
  }

public:
  virtual std::string name() const
  { return "hpce.yc12015.direct_fourier_transform_parfor_inner"; }

  virtual bool is_quadratic() const
  { return true; }
};

std::shared_ptr<fourier_transform> Create_direct_fourier_transform_parfor_inner()
{
  return std::make_shared<direct_fourier_transform_parfor_inner>();
}

}; // namepsace yc12015
}; // namespace hpce
