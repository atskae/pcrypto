#include <stdio.h>
#include <stdlib.h> // rand()
#include <time.h> // time()
#include <stdint.h>
#include <string.h>
#include <gmp.h>

#include "rsa.h"
#include "utils.h"

int main(int argc, char* argv[]) {
	
	printf("Hallo Welt! RSA los gehts!\n");
	
    // statistics
    //printf("Statistics to be collected:\n");
    //for(int i=0; i<NUM_STATS; i++ ){
    //    printf("%i: %s\n", i, all_stats[i]);
    //}
    double s_stats[NUM_STATS] = {0.0};
    double p_stats[NUM_STATS] = {0.0};

    //char* message = "Pajama Sam";
  
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
			//mpz_print("p", p);			
			//mpz_print("q", q);		
			break;
		}
	}
    clock_t end = clock();
	print_elapsed("Getting p and q", start, end);
    s_stats[STAT_GET_PQ] = get_seconds(end - start);

	mpz_t pq;
	mpz_init(pq);
	mpz_mul(pq, p, q);
	//mpz_print("p*q", pq);

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
	//mpz_print("d", d);

	mpz_t e;
	mpz_init(e);
	get_e(e, p, q, d);
	//mpz_print("e", e);	
	
	// n = p*q
	mpz_t n;
	mpz_init(n);
	mpz_set(n, p);
	mpz_mul(n, n, q);
	//mpz_print("n", n);	

	// n must be less tha 2^64, since 64-bit blocks are used
	int result = mpz_cmp_ui(n, UINT64_MAX);
	if(result > 0) {
		printf("p*q is too large. Must be less than 2^64 (block size)\n");
		return 0;
	}

	/* Let's encrypt! */
	
    // sequential
    start = clock();
    uint64_t* cipher_blocks = rsa(ENCRYPT, blocks, num_blocks, e, n);
	char* encrypted = int_to_msg(cipher_blocks, num_blocks);
	end = clock();
    print_elapsed("Encryption Sequential", start, end);	
    s_stats[STAT_ENCRYPT] = get_seconds(end - start);
    //printf("Encrypted: %s\n", encrypted);
    
    free(cipher_blocks);
    free(encrypted);
 
    // parallel
    start = clock();
    cipher_blocks = p_rsa(ENCRYPT, blocks, num_blocks, e, n);
    encrypted = int_to_msg(cipher_blocks, num_blocks);
    end = clock();
    print_elapsed("Encryption Parallel", start, end);	
    p_stats[STAT_ENCRYPT] = get_seconds(end - start);
    //printf("Encrypted: %s\n", encrypted);

	/* Let's decrypt! */
	
    // sequential
    uint64_t* plaintext_blocks = rsa(DECRYPT, cipher_blocks, num_blocks, d, n);
	//print_blocks(plaintext_blocks, num_blocks); // each block must be encrypted with rsa
	char* decrypted = int_to_msg(plaintext_blocks, num_blocks);
    printf("Sequential decryption correct %i\n", is_correct(message, decrypted, strlen(message)));	
    //printf("Decrypted: %s\n", decrypted);

    free(plaintext_blocks);
    free(decrypted);

    // parallel
    plaintext_blocks = p_rsa(DECRYPT, cipher_blocks, num_blocks, d, n);
    decrypted = int_to_msg(plaintext_blocks, num_blocks);
    printf("Parallel decryption correct %i\n", is_correct(message, decrypted, strlen(message)));

	// clean up when done
	mpz_clear(p);
	mpz_clear(q);
	mpz_clear(pq);
	mpz_clear(d);
	mpz_clear(e);

	free(blocks);
	free(cipher_blocks);

    // print statistics
    printf("%64s %16s %16s %16s\n", "stat", "sequential", "parallel", "faster");
    printf("%64s %16s %16s %16s\n", "---", "---", "---", "---");
    for(int i=0; i<NUM_STATS; i++) {
        char faster = (s_stats[i] < p_stats[i]) ? 's' : 'p';
        printf("%64s %16.5f %16.5f %16c\n", all_stats[i], s_stats[i], p_stats[i], faster);
    }

	return 0;
}
