#include <stdio.h>
#include <stdlib.h> // rand()
#include <time.h> // time()
#include <stdint.h>
#include <gmp.h>
#include <omp.h>

#include "rsa_p.h"
#include "utils.h"

int main(int argc, char* argv[]) {
	
	printf("Hallo Welt! RSA los gehts!\n");
	char* message = "Pajama Sam Pajama Sam Pajama Sam Pajama Sam Pajama Sam Pajama Sam Pajama Sam Pajama Sam Pajama Sam Pajama Sam Pajama Sam Pajama Sam Pajama Sam Pajama Sam Pajama Sam Pajama Sam";

	// generate two primes, p and q
	clock_t start = clock();
	mpz_t p;
	mpz_init(p);	
	
	mpz_t q;
	mpz_init(q);	

	while(1) {
		get_rand_prime(p);	
		get_rand_prime(q);
	
		mpz_t pq;
		mpz_init(pq);
		mpz_mul(pq, p, q);
		int num_bits = get_num_bits(pq);	
		mpz_clear(pq);
		if(num_bits != 64) continue; // 64-bit blocks

		int result = mpz_cmp(p, q);
		if(result == 0) continue; // must be different primes 
		else {
			mpz_print("p", p);			
			mpz_print("q", q);		
			break;
		}
	}
	print_elapsed("Getting p and q", start, clock());	

	mpz_t pq;
	mpz_init(pq);
	mpz_mul(pq, p, q);
	mpz_print("p*q", pq);

	// convert message to integer blocks
	int num_blocks;
	uint64_t* blocks = msg_to_int(message, &num_blocks);
	//print_blocks(blocks, num_blocks); // each block must be encrypted with rsa

	// the values of the blocks must be less than (p*q - 1)
	for(int i=0; i<num_blocks; i++) {
		uint64_t block = blocks[i];
		int result = mpz_cmp_ui(pq, block);
		if(result < 0) { // block values are larger than p*q
			printf("%i digits for random prime is insufficient.\n", NUM_DIGITS_P);
			return 0;
		}
	}

	mpz_t d;
	mpz_init(d);
	get_d(d, p, q);
	mpz_print("d", d);

	mpz_t e;
	mpz_init(e);
	get_e(e, p, q, d);
	mpz_print("e", e);	
	
	// n = p*q
	mpz_t n;
	mpz_init(n);
	mpz_set(n, p);
	mpz_mul(n, n, q);
	mpz_print("n", n);	

	// n must be less tha 2^64, since 64-bit blocks are used
	int result = mpz_cmp_ui(n, UINT64_MAX);
	if(result > 0) {
		printf("p*q is too large. Must be less than 2^64 (block size)\n");
		return 0;
	}

	/* Let's encrypt! */
	//start = clock();
    double start_time = omp_get_wtime();
    uint64_t* cipher_blocks = rsa(ENCRYPT, blocks, num_blocks, e, n);
	//print_blocks(cipher_blocks, num_blocks); // each block must be encrypted with rsa
	char* encrypted= int_to_msg(cipher_blocks, num_blocks);
    double elapsed = omp_get_wtime() - start_time;	
    printf("Encryption :%.5f sec\n", elapsed);
    //print_elapsed("Encryption", start, clock());	
    printf("Encrypted: %s\n", encrypted);

	/* Let's decrypt! */
	uint64_t* plaintext_blocks = rsa(DECRYPT, cipher_blocks, num_blocks, d, n);
	print_blocks(plaintext_blocks, num_blocks); // each block must be encrypted with rsa

	char* decrypted = int_to_msg(plaintext_blocks, num_blocks);
	printf("Decrypted: %s\n", decrypted);

	// clean up when done
	mpz_clear(p);
	mpz_clear(q);
	mpz_clear(pq);
	mpz_clear(d);
	mpz_clear(e);

	free(blocks);
	free(cipher_blocks);

	return 0;
}
