#ifndef RSA_H
#define RSA_H

#define NUM_DIGITS_P 10

#define BLOCK_SIZE_BITS 64
#define CHARS_PER_BLOCK (BLOCK_SIZE_BITS / 8)

#define ENCRYPT 0
#define DECRYPT 1

/* Sequential */
uint64_t* rsa(int mode, uint64_t* blocks, int num_blocks, mpz_t e_or_d, mpz_t n);
char* int_to_msg(uint64_t* blocks, int num_blocks);
uint64_t* msg_to_int(char* msg, int* num_blocks);
void mpz_print(char* name, mpz_t n);
void get_rand_prime(mpz_t p);
void get_d(mpz_t d, mpz_t p, mpz_t q);
void get_e(mpz_t e, mpz_t p, mpz_t q, mpz_t d);
void get_p_q(mpz_t p, mpz_t q);

/* Parallel */
uint64_t* p_rsa(int mode, uint64_t* blocks, int num_blocks, mpz_t e_or_d, mpz_t n);
void p_get_rand_prime(mpz_t p);
void p_get_p_q(mpz_t p, mpz_t q);

#endif // RSA_H
