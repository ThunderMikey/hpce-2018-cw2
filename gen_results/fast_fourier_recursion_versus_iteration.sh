#!/bin/bash
# this script should be run in repo root

WORK_DIR="data"
TEST_NAME="fast_fourier_recursion_versus_iteration"
# seconds is enough for n=4096 on intel core 6700hq
T_LIMIT=30

REC_KS="1 4 32 64 128"
LOOP_KS="1 4 32 64 128"

CSV_HEAD="RECURSION_K, LOOP_K, name, allowedP, trueP, n, [sentinel], time"

mkdir -p $WORK_DIR

# add K to each row
function add_k()
{
  # $1: file_name
  # $2: RECURSION_K
  # $3: LOOP_K
  sed -i -e "1 ! {s/^/$2, $3, /}" $1
}

function delete_comment()
{
  # $1 file
  sed -i '/^#/d' $1
}

function run_tests()
{
for loop_k in $LOOP_KS; do
  for rec_k in $REC_KS; do
    R_FILE="$WORK_DIR/${TEST_NAME}_R${rec_k}_L${loop_k}.csv"
    echo $CSV_HEAD > $R_FILE
    HPCE_FFT_RECURSION_K=$rec_k \
    HPCE_FFT_LOOP_K=$loop_k \
      bin/time_fourier_transform \
      hpce.yc12015.fast_fourier_transform_combined 0 $T_LIMIT \
      >> $R_FILE
    delete_comment $R_FILE
    add_k $R_FILE $rec_k $loop_k
  done
done
}

function gather_results()
{
  SUM_FILE="$WORK_DIR/${TEST_NAME}.csv"
  echo $CSV_HEAD > $SUM_FILE
  for loop_k in $LOOP_KS; do
    for rec_k in $REC_KS; do
      R_FILE="$WORK_DIR/${TEST_NAME}_R${rec_k}_L${loop_k}.csv"
      sed -n "1!p" $R_FILE >> $SUM_FILE
    done
  done
}

run_tests

gather_results

