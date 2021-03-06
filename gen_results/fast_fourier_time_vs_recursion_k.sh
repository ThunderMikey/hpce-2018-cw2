#!/bin/bash
# this script should be run in repo root

WORK_DIR="data"
TEST_NAME="fast_fourier_time_vs_recursion_k"
# seconds is enough for n=4096 on intel core 6700hq
T_LIMIT=30

KS="2 4 8 16 32 64"

CSV_HEAD="K, name, allowedP, trueP, n, [sentinel], time"

mkdir -p $WORK_DIR

# add K to each row
function add_k()
{
  # $1: file_name
  # $2: K value
  sed -i -e "1 ! {s/^/$2, /}" $1
}

function delete_comment()
{
  # $1 file
  sed -i '/^#/d' $1
}

function run_tests()
{
for k in $KS; do
  R_FILE="$WORK_DIR/${TEST_NAME}_$k.csv"
  echo $CSV_HEAD > $R_FILE
  HPCE_FFT_RECURSION_K=$k \
    bin/time_fourier_transform \
    hpce.yc12015.fast_fourier_transform_taskgroup 0 $T_LIMIT \
    >> $R_FILE
  delete_comment $R_FILE
  add_k $R_FILE $k
done
}

function gather_results()
{
  SUM_FILE="$WORK_DIR/${TEST_NAME}.csv"
  echo $CSV_HEAD > $SUM_FILE
  for k in $KS; do
    R_FILE="$WORK_DIR/${TEST_NAME}_$k.csv"
    sed -n "1!p" $R_FILE >> $SUM_FILE
  done
}

run_tests

gather_results

