#!/usr/bin/python

import sys
import math

for line in sys.stdin:
    a = line.split('\t')
    min_f = float(a[0])
    harm_f = float(a[1])
    m = int(a[2], 0)

    min_lag = 1
    for i in range(0,7):
        min_lag = min(min_lag, float(a[11 + i * 11]))

    print("%f\t%f\t%f\t%d\t0x%x\t%s" % (min_f, min_lag, harm_f, m, m, '\t'.join(a[4:11])))
    sys.stdout.flush()
