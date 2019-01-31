#!/bin/bash
# this script should be run in repo root

WORK_DIR="data"
TEST_NAME="direct_outer_time_versus_p"
T_LIMIT=30
CSV_HEAD="name, allowedP, trueP, n, [sentinel], time"
# processors
PS=$(seq 1 8)
CHUNK_SZIE=16

mkdir -p $WORK_DIR

function delete_comment()
{
  # $1 file
  sed -i '/^#/d' $1
}

function run_tests()
{
for p in $PS; do
  R_FILE="$WORK_DIR/${TEST_NAME}_$p.csv"
  echo $CSV_HEAD > $R_FILE
  HPCE_DIRECT_OUTER_K=$CHUNK_SZIE \
    bin/time_fourier_transform \
    hpce.yc12015.direct_fourier_transform_parfor_outer $p $T_LIMIT \
    >> $R_FILE
  delete_comment $R_FILE
done
}

function gather_results()
{
  SUM_FILE="$WORK_DIR/${TEST_NAME}.csv"
  echo $CSV_HEAD > $SUM_FILE
  for p in $PS; do
    R_FILE="$WORK_DIR/${TEST_NAME}_$p.csv"
    sed -n "1!p" $R_FILE >> $SUM_FILE
  done
}

run_tests

gather_results

