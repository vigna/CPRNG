#!/usr/bin/python3

#  Written in 2019 by Sebastiano Vigna
#
# To the extent possible under law, the author has dedicated all copyright
# and related and neighboring rights to this software to the public domain
# worldwide. This software is distributed without any warranty.
#
# See <http://creativecommons.org/publicdomain/zero/1.0/>.

import math
import sys
import pandas as pd 

if len(sys.argv) < 2:
    sys.stderr.write("Usage: %s QUANTILE\n" % sys.argv[0])
    exit()

files = [
	["LCG-32-16", 3],
	["LCG-32-17", 3],
	["LCG-32-18", 3],
	["LCG-32-19", 3],
	["LCG-32-24", 3],
	["LCG-32-32", 0],
	["MCG-32-15", 3],
	["MCG-32-16", 3],
	["MCG-32-17", 3],
	["MCG-32-18", 3],
	["MCG-32-19", 3],
	["MCG-32-24", 3],
	["MCG-32-32", 0],
	["LCG-64-32", 3],
	["LCG-64-33", 3],
	["LCG-64-34", 3],
	["LCG-64-35", 3],
	["LCG-64-48", 0],
	["LCG-64-64", 0],
	["MCG-64-31", 3],
	["MCG-64-32", 3],
	["MCG-64-33", 3],
	["MCG-64-34", 3],
	["MCG-64-35", 3],
	["MCG-64-48", 0],
	["MCG-64-64", 0],
	["LCG-128-64", 3],
	["LCG-128-65", 3],
	["LCG-128-66", 3],
	["LCG-128-67", 3],
	["LCG-128-68", 3],
	["LCG-128-69", 3],
	["LCG-128-70", 3],
	["LCG-128-71", 3],
	["LCG-128-72", 3],
	["LCG-128-80", 3],
	["LCG-128-96", 0],
	["LCG-128-128", 0],
	["MCG-128-63", 3],
	["MCG-128-64", 3],
	["MCG-128-65", 3],
	["MCG-128-66", 3],
	["MCG-128-67", 3],
	["MCG-128-68", 3],
	["MCG-128-69", 3],
	["MCG-128-70", 3],
	["MCG-128-71", 3],
	["MCG-128-72", 3],
	["MCG-128-80", 3],
	["MCG-128-96", 0],
	["MCG-128-128", 0],
]

for p in files:

    file = p[0] + "-L"
    hi_mask = p[1]
    data = pd.read_csv(file, sep='\t', names = ['M₈', 'L₈', 'H₈', 'd', 'h', 'f₂', 'f₃', 'f₄', 'f₅', 'f₆', 'f₇', 'f₈'], index_col=False, dtype={'h':'str'}) 

    if hi_mask != 0:
        mask = hi_mask << (math.floor(math.log(int(data['d'][0]), 2)) - math.floor(math.log(hi_mask, 2)))
        # Manually select lines satisfying the mask (too large integers for pandas)
        loc = []
        for i in range(len(data)):
            if (int(data['d'][i]) & mask) == mask:
                loc.append(i)
        data = data.loc[loc]

    lag_threshold = data['L₈'].quantile(float(sys.argv[1]));

    # Filter away candidates below the lag threshold
    data = data[data['L₈'] >= lag_threshold]
    # Discard candidates with know problems in high dimension
    s = (
            set(["0x738f6e765", "0xe264473d4e90bb31d", "0x719bb1e7f56883e5d", "0x1da2e4605", "0x1e5e536a3711bc7755", "0x1f3ad22e4bd1a692d", "0x3ad59455e2c68e647d", "0x3e0d997a645f176dd", "0x67c4de5a048ebff1d", "0x728917326ee7fe425", "0x7c87460e5", "0x87338161ef95", "0x87ea3de194dd2e97074f3d0c2ea63d35", "0xb4d13225", "0xc1be9eb68e95", "0xc35bd29e4b5db7ff6d", "0xc478db86929909e45", "0xc8a0bcbd37f06521c5", "0xca7592823d4c35535", "0xddb80b9ccb1066bee495", "0xee3599ad", "0xef78d07615711c7d", "0xf8d79515e58138fd", "0xfaf00a25", "0xfe2d84b0671aa6f869bd", "0xff2826ad","0xee3599ad"])
            if p[0][:3] == "LCG"
            else set(["0xee3cc725", "0xe6dccdea3cce7675", "0xf43855c5", "0xcb2dc73bb3a3e8765", "0x1c8134a55f3b9968d", "0x1e5daed42b68e08645", "0x1fa661a55", "0x31e2d", "0x35328ed0d", "0x365b6a2e27d09e2fd", "0x3782c32a82c5dbf4f5", "0x3d30df295", "0x3ea015695", "0x689a0e46d", "0x71217c8b506a03245", "0x78eead14469fc5733d", "0x79f0d", "0x82c1fcad", "0x83b5b142866da9d5", "0xa5c68ce1b17d", "0xaa7a0cf56782fcf5", "0xae36bfb5", "0xae3cc725", "0xc2c1fcad", "0xc3a77d", "0xc7fb6d", "0xe6fd44dd", "0xecbce6ad", "0xed126c68193f2a63846d", "0xee36bfb5", "0xef33769d", "0xf06747158d1be36d", "0xf5ec16ed", "0xf6473f07ba5d", "0xf6ff3eae6408c3dd7d", "0xf7540ec5", "0xf97d5a195ad7c421d","0xaf33769d"])
        )

    data = data[~data['h'].isin(s)]

    lowerm = data['M₈'].quantile(.999)
    lowerh = data['H₈'].quantile(.999)

    besth = data.sort_values('H₈', ascending=False).iloc[0]['h']
    sys.stdout.write("%s\t%s\n" % (file, besth))
    row = data[data['M₈'] >= lowerm].sort_values('H₈', ascending=False).iloc[0]
    besth2 = row['h']
    sys.stdout.write("%s\t%s\n\n\n" % (file, besth2))

    bestm = data.sort_values('M₈', ascending=False).iloc[0]['h']
    sys.stderr.write("%s\t%s\n" % (file, bestm))
    row = data[data['H₈'] >= lowerh].sort_values('M₈', ascending=False).iloc[0]
    bestm2 = row['h']
    sys.stderr.write("%s\t%s\n\n\n" % (file, bestm2))
