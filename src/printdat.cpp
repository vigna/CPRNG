/*  Written in 2019-2021 by Sebastiano Vigna (vigna@acm.org)

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.

See <http://creativecommons.org/publicdomain/zero/1.0/>. */

/* Writes input files for LatticeTester (https://github.com/umontreal-simul/latticetester).

   If MULT is defined, outputs data for an MCG with power-of-two modulus;
   otherwise, for a full-period congruential generator (including LCGs with
   power-of-two moduli and MCGs with prime moduli). */

#include <iostream>
#include <fstream>
#include <NTL/LLL.h>

using namespace NTL;
using namespace std;
#include "common.cpp"

int main(int argc, char *argv[]) {
	if (argc != 6) {
		cerr << "USAGE: " << argv[0] << " LAG MAXDIM MULTIPLIER MODULUS BASENAME" << endl << endl;
		cerr << "Prints input files for LatticeTester to compute the spectral" << endl;
#ifdef MULT
		cerr << "figures of merit for MCGs with power-of-two moduli" << endl;
#else
		cerr << "figures of merit for full-period congruential generators (including" << endl;
		cerr << "LCGs with power-of-two moduli and MCGs with prime moduli) " << endl;
#endif
		cerr << "up to the specified maximum dimension for the given lag." << endl;
		exit(1);
	}

	const int lag = atoi(argv[1]);

	if (lag < 1) {
		cerr << "The lag must be strictly positive" << endl;
		exit(1);
	}

	const int max_dim = atoi(argv[2]);

	if (max_dim < 2) {
		cerr << "Minimum possible number of dimensions: 2" << endl;
		exit(1);
	}

	ZZ a = strtoZZ(argv[3]);
	ZZ mod = strtoZZ(argv[4]);
#ifdef MULT
	if ((mod & (mod - 1)) != 0) {
		cerr << "The modulus must be a power of two" << endl;
		exit(1);
	}
#endif

	if (a >= mod) {
		cerr << "The multiplier must be smaller than the modulus" << endl;
		exit(1);
	}

	string basename(argv[5]);

	mat_ZZ x;
	x.SetDims(max_dim, max_dim);

	// Entacher's characterization of lagged lattices (https://dl.acm.org/doi/10.1145/301677.301682)
	ZZ alag = PowerMod(a, lag, mod);
	mod /= GCD(mod, conv<ZZ>(lag));
#ifdef MULT
	// See Knuth TAoCP Vol. 2, 3.3.4, Exercise 20.
	mod /= 4;
#endif

	for (int d = 2; d <= max_dim; d++) {
		ofstream s;
		s.open(basename + "-" + to_string(d) + ".dat");
		s << "SPECTRAL BESTLAT" << endl << "BKZ ARBITRARY 0.9999999999 100" << endl << d << endl;
		x.SetDims(d, d);
		// Dual lattice (see Knuth TAoCP Vol. 2, 3.3.4/B*).
		x[0][0] = mod;
		for (int i = 1; i < d; i++) x[i][i] = 1;
		for (int i = 1; i < d; i++) x[i][0] = -power(alag, i);

		for(int i = 0; i < d; i++) {
			for(int j = 0; j < d; j++) {
				s << x[i][j] << " ";
			}
			s << endl;
		}

		// Max nodes, output
		s << 1000000000 << endl << "TERMINAL" << endl;
		s.close();
	}
}
