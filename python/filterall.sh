#!/bin/bash

for b in 16 17 18 19 24 32; do ( ./filter.sh LCG 32 $b > LCG-32-$b-L ) ; done
for b in 32 33 34 35 48 64; do ( ./filter.sh LCG 64 $b > LCG-64-$b-L ) ; done
for b in 64 65 66 67 68 69 70 71 72 80 96 128; do ( ./filter.sh LCG 128 $b > LCG-128-$b-L ) ; done

for b in 15 16 17 18 19 24 32; do ( ./filter.sh MCG 32 $b > MCG-32-$b-L ) ; done
for b in 31 32 33 34 35 48 64; do ( ./filter.sh MCG 64 $b > MCG-64-$b-L ) ; done
for b in 63 64 65 66 67 68 69 70 71 72 80 96 128; do ( ./filter.sh MCG 128 $b > MCG-128-$b-L ) ; done
