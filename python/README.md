Selection Scripts
=================

To reproduce the selection procedure used by [“Computationally easy,
spectrally good multipliers for congruential pseudorandom number
generators”](https://doi.org/10.1002/spe.3030) and by the LXM PRNG family
of Java 17 described by Guy Steele and Sebastiano Vigna in [“LXM: Better
Splittable Pseudorandom Number Generators (and Almost as
Fast)”](https://doi.org/10.1145/3485525) you must run `./filterall.sh` in a
directory containing the databases of candidate multipliers. The script
will generate aggregate files ending in `-L` which contain information about the
minimum lagged score.

At that point, `gensel.py` should be run as

    ./gensel.py 0.75 >best_harmonic.txt 2>best_minimum.txt

The parameter `0.75` specifies the quantile used to select multipliers
by minimum lagged score across all lags and dimensions.

The resulting two files contain for each type and size two choices. The
first choice is the best multiplier by the main criterion (e.g., by
harmonic score in `best_harmonic.txt`). The second choice is the best
multiplier by the main criterion within the first millile of the alternate
criterion (e.g., by harmonic score within the first millile by minimum
score in `best_harmonic.txt`).

The tables published in [“Computationally easy, spectrally good
multipliers for congruential pseudorandom number
generators”](https://doi.org/10.1002/spe.3030) come from the second choice
of `best_harmonic.txt`.

The multipliers used by the LXM family of Java 17 are once again the
second choice of `best_harmonic.txt`, but using a selection process
that ignores lagged scores:

    ./gensel.py 0 >best_harmonic.txt 2>best_minimum.txt
