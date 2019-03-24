#ifndef UTILS_H
#define UTILS_H

#include <time.h>
#include <gmp.h>

#define ASCII_0 48
#define ASCII_9 57

void print_elapsed(char* msg, clock_t start, clock_t stop);
void print_bits8(char n);
void print_bits64(uint64_t n);
void print_blocks(uint64_t* blocks, int num_blocks);

int get_num_digits(mpz_t n);
int get_num_bits(mpz_t n);
void uint64_to_mpz(uint64_t* n, mpz_t a);
uint64_t mpz_to_uint64(mpz_t a);
void get_rand_intStr(char* intStr, int num_digits);

#endif // UTILS_H
