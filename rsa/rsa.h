#ifndef RSA_H
#define RSA_H

#define NUM_DIGITS_P 8

uint32_t* msg_to_int(char* msg, int* num_blocks);
void mpz_print(char* name, mpz_t n);
void get_rand_prime(mpz_t p);
void get_d(mpz_t d, mpz_t p, mpz_t q);
void get_e(mpz_t e, mpz_t p, mpz_t q, mpz_t d);

#endif // RSA_H
