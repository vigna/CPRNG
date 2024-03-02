/*  Written in 2019-2021 by Sebastiano Vigna (vigna@acm.org)

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.

See <http://creativecommons.org/publicdomain/zero/1.0/>. */

/* Prints the figures of merit for a multiplier and a power-of-two
   modulus. If MULT is defined, computes figures of merit for an MCG;
   otherwise, for an LCG. */

#include <algorithm>

const int dim_max = 24;

typedef __uint128_t uint128_t;

/* These are the values of gamma_t in Knuth, taken from L'Ecuyer's Lattice Tester. */

static double norm[] = {
	1.1547005383793, // gamma_2
	1.2599210498949,
	1.4142135623731,
	1.5157165665104,
	1.6653663553112,
	1.8114473285278,
	2.0,
	2.0,
	2.0583720179295,
	2.140198065871,
	2.3094010767585,
	2.3563484301065,
	2.4886439198224,
	2.6390158215458,
	2.8284271247462,
	2.8866811540599,
	2.986825999361,
	3.0985192845333,
	3.2490095854249,
	3.3914559675101,
	3.5727801951422,
	3.7660273525956,
	4.0,
};

static inline uint64_t rotl(const uint64_t x, int k) {
	return (x << k) | (x >> (64 - k));
}

// xoshiro256**
static uint64_t s[4];

// Initialization by SplitMix64
static void init(uint64_t x) {
	for(int i = 0; i < 4; i++) {
		uint64_t z = (x += 0x9e3779b97f4a7c15);
		z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
		z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
		s[i] = z ^ (z >> 31);
	}
}

static uint64_t next(void) {
	const uint64_t result = rotl(s[1] * 5, 7) * 9;

	const uint64_t t = s[1] << 17;

	s[2] ^= s[0];
	s[3] ^= s[1];
	s[1] ^= s[2];
	s[0] ^= s[3];

	s[2] ^= t;

	s[3] = rotl(s[3], 45);

	return result;
}

// Conversion for large integers in hexadecimal form, too. Supports the "2^k" format.
static ZZ strtoZZ(const char * const s) {
	if (strstr(s, "2^") == s) {
		// Special case: power of 2
		long long p = strtoull(s + 2, NULL, 0);
		if (p == 0) {
			cerr << "Invalid power " << (s + 2) << endl;
			abort();
		}
		return conv<ZZ>(1) << p;
	}

	if (s[0] == '0' && tolower(s[1]) == 'x') {
		ZZ x = conv<ZZ>(0);
		for(size_t i = 2; i < strlen(s); i++) {
			x *= 16;
			if (!isdigit(s[i]) && (tolower(s[i]) < 'a' || tolower(s[i]) > 'f')) {
				cerr << "Invalid hexadecimal constant " << s << endl;
				abort();
			}
			x += isdigit(s[i]) ? s[i] - '0' : tolower(s[i]) - 'a' + 10;
		}

		return x;
	}

	for(size_t i = 0; i < strlen(s); i++) 
		if (!isdigit(s[i])) {
			cerr << "Invalid decimal constant " << s << endl;
			abort();
		}

	return conv<ZZ>(s);
}

#if defined(__clang__) && defined(__APPLE__)

// https://github.com/libntl/ntl/issues/28

template <> ZZ NTL_NAMESPACE::conv<>(const uint128_t& x) {
	return conv<ZZ, unsigned long>(uint64_t(x >> 64)) << 64 | conv<ZZ, unsigned long>((uint64_t&)x);
}

template <> uint128_t NTL_NAMESPACE::conv<>(const ZZ& x) {
	return uint128_t(conv<unsigned long>(x >> 64 & conv<ZZ>(0xFFFFFFFFFFFFFFFF))) << 64 | conv<unsigned long>(x & conv<ZZ>(0xFFFFFFFFFFFFFFFF));
}

#else

template <> ZZ NTL::conv<>(const uint128_t& x) {
	return conv<ZZ>(uint64_t(x >> 64)) << 64 | conv<ZZ>((uint64_t&)x);
}

template <> uint128_t NTL::conv<>(const ZZ& x) {
	return uint128_t(conv<uint64_t>(x >> 64 & conv<ZZ>(0xFFFFFFFFFFFFFFFF))) << 64 | conv<uint64_t>(x & conv<ZZ>(0xFFFFFFFFFFFFFFFF));
}

#endif

template <typename T> string hex(T);

template <> string hex<>(ZZ a) {
	// Hexadecimal representation of a
	string s = "";
	while(a != 0) {
		s += "0123456789abcdef"[a % 16];
		a /= 16;
	}
	reverse(s.begin(), s.end());
	return s;
}

#if defined(__clang__) && defined(__APPLE__)

template <> string hex<>(uint64_t a) {
	return hex(conv<ZZ, unsigned long>(a));
}

#else

template <> string hex<>(uint64_t a) {
	return hex(conv<ZZ>(a));
}

#endif

template <> string hex<>(uint128_t a) {
	return hex(conv<ZZ, uint128_t>(a));
}
