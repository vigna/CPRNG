Code for the Spectral Test
==========================

The code in this directory deals with (approximate) spectral figures of
merit and aggregate scores for full-period congruential generators
(including LCGs with power-of-two moduli and MCGs with prime moduli) and
for MCGs with power-of-two moduli. It uses [NTL](https://www.shoup.net/ntl/).

- `search.cpp` code searches for good multipliers for LCGs and MCGs with
  power-of-two moduli.

- `spect.cpp` prints spectral scores and figures of merit for a given multiplier.

- `printdat.cpp` prints configuration files for
  [LatticeTester](https://github.com/umontreal-simul/latticetester) for a
  given multiplier.

- `benchmark.c` is a simple microbenchmark comparing different multiplier
  sizes (compilation instructions can be found at the start of the file).

The code for spectral figures of merit is based on the
Lenstra–Lenstra–Lovász lattice basis reduction algorithm, and as such it
computes approximate results. Up to dimension 8, however, the
approximation is usually excellent (see
<https://doi.org/10.1090/S0025-5718-01-01415-6>). If you want to get exact
results for a multiplier, use LatticeTester with the configuration files
provided by `printdat`. Note that you can specify the modulus using the
notation `2^k`.

The `comp.sh` script will compile the sources above. The executables
for MCGs with power-of-two moduli will be prefixed by an `m`.
