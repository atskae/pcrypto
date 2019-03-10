#include <stdio.h>
#include <stdlib.h> // rand()
#include <time.h> // time()
#include <stdint.h>
#include <gmp.h>

#include "rsa.h"
#include "utils.h"

int main(int argc, char* argv[]) {
	
	printf("Hallo Welt! RSA los gehts!\n");
	char* message = "Pajama Sam!";

	// convert message to integer blocks
	int num_blocks;
	uint32_t* blocks = msg_to_int(message, &num_blocks);
	print_blocks(blocks, num_blocks); // each block must be encrypted with rsa

	// generate two primes, p and q
	clock_t start = clock();
	mpz_t p;
	mpz_init(p);	
	
	mpz_t q;
	mpz_init(q);	

	while(1) {
		get_rand_prime(p);	
		get_rand_prime(q);

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
	mpz_sub_ui(pq, pq, 1);
	mpz_print("p*q", pq);

	// the values of the blocks must be less than (p*q - 1)
	for(int i=0; i<num_blocks; i++) {
		uint32_t block = blocks[i];
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

	// clean up when done
	mpz_clear(p);
	mpz_clear(q);
	mpz_clear(pq);
	mpz_clear(d);
	mpz_clear(e);

	free(blocks);

	return 0;
}
