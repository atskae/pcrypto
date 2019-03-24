#ifndef RSA_H
#define RSA_H

#define NUM_DIGITS_P 10

#define BLOCK_SIZE_BITS 64
#define CHARS_PER_BLOCK (BLOCK_SIZE_BITS / 8)

uint64_t* rsa_encrypt(uint64_t* blocks, int num_blocks, mpz_t e, mpz_t n);
uint64_t* msg_to_int(char* msg, int* num_blocks, mpz_t pq);
void mpz_print(char* name, mpz_t n);
void get_rand_prime(mpz_t p);
void get_d(mpz_t d, mpz_t p, mpz_t q);
void get_e(mpz_t e, mpz_t p, mpz_t q, mpz_t d);

#endif // RSA_H
