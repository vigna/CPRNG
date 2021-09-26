#!/bin/bash 

TYPE=$1
STATE=$2
BITS=$3
echo "Filtering $1-$2-$3" 1>&2

paste $TYPE-$STATE-$BITS $(for((i=2; i<=8; i++)); do echo -n " $TYPE-$STATE-$i-$BITS"; done) | ./filter.py
