/*  Written in 2021 by Sebastiano Vigna

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.

See <http://creativecommons.org/publicdomain/zero/1.0/>. */

/* 

Microbenchmarks for multipliers of different sizes. Please compile with

gcc -fno-unroll-loops -fomit-frame-pointer -O3 -DCONST=0 test.c -o test-mcg
gcc -fno-unroll-loops -fomit-frame-pointer -O3 -DCONST=0xdeadbeefdeadf00d test.c -o test-lcg

(or your favorite compiler) and run with

./test-mcg 1000000000
./test-lcg 1000000000

*/

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>

uint64_t get_user_time(void) {
	struct rusage rusage;
	getrusage(0, &rusage);
	return rusage.ru_utime.tv_sec * 1000000000ULL + rusage.ru_utime.tv_usec * 1000ULL;
}

__uint128_t __attribute__((noinline)) f64(__uint128_t a) {
	return a * 0xdeadf00ddeadf00d + CONST;
}

__uint128_t __attribute__((noinline)) f65(__uint128_t a) {
	return a * (((__uint128_t)1 << 64) + 0xdeadf00ddeadf00d) + CONST;
}

__uint128_t __attribute__((noinline)) f66_2(__uint128_t a) {
	return a * (((__uint128_t)2 << 64) + 0xdeadf00ddeadf00d) + CONST;
}

__uint128_t __attribute__((noinline)) f66_3(__uint128_t a) {
	return a * (((__uint128_t)3 << 64) + 0xdeadf00ddeadf00d) + CONST;
}

__uint128_t __attribute__((noinline)) f67_4(__uint128_t a) {
	return a * (((__uint128_t)4 << 64) + 0xdeadf00ddeadf00d) + CONST;
}

__uint128_t __attribute__((noinline)) f67_5(__uint128_t a) {
	return a * (((__uint128_t)5 << 64) + 0xdeadf00ddeadf00d) + CONST;
}

__uint128_t __attribute__((noinline)) f67_6(__uint128_t a) {
	return a * (((__uint128_t)6 << 64) + 0xdeadf00ddeadf00d) + CONST;
}

__uint128_t __attribute__((noinline)) f67_7(__uint128_t a) {
	return a * (((__uint128_t)7 << 64) + 0xdeadf00ddeadf00d) + CONST;
}

__uint128_t __attribute__((noinline)) f68_0xc(__uint128_t a) {
	return a * (((__uint128_t)0xc << 64) + 0xdeadf00ddeadf00d) + CONST;
}

__uint128_t __attribute__((noinline)) f69_0x18(__uint128_t a) {
	return a * (((__uint128_t)0x18 << 64) + 0xdeadf00ddeadf00d) + CONST;
}

__uint128_t __attribute__((noinline)) f72(__uint128_t a) {
	return a * (((__uint128_t)0xEF << 64) + 0xdeadf00ddeadf00d) + CONST;
}

__uint128_t __attribute__((noinline)) f80(__uint128_t a) {
	return a * (((__uint128_t)0xBEEF << 64) + 0xdeadf00ddeadf00d) + CONST;
}

__uint128_t __attribute__((noinline)) f81(__uint128_t a) {
	return a * (((__uint128_t)0x1BEEF << 64) + 0xdeadf00ddeadf00d) + CONST;
}

__uint128_t __attribute__((noinline)) f95(__uint128_t a) {
	return a * (((__uint128_t)0x7EADBEEF << 64) + 0xdeadf00ddeadf00d) + CONST;
}

__uint128_t __attribute__((noinline)) f96(__uint128_t a) {
	return a * (((__uint128_t)0xDEADBEEF << 64) + 0xdeadf00ddeadf00d) + CONST;
}

__uint128_t __attribute__((noinline)) f128(__uint128_t a) {
	return a * (((__uint128_t)0xDEADBEEFDEADBEEF << 64) + 0xdeadf00ddeadf00d) + CONST;
}


