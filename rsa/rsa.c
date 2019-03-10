#include <stdio.h>
#include <stdlib.h> // srand(), rand()
#include <stdint.h>
#include <time.h> // time()
#include <math.h>
#include <assert.h>
#include <string.h>
#include <gmp.h>

#include "rsa.h"
#include "utils.h"

#define DEBUG 0

// e is the multiplicative inverse of d mod (p-1)(q-1)
void get_e(mpz_t e, mpz_t p, mpz_t q, mpz_t d) {
	
	// calculate (p-1) * (q-1)
	
	// (p-1)
	mpz_t p1;
	mpz_init(p1);
	mpz_set(p1, p);
	mpz_sub_ui(p1, p1, 1);
	// (q-1)
	mpz_t q1;
	mpz_init(q1);
	mpz_set(q1, q);
	mpz_sub_ui(q1, q1, 1);
	// (p-1) * (q-1)
	mpz_t p1_q1;
	mpz_init(p1_q1);
	mpz_mul(p1_q1, p1, q1);

	int flag = mpz_invert(e, d, p1_q1);
	if(flag == 0) printf("Multiplicative inverse does not exist for given d.\n");
	//else { // valid inverse ; check if correct
	//
	//	// (e * d) mod (p-1)(q-1)
	//	mpz_t ed;
	//	mpz_init(ed);
	//	
	//	mpz_set(ed, e);
	//	mpz_mul(ed, ed, d);
	//	mpz_mod(ed, ed, p1_q1);

	//	int result = mpz_cmp_ui(ed, 1);
	//	assert(result == 0); 

	//	mpz_clear(ed);
	//	
	//}

	// clean up
	mpz_clear(p1);
	mpz_clear(q1);	
	mpz_clear(p1_q1);

}

// d is a random large integer which is relatively prime to (p-1)(q-1) ; gcd(d, (p-1)(q-1)) = 1
void get_d(mpz_t d, mpz_t p, mpz_t q) {

	// calculate (p-1) * (q-1)
	
	// (p-1)
	mpz_t p1;
	mpz_init(p1);
	mpz_set(p1, p);
	mpz_sub_ui(p1, p1, 1);
	// (q-1)
	mpz_t q1;
	mpz_init(q1);
	mpz_set(q1, q);
	mpz_sub_ui(q1, q1, 1);
	// (p-1) * (q-1)
	mpz_t p1_q1;
	mpz_init(p1_q1);
	mpz_mul(p1_q1, p1, q1);

	mpz_print("(p-1)(q-1)", p1_q1);
	
	while(1) {

		mpz_set_ui(d, 0); // reset value to 0

		char intStr[NUM_DIGITS_P + 1]; // +1 for null terminator
		get_rand_intStr(intStr, NUM_DIGITS_P);
		
		int flag = mpz_set_str(d, intStr, 10);
		assert(flag == 0); // non-zero = fail	
		
		mpz_t gcd;
		mpz_init(gcd);

		// calculate the gcd
		mpz_gcd(gcd, d, p1_q1);	
		int result = mpz_cmp_ui(gcd, 1);
		
		// clean up
		mpz_clear(gcd);

		if(result != 0) continue; // the gcd does not equal 1
		else break;
	}

	// clean up
	mpz_clear(p1);
	mpz_clear(q1);	
	mpz_clear(p1_q1);
}

// each block is contained in a uint32_t integer
uint32_t* msg_to_int(char* msg, int* num_blocks) {
	
	printf("msg = %s\n", msg);
	//if(DEBUG) {
		for(int i=0; i<strlen(msg); i++) {
			print_bits8(msg[i]);
		}
	//}

	int num_chars = strlen(msg);
	*num_blocks = ceil((float)num_chars/4);
	if(DEBUG) printf("num_blocks = %i, num_chars %i\n", (*num_blocks), num_chars);

	uint32_t* blocks = (uint32_t*) malloc((*num_blocks) * sizeof(uint32_t));

	for(int i=0; i<(*num_blocks); i++) {
		uint32_t val = 0;
		for(int c=3; c>=0; c--) { // block = ['S', 'a', 'm', '!' ], the first bit starts at the last character '!'
			int idx = i*4 + c; // block_idx * 4
			if(idx >= num_chars) continue;
			for(int b=0; b<8; b++) { // for each bit
				unsigned int bit_val = msg[idx] & (1 << b);
				if(bit_val > 0) bit_val = 1;
				val |= (bit_val << ((3-c)*8 + b));
			}	
		}
		blocks[i] = val;
	}

	return blocks;
} 

