#!/bin/bash
# this script should be run in repo root

WORK_DIR="data"
TEST_NAME="sequential_direct_fourier_transform"
T_LIMIT=30
CSV_HEAD="name, allowedP, trueP, n, [sentinel], time"

mkdir -p $WORK_DIR

function delete_comment()
{
  # $1 file
  sed -i '/^#/d' $1
}

function run_tests()
{
  R_FILE="$WORK_DIR/${TEST_NAME}.csv"
  echo $CSV_HEAD > $R_FILE
  bin/time_fourier_transform \
    hpce.direct_fourier_transform 0 $T_LIMIT \
    >> $R_FILE
  delete_comment $R_FILE
}

run_tests