int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "USAGE: %s ITERATIONS\n", argv[0]);
		exit(1);
	}
	const uint64_t n = strtoll(argv[1], NULL, 0);

	__uint128_t x = n;

	uint64_t start_time = get_user_time();
	for(int64_t i = n; i-- != 0;)
		x = f64(x);

	uint64_t time_delta = (get_user_time() - start_time);
	printf("64: %s: %.03f s, %.03f GB/s, %.03f words/ns, %.03f ns/word\n", argv[0], time_delta / 1E9, (n * 8.) / time_delta, n / (double)time_delta, time_delta / (double)n);

	start_time = get_user_time();
	for(int64_t i = n; i-- != 0;)
		x = f65(x);

	time_delta = (get_user_time() - start_time);
	printf("65: %s: %.03f s, %.03f GB/s, %.03f words/ns, %.03f ns/word\n", argv[0], time_delta / 1E9, (n * 8.) / time_delta, n / (double)time_delta, time_delta / (double)n);

	start_time = get_user_time();
	for(int64_t i = n; i-- != 0;)
		x = f66_2(x);

	time_delta = (get_user_time() - start_time);
	printf("66_2: %s: %.03f s, %.03f GB/s, %.03f words/ns, %.03f ns/word\n", argv[0], time_delta / 1E9, (n * 8.) / time_delta, n / (double)time_delta, time_delta / (double)n);

	start_time = get_user_time();
	for(int64_t i = n; i-- != 0;)
		x = f66_3(x);

	time_delta = (get_user_time() - start_time);
	printf("66_3: %s: %.03f s, %.03f GB/s, %.03f words/ns, %.03f ns/word\n", argv[0], time_delta / 1E9, (n * 8.) / time_delta, n / (double)time_delta, time_delta / (double)n);

	start_time = get_user_time();
	for(int64_t i = n; i-- != 0;)
		x = f67_4(x);

	time_delta = (get_user_time() - start_time);
	printf("67_4: %s: %.03f s, %.03f GB/s, %.03f words/ns, %.03f ns/word\n", argv[0], time_delta / 1E9, (n * 8.) / time_delta, n / (double)time_delta, time_delta / (double)n);


	start_time = get_user_time();
	for(int64_t i = n; i-- != 0;)
		x = f67_5(x);

	time_delta = (get_user_time() - start_time);
	printf("67_5: %s: %.03f s, %.03f GB/s, %.03f words/ns, %.03f ns/word\n", argv[0], time_delta / 1E9, (n * 8.) / time_delta, n / (double)time_delta, time_delta / (double)n);


	start_time = get_user_time();
	for(int64_t i = n; i-- != 0;)
		x = f67_6(x);

	time_delta = (get_user_time() - start_time);
	printf("67_6: %s: %.03f s, %.03f GB/s, %.03f words/ns, %.03f ns/word\n", argv[0], time_delta / 1E9, (n * 8.) / time_delta, n / (double)time_delta, time_delta / (double)n);


	start_time = get_user_time();
	for(int64_t i = n; i-- != 0;)
		x = f67_7(x);

	time_delta = (get_user_time() - start_time);
	printf("67_7: %s: %.03f s, %.03f GB/s, %.03f words/ns, %.03f ns/word\n", argv[0], time_delta / 1E9, (n * 8.) / time_delta, n / (double)time_delta, time_delta / (double)n);


	start_time = get_user_time();
	for(int64_t i = n; i-- != 0;)
		x = f68_0xc(x);

	time_delta = (get_user_time() - start_time);
	printf("68_0xc: %s: %.03f s, %.03f GB/s, %.03f words/ns, %.03f ns/word\n", argv[0], time_delta / 1E9, (n * 8.) / time_delta, n / (double)time_delta, time_delta / (double)n);


	start_time = get_user_time();
	for(int64_t i = n; i-- != 0;)
		x = f69_0x18(x);

	time_delta = (get_user_time() - start_time);
	printf("69_0x18: %s: %.03f s, %.03f GB/s, %.03f words/ns, %.03f ns/word\n", argv[0], time_delta / 1E9, (n * 8.) / time_delta, n / (double)time_delta, time_delta / (double)n);


	start_time = get_user_time();
	for(int64_t i = n; i-- != 0;)
		x = f72(x);

	time_delta = (get_user_time() - start_time);
	printf("72: %s: %.03f s, %.03f GB/s, %.03f words/ns, %.03f ns/word\n", argv[0], time_delta / 1E9, (n * 8.) / time_delta, n / (double)time_delta, time_delta / (double)n);


	start_time = get_user_time();
	for(int64_t i = n; i-- != 0;)
		x = f80(x);

	time_delta = (get_user_time() - start_time);
	printf("80: %s: %.03f s, %.03f GB/s, %.03f words/ns, %.03f ns/word\n", argv[0], time_delta / 1E9, (n * 8.) / time_delta, n / (double)time_delta, time_delta / (double)n);


	start_time = get_user_time();
	for(int64_t i = n; i-- != 0;)
		x = f81(x);

	time_delta = (get_user_time() - start_time);
	printf("81: %s: %.03f s, %.03f GB/s, %.03f words/ns, %.03f ns/word\n", argv[0], time_delta / 1E9, (n * 8.) / time_delta, n / (double)time_delta, time_delta / (double)n);


	start_time = get_user_time();
	for(int64_t i = n; i-- != 0;)
		x = f95(x);

	time_delta = (get_user_time() - start_time);
	printf("95: %s: %.03f s, %.03f GB/s, %.03f words/ns, %.03f ns/word\n", argv[0], time_delta / 1E9, (n * 8.) / time_delta, n / (double)time_delta, time_delta / (double)n);


	start_time = get_user_time();
	for(int64_t i = n; i-- != 0;)
		x = f96(x);

	time_delta = (get_user_time() - start_time);
	printf("96: %s: %.03f s, %.03f GB/s, %.03f words/ns, %.03f ns/word\n", argv[0], time_delta / 1E9, (n * 8.) / time_delta, n / (double)time_delta, time_delta / (double)n);


	start_time = get_user_time();
	for(int64_t i = n; i-- != 0;)
		x = f128(x);

	time_delta = (get_user_time() - start_time);
	printf("128: %s: %.03f s, %.03f GB/s, %.03f words/ns, %.03f ns/word\n", argv[0], time_delta / 1E9, (n * 8.) / time_delta, n / (double)time_delta, time_delta / (double)n);

	const volatile __uint128_t unused = x;
	return 0;
}