void mpz_print(char* name, mpz_t n) {
	printf("%s = ", name);
	mpz_out_str(stdout, 10, n);	// print in base 10
	printf("\n");
}

// generates a 100-digit prime number, and sets it to p
void get_rand_prime(mpz_t p) {

	// initialize random number generators	
	srand(time(NULL));
	gmp_randstate_t state;
	gmp_randinit_default(state);

	// the random prime to generate	; this is set to p at the end
	mpz_t b;
	mpz_init(b);

	char intStr[NUM_DIGITS_P + 1]; // string representation of int ; +1 null-terminator
	uint64_t tries = 0;
	while(1) {
		tries++;
		if(DEBUG) printf("===Try %llu===\n", tries);
	
		mpz_set_ui(b, 0); // reset value to 0
		
		// generate random digits
		get_rand_intStr(intStr, NUM_DIGITS_P);
		
		// make integer odd
		if(intStr[NUM_DIGITS_P-1] < ASCII_9) {
			char ls_int = intStr[NUM_DIGITS_P-1]; // least significant int
			if(ls_int % 2 == 0) intStr[NUM_DIGITS_P-1]++;	
		}

		// create int
		int flag = mpz_set_str(b, intStr, 10);
		assert(flag == 0); // non-zero = fail	
		if(DEBUG) mpz_print("b", b);

		// random integer to test against n
		mpz_t a;
		mpz_init(a);	
		
		mpz_t gcd;
		mpz_init(gcd);

		mpz_t jacobi_sym;
		mpz_init(jacobi_sym); 

		// the value to compare the calculated Jacobi symbol to
		mpz_t j_ab;
		mpz_init(j_ab);
		mpz_t exp; // 
		mpz_init(exp);

		char is_prime = 1;
		// Check if b is prime (Solovay and Strassen algorithm)
		// generate random number a, 100 times ; if prime test passes for all 100 numbers, b is highly likely to be prime
		for(int i=0; i<NUM_DIGITS_P; i++) {
			mpz_urandomm(a, state, b); // generates a random number between 0-b

			if(DEBUG) {
				printf("%i) ", i);
				mpz_print("a", a);	
			}
	
			// calculate the gcd
			mpz_gcd(gcd, a, b);	
			int result = mpz_cmp_ui(gcd, 1);
			if(result != 0) { // the gcd does not equal 1
				is_prime = 0;
				break;
			}

			if(DEBUG) mpz_print("gcd of a and b", gcd);

			// calculate the Jacobi symbol J(a,b)
			int j = mpz_jacobi(a, b); // b must be odd, which is ensured above
			mpz_set_ui(jacobi_sym, j); // 1 or -1
			if(DEBUG) printf("jacobi symbol = %i\n", j);
	
			// calculate the exponent (b-1)/2 
			mpz_set(exp, b);
			mpz_sub_ui(exp, exp, 1);
			mpz_cdiv_q_ui(exp, exp, 2);

			if(DEBUG) mpz_print("exp", exp);
	
			mpz_powm(j_ab, a, exp, b); // a^{(b-1)/2} mod b
			if(DEBUG) mpz_print("a^exp mod b", j_ab);
			result = mpz_cmp(jacobi_sym, j_ab);
			if(result != 0) {
				is_prime = 0;
				break;
			}
		}
	
		if(tries % 100000 == 0) printf("%llu tries\n", tries);
		if(!is_prime) continue;
		
		// at this point, we got a prime

		// don't need these anymore ; clean up to prevent memory leaks
		mpz_clear(a);
		mpz_clear(gcd);
		mpz_clear(jacobi_sym);
		mpz_clear(j_ab);
		mpz_clear(exp);
		break;
	}
	
	// b is prime! set to p
	mpz_set(p, b);
	
	// clean up to prevent memory leaks
	mpz_clear(b);
	gmp_randclear(state);
}
