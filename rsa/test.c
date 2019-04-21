/* Tests Sequential and Parallel RSA */

#include <stdio.h>
#include <stdlib.h> // rand()
#include <time.h> // time()
#include <stdint.h>
#include <string.h>
#include <gmp.h>

#include "rsa.h"
#include "utils.h"

// number of times to run each experiment
#define NUM_TRIALS_PQ 5
#define NUM_TRIALS 25

// index into stats 2D array ; S=sequential ; P=parallel
#define S 0
#define P 1

int main(int argc, char* argv[]) {

    if(argc < 2) {
        printf("./test <message.txt>\n");
        return 1;
    }
    
    char* message = get_message(argv[1]);
    if(!message) {
        printf("Failed to retrieve message from %s\n", argv[1]);
        return 1;
    }  

    printf("Running each experiment for %i trials\n", NUM_TRIALS); 
    double stats[2][NUM_STATS] = { {-1} };
    double s_avr = 0.0;
    double p_avr = 0.0;
    clock_t start, end;
    	
    // generate two primes, p and q
	mpz_t p;
	mpz_init(p);	
	
	mpz_t q;
	mpz_init(q);	

    /*
        Testing get_p_q()
    */
    s_avr = 0.0;
    p_avr = 0.0;
    for(int i=0; i<NUM_TRIALS_PQ; i++) {    
        printf("%i: get_p_q()\n", i);
        // sequential    
	    start = clock();
        get_p_q(p, q);
	    end = clock();
        s_avr += get_seconds(end - start);  
        printf("Finished sequential %i\n", i);

        // parallel
        start = clock();
        p_get_p_q(p, q);
        end = clock();
        p_avr += get_seconds(end - start); 
    }
    stats[S][STAT_GET_PQ] = s_avr/NUM_TRIALS;
    stats[P][STAT_GET_PQ] = p_avr/NUM_TRIALS;
    printf("Completed runs of get_p_q()\n");

	mpz_t pq;
	mpz_init(pq);
	mpz_mul(pq, p, q);

	/*
        Testing convert message to integer blocks
    */
	int num_blocks;
	uint64_t* blocks;
    
    s_avr = 0.0;
    p_avr = 0.0;
    for(int i=0; i<NUM_TRIALS; i++) {    
        // sequential    
	    start = clock(); 
        blocks = msg_to_int(message, &num_blocks);
	    end = clock();
        s_avr += get_seconds(end - start); 

        free(blocks);

        // parallel
        start = clock();
        blocks = p_msg_to_int(message, &num_blocks);
        end = clock();
        p_avr += get_seconds(end - start);
        
        if(i != NUM_TRIALS-1) free(blocks);
    }
    stats[S][STAT_MSG_TO_INT] = s_avr/NUM_TRIALS;
    stats[P][STAT_MSG_TO_INT] = p_avr/NUM_TRIALS;
    printf("Completed runs of msg_to_int\n");

    printf("%i message blocks (%lu bytes)\n", num_blocks, strlen(message));

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

	/* 
        Testing encrypt
    */
    uint64_t* cipher_blocks;
    char* encrypted;

    s_avr = 0.0;
    p_avr = 0.0;
    for(int i=0; i<NUM_TRIALS; i++) {	
        // sequential
        start = clock();
        cipher_blocks = rsa(ENCRYPT, blocks, num_blocks, e, n);
	    encrypted = int_to_msg(cipher_blocks, num_blocks);
	    end = clock();
        s_avr += get_seconds(end - start);
        
        free(cipher_blocks);
        free(encrypted);
 
        // parallel
        start = clock();
        cipher_blocks = p_rsa(ENCRYPT, blocks, num_blocks, e, n);
        encrypted = int_to_msg(cipher_blocks, num_blocks);
        end = clock();
        p_avr += get_seconds(end - start);

        if(i != NUM_TRIALS-1) {
            free(cipher_blocks);
            free(encrypted);
        }
    }
    stats[S][STAT_ENCRYPT] = s_avr/NUM_TRIALS;
    stats[P][STAT_ENCRYPT] = p_avr/NUM_TRIALS;
    printf("Completed runs of encrypt\n");

	/*
        Testing decrypt
    */
    uint64_t* plaintext_blocks;
    char* decrypted;
    
    s_avr = 0.0;
    p_avr = 0.0;
    for(int i=0; i<NUM_TRIALS; i++) {	
        // sequential
        start = clock();
        plaintext_blocks = rsa(DECRYPT, cipher_blocks, num_blocks, d, n);
	    decrypted = int_to_msg(plaintext_blocks, num_blocks);
        end = clock();
        s_avr += get_seconds(end - start);
        if(!is_correct(message, decrypted, strlen(message))) {
            printf("Decryption failed for sequential\n");
            return 1;
        }

        free(plaintext_blocks);
        free(decrypted);

        // parallel
        start = clock();
        plaintext_blocks = p_rsa(DECRYPT, cipher_blocks, num_blocks, d, n);
        decrypted = int_to_msg(plaintext_blocks, num_blocks);
        end = clock();
        p_avr += get_seconds(end - start);
        if(!is_correct(message, decrypted, strlen(message))) {
            printf("Decryption failed for parallel\n");
            return 1;
        }
    
        free(plaintext_blocks);
        free(decrypted);
    }
    stats[S][STAT_DECRYPT] = s_avr/NUM_TRIALS;
    stats[P][STAT_DECRYPT] = p_avr/NUM_TRIALS;
    printf("Completed runs of decrypt\n");

	// clean up when done
	mpz_clear(p);
	mpz_clear(q);
	mpz_clear(pq);
	mpz_clear(d);
	mpz_clear(e);

	free(blocks);
	free(cipher_blocks);

    print_stats(stats[S], stats[P]);

	return 0;
}
