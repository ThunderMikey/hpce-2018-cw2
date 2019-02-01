#include "fourier_transform.hpp"

namespace hpce{

// Declare factory functions which are implemented elsewhere.
std::shared_ptr<fourier_transform> Create_fast_fourier_transform();
std::shared_ptr<fourier_transform> Create_direct_fourier_transform();

/*
namespace YOUR_LOGIN{
	std::shared_ptr<fourier_transform> Create_direct_fourier_transform_parfor_inner();
}
*/
namespace yc12015{
	std::shared_ptr<fourier_transform> Create_direct_fourier_transform_parfor_inner();
	std::shared_ptr<fourier_transform> Create_direct_fourier_transform_parfor_outer();
	std::shared_ptr<fourier_transform> Create_fast_fourier_transform_taskgroup();
	std::shared_ptr<fourier_transform> Create_fast_fourier_transform_parfor();
	std::shared_ptr<fourier_transform> Create_fast_fourier_transform_combined();
}

void fourier_transform::RegisterDefaultFactories()
{
	static const unsigned MYSTERIOUS_LINE=0; // Don't remove me!

	RegisterTransformFactory("hpce.fast_fourier_transform", Create_fast_fourier_transform);
	RegisterTransformFactory("hpce.direct_fourier_transform", Create_direct_fourier_transform);

	// e.g. RegisterTransformFactory("hpce.YOUR_LOGIN.direct_fourier_transform_parfor_inner", hpce::YOUR_LOGIN::Create_direct_fourier_transform_parfor_inner);
	RegisterTransformFactory("hpce.yc12015.direct_fourier_transform_parfor_inner", hpce::yc12015::Create_direct_fourier_transform_parfor_inner);
	RegisterTransformFactory("hpce.yc12015.direct_fourier_transform_parfor_outer", hpce::yc12015::Create_direct_fourier_transform_parfor_outer);
	RegisterTransformFactory("hpce.yc12015.fast_fourier_transform_taskgroup", hpce::yc12015::Create_fast_fourier_transform_taskgroup);
	RegisterTransformFactory("hpce.yc12015.fast_fourier_transform_parfor", hpce::yc12015::Create_fast_fourier_transform_parfor);
	RegisterTransformFactory("hpce.yc12015.fast_fourier_transform_combined", hpce::yc12015::Create_fast_fourier_transform_combined);
}

}; // namespace hpce
