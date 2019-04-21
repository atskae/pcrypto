#include <stdio.h>
#include <stdlib.h> // rand()
#include <time.h> // time()
#include <stdint.h>
#include <string.h>
#include <gmp.h>

#include "rsa.h"
#include "utils.h"

/* For running through valgrind ; so dang slow though... */

int main(int argc, char* argv[]) {
    
    char* text_file = "text_files/test.txt";
    char* message = get_message(text_file);
    if(!message) {
        printf("Failed to retrieve message from %s\n", text_file);
        return 1;
    }  
    printf("%s\n", message);
	
    // generate two primes, p and q
	mpz_t p;
	mpz_init(p);	
	
	mpz_t q;
	mpz_init(q);	

    // sequential    
    get_p_q(p, q);

    // parallel
    p_get_p_q(p, q);

	//mpz_t pq;
	//mpz_init(pq);
	//mpz_mul(pq, p, q);

	//// convert message to integer blocks
	//int num_blocks;
	//uint64_t* blocks;

    //// sequential
    //start = clock();
    //blocks = msg_to_int(message, &num_blocks);
    //end = clock();
    //s_stats[STAT_MSG_TO_INT] = get_seconds(end - start);

    //free(blocks);

    //// parallel
    //start = clock();
    //blocks = p_msg_to_int(message, &num_blocks);
    //end = clock();
    //p_stats[STAT_MSG_TO_INT] = get_seconds(end - start);

    //printf("%i message blocks (%lu bytes)\n", num_blocks, strlen(message));

	//// the values of the blocks must be less than (p*q - 1)
	//for(int i=0; i<num_blocks; i++) {
	//	uint64_t block = blocks[i];
	//	int result = mpz_cmp_ui(pq, block);
	//	if(result < 0) { // block values are larger than p*q
	//		printf("%i digits for random prime is insufficient.\n", NUM_DIGITS_P);
	//		return 0;
	//	}
	//}

	//mpz_t d;
	//mpz_init(d);
	//get_d(d, p, q);

	//mpz_t e;
	//mpz_init(e);
	//get_e(e, p, q, d);
	//
	//// n = p*q
	//mpz_t n;
	//mpz_init(n);
	//mpz_set(n, p);
	//mpz_mul(n, n, q);

	//// n must be less tha 2^64, since 64-bit blocks are used
	//int result = mpz_cmp_ui(n, UINT64_MAX);
	//if(result > 0) {
	//	printf("p*q is too large. Must be less than 2^64 (block size)\n");
	//	return 0;
	//}

	///* Let's encrypt! */
    //uint64_t* cipher_blocks;
    //char* encrypted;
	//
    //// sequential
    //start = clock();
    //cipher_blocks = rsa(ENCRYPT, blocks, num_blocks, e, n);
	//encrypted = int_to_msg(cipher_blocks, num_blocks);
	//end = clock();
    //s_stats[STAT_ENCRYPT] = get_seconds(end - start);
    //
    //free(cipher_blocks);
    //free(encrypted);
 
    //// parallel
    //start = clock();
    //cipher_blocks = p_rsa(ENCRYPT, blocks, num_blocks, e, n);
    //encrypted = int_to_msg(cipher_blocks, num_blocks);
    //end = clock();
    //p_stats[STAT_ENCRYPT] = get_seconds(end - start);

	///* Let's decrypt! */
    //uint64_t* plaintext_blocks;
    //char* decrypted;
	//
    //// sequential
    //start = clock();
    //plaintext_blocks = rsa(DECRYPT, cipher_blocks, num_blocks, d, n);
	//decrypted = int_to_msg(plaintext_blocks, num_blocks);
    //end = clock();
    //printf("Sequential decryption correct %i\n", is_correct(message, decrypted, strlen(message)));	
    //s_stats[STAT_DECRYPT] = get_seconds(end - start);

    //free(plaintext_blocks);
    //free(decrypted);

    //// parallel
    //start = clock();
    //plaintext_blocks = p_rsa(DECRYPT, cipher_blocks, num_blocks, d, n);
    //decrypted = int_to_msg(plaintext_blocks, num_blocks);
    //end = clock();
    //printf("Parallel decryption correct %i\n", is_correct(message, decrypted, strlen(message)));
    //p_stats[STAT_DECRYPT] = get_seconds(end - start);

	// clean up when done
	free(message);
    mpz_clear(p);
	mpz_clear(q);
	//mpz_clear(pq);
	//mpz_clear(d);
	//mpz_clear(e);

	//free(blocks);
	//free(cipher_blocks);

    //print_stats(s_stats, p_stats);

	return 0;
}
