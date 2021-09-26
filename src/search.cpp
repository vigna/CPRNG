/*  Written in 2019-2021 by Guy Steele and Sebastiano Vigna

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.

See <http://creativecommons.org/publicdomain/zero/1.0/>. */

/* Searches for multipliers with good spectral properties by testing random
   candidates. Please see below where to insert your own candidate generation
   procedure.

   If MULT is defined, searches for multipliers for maximum-period MCGs with
   power-of-two moduli; otherwise, for full-period LCGs with power-of-two moduli.

   See also Karl Entacher & Thomas Schell's code associated with the paper

   Karl Entacher, Thomas Schell, and Andreas Uhl. Efficient lattice assessment
   for LCG and GLP parameter searches. Mathematics of Computation,
   71(239):1231–1242, 2002.

   at

   https://web.archive.org/web/20181128022136/http://random.mat.sbg.ac.at/results/karl/spectraltest/
*/

#include <iostream>
#include <NTL/LLL.h>

using namespace NTL;
using namespace std;
#include "common.cpp"

// Only multipliers with a minimum spectral score larger than this value will be printed.
const double threshold = 0.70;

int main(int argc, char *argv[]) {

	if (argc != 5 && argc != 6) {
		cerr << "USAGE: " << argv[0] << " SEED MAXDIM MODULUS MSIZE [ITERS]" << endl << endl;
		cerr << "Searches for multipliers with good spectral properties for" << endl;
#ifdef MULT
		cerr << "MCGs with power-of-two moduli by testing random candidates using" << endl;
#else
		cerr << "LCGs with power-of-two moduli by testing random candidates using" << endl;
#endif
		cerr << "the LLL lattice-reduction algorithm. For each multiplier with" << endl;
		cerr << "minimium figure of merit larger than " << threshold << " prints the minimum" << endl;
		cerr << "spectral score, the harmonic spectral score, the multiplier" << endl;
		cerr << "in decimal and hexadecimal, the figures of merit up to the" << endl;
		cerr << "specified maximum dimension (TAB-separated)." << endl;
		cerr << "ITER can be used to limit the number of iterations. If ITER" << endl;
		cerr << "is negative, -ITER multipliers of MSIZE bits are tested starting" << endl;
		cerr << "from SEED * 8 + 5; otherwise, SEED is used to seed a pseudorandom" << endl;
		cerr << "number generator that generates multipliers of MSIZE bits." << endl;
		exit(1);
	}

	char *end = NULL;
	uint64_t seed = strtoll(argv[1], &end, 0);
	if (*end) {
		cerr << "Unreadable seed: " << argv[1] << endl;
		exit(1);
	}

	const int max_dim = strtoll(argv[2], &end, 0);
	if (*end) {
		cerr << "Unreadable maximum dimension: " << argv[2] << endl;
		exit(1);
	}

	if (max_dim > dim_max) {
		cerr << "Maximum possible number of dimensions: " << dim_max << endl;
		exit(1);
	}

	if (max_dim < 2) {
		cerr << "Minimum possible number of dimensions: 2" << endl;
		exit(1);
	}

	ZZ mod = strtoZZ(argv[3]);
	if ((mod & (mod - 1)) != 0) {
		cerr << "The modulus must be a power of two" << endl;
		exit(1);
	}

	const int multiplier_size = strtoll(argv[4], &end, 0);
	if (*end) {
		cerr << "Unreadable multiplier size: " << argv[4] << endl;
		exit(1);
	}

	if (multiplier_size < 1) {
		cerr << "Multiplier size too small: " << multiplier_size << endl;
		exit(1);
	}

	int64_t iters = argc == 6 ? strtoll(argv[5], &end, 0) : numeric_limits<int64_t>::max();

	const bool random = iters >= 0;	
	if (! random) iters = -iters;

	if (*end) {
		cerr << "Unreadable number of iterations: " << argv[5] << endl;
		exit(1);
	}

	const ZZ multiplier_surround_bits = conv<ZZ>(1) << (multiplier_size - 1) | 5;
	const ZZ multiplier_mask = (conv<ZZ>(1) << (multiplier_size - 1)) - 8;

	if ((conv<ZZ>(1) << multiplier_size) > mod) {
		cerr << "Multiplier size too large for modulus: " << multiplier_size << endl;
		exit(1);
	}

	init(seed << 8 | multiplier_size);
	cerr << (random ? "Seed: 0x" : "Start: 0x") << hex << seed << endl;
	cerr << "Maximum dimension: " << dec << max_dim << endl;
	cerr << "Modulus: " << mod << endl;
	cerr << "Multiplier size: " << dec << multiplier_size << " bits " << endl;

#ifdef MULT
	// See Knuth TAoCP Vol. 2, 3.3.4, Exercise 20.
	mod /= 4;
#endif
	RR sqrt_mod = sqrt(conv<RR>(mod));

	// Compute the normalization factor starting from gamma_t
	for(int d = 2; d <= dim_max; d++)
		norm[d - 2] = conv<double>(conv<RR>(1) / (pow(conv<RR>(norm[d - 2]), conv<RR>(1./2)) * pow(conv<RR>(mod), conv<RR>(1) / conv<RR>(d))));

	ZZ a;
	mat_ZZ mat;
	double cur_fm[dim_max];
	double harm_norm = 0;

	for (int d = 2; d <= max_dim; d++) harm_norm += 1. / (d - 1);

	for (int64_t c = 0; c < iters; c++) {
		/* We generate only full-period multipliers of maximum potency, and,
		   in the multiplicative case, maximum-period multipliers whose
		   lattice of upper bits (minus the lowest two) is a translated
		   and scaled version of the lattice on all bits. In both cases,
		   these are exactly the multipliers whose residue modulo 8 is 5. */
		if (random) {
		    // Insert here your preferred candidate generation scheme
		    // Random odd multiplier in the range [2^(multiplier_size-1)..2^multiplier_size) whose residual modulo 8 is 5; it fits in multiplier_size bits
			a = ((((conv<ZZ>(0) + next()) << 192) + ((conv<ZZ>(0) + next()) << 128) + ((conv<ZZ>(0) + next()) << 64) + conv<ZZ>(next())) & multiplier_mask) | multiplier_surround_bits;
		}
		else a = ((((conv<ZZ>(0) + c) + seed) * 8) & multiplier_mask) | multiplier_surround_bits;

		double min_fm = numeric_limits<double>::infinity(), harm_score = 0;

		mat.SetDims(1, 1); // Reset

		for (int d = 2; d <= max_dim; d++) {
			mat.SetDims(d, d);
			// Dual lattice (see Knuth TAoCP Vol. 2, 3.3.4/B*).
			mat[0][0] = mod;
			for (int i = 1; i < d; i++) mat[i][i] = 1;
			for (int i = 1; i < d; i++) mat[i][0] = -power(a, i);
			ZZ det2;
			// LLL reduction with delta = 0.999999999
			LLL(det2, mat, 999999999, 1000000000);

			double min2 = numeric_limits<double>::infinity();
			for (int i = 0; i < d; i++) min2 = min(min2, conv<double>(mat[i] * mat[i]));

			cur_fm[d - 2] = norm[d - 2] * sqrt(min2);
			min_fm = min(min_fm, cur_fm[d - 2]);
			harm_score += cur_fm[d - 2] / (d - 1);
		}

		harm_score /= harm_norm;

		if (min_fm >= threshold) {
			printf("%8.6f\t%8.6f\t", min_fm, harm_score);
			cout << a << "\t" << "0x" << hex(a);
			for (int d = 2; d <= max_dim; d++) printf("\t%8.6f", cur_fm[d - 2]);
			printf("\n");
		}
	}
}
