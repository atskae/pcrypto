#include <stdio.h>
#include <stdlib.h> // rand()
#include <time.h> // time()
#include <stdint.h>
#include <string.h>
#include <gmp.h>

#include "rsa.h"
#include "utils.h"

int main(int argc, char* argv[]) {

    // statistics
    double s_stats[NUM_STATS] = {-1};
    double p_stats[NUM_STATS] = {-1};
    clock_t start, end;
 
	char* message = "Pajama Sam";
	
    // generate two primes, p and q
	mpz_t p;
	mpz_init(p);	
	
	mpz_t q;
	mpz_init(q);	
    
	start = clock();
    get_p_q(p, q);
	end = clock();
    s_stats[STAT_GET_PQ] = get_seconds(end - start);

    start = clock();
    p_get_p_q(p, q);
    end = clock();
    p_stats[STAT_GET_PQ] = get_seconds(end - start);

	mpz_t pq;
	mpz_init(pq);
	mpz_mul(pq, p, q);
	//mpz_print("p*q", pq);

	// convert message to integer blocks
	int num_blocks;
	uint64_t* blocks = msg_to_int(message, &num_blocks);

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

	mpz_t e;
	mpz_init(e);
	get_e(e, p, q, d);
	
	// n = p*q
	mpz_t n;
	mpz_init(n);
	mpz_set(n, p);
	mpz_mul(n, n, q);

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
    uint64_t* plaintext_blocks;
    char* decrypted;
	
    // sequential
    start = clock();
    plaintext_blocks = rsa(DECRYPT, cipher_blocks, num_blocks, d, n);
	decrypted = int_to_msg(plaintext_blocks, num_blocks);
    end = clock();
    printf("Sequential decryption correct %i\n", is_correct(message, decrypted, strlen(message)));	
    s_stats[STAT_DECRYPT] = get_seconds(end - start);

    free(plaintext_blocks);
    free(decrypted);

    // parallel
    start = clock();
    plaintext_blocks = p_rsa(DECRYPT, cipher_blocks, num_blocks, d, n);
    decrypted = int_to_msg(plaintext_blocks, num_blocks);
    end = clock();
    printf("Parallel decryption correct %i\n", is_correct(message, decrypted, strlen(message)));
    p_stats[STAT_DECRYPT] = get_seconds(end - start);

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
    printf("%64s %16s %16s %16s\n", "-----", "-----", "-----", "-----");
    
    double s_total = 0.0;
    double p_total = 0.0;
    
    for(int i=0; i<NUM_STATS; i++) {
        double s = s_stats[i];
        double p = p_stats[i];

        char faster = (s < p) ? 's' : 'p';
        printf("%64s %16.8f %16.8f %16c\n", all_stats[i], s, p, faster);
    
        s_total += s;
        p_total += p;
    }
    printf("%64s\n", "-----");
    printf("%64s %16.8f %16.8f %16c\n", "OVERALL", s_total, p_total, (s_total < p_total) ? 's' : 'p');

	return 0;
}
