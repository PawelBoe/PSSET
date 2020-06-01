#!/bin/bash

OUTFILE="psset.h"
TMPFILE="tmp"
HEADERS=("sparse_set.h" "sparse_map.h" "sparse_factory.h")

rm $OUTFILE

for VALUE in "${HEADERS[@]}"
do
    sed -e '/#include "'${HEADERS[0]}'"/d' -e '/#include "'${HEADERS[1]}'"/d' -e '/#include "'${HEADERS[2]}'"/d' $VALUE >> $OUTFILE
done
