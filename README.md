CPRNG
=====

This repository contains data and code associated with the paper
[“Computationally easy, spectrally good multipliers for congruential
pseudorandom number generators”](https://doi.org/10.1002/spe.3030), by Guy
Steele and Sebastiano Vigna.

The directory `src` contains the code used to sample multipliers.

The directory `python` contains the Python code used to select the
multipliers reported in the paper.

The databases of candidate multipliers are available at
http://vigna.di.unimi.it/CPRNG.tar.bz2. (The size of the bzip2 file is
approximately 14 gigabytes; the unzipped data is approximately 36
gigabytes.)

The databases contain, for each type, state size, and multiplier size,
spectral scores (e.g., `LCG-128-128` for spectral scores of 128-bit
multipliers for LCGs with 128 bits of state) and lagged spectral scores
(e.g., `MCG-128-3-64` for 3-lagged scores of 64-bit multipliers for MCGs
with 128 bits of state). For each multiplier we list the minimum score
from dimension 2 to dimension 8, the harmonic score (see the paper) from
dimension 2 to dimension 8, the multiplier in decimal and hexadecimal
form, and finally the spectral scores from dimension 2 to dimension 8.
