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

	// generate two large primes, p and q
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

	//// clean up when done
	//mpz_clear(p);
	//mpz_clear(q);
	//free(blocks);

	return 0;
}
