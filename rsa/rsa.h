#ifndef RSA_H
#define RSA_H

#define ASCII_0 48
#define ASCII_9 57

#define NUM_DIGITS_PQ 3

//int gcd(int x, int y);
void mpz_print(char* name, mpz_t n);

void get_rand_prime(mpz_t p);

#endif // RSA_H
