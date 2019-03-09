#include <stdio.h>
#include <stdlib.h> // srand(), rand()
#include <stdint.h>
#include <time.h> // time()
#include <math.h>
#include <gmp.h>

#include "rsa.h"

// gets the greatest common divisor of x and y (Euclidean algorthm)
int gcd(int x, int y) {
	
	// set a to the larger value between x and y
	int a = (x > y) ? x : y;
	int b = (x > y) ? y : x;

	if(a == 0) return b;
	if(b == 0) return a;

	return gcd(b, a % b);

}

// generates a 100-digit prime number
mpz_t get_rand_prime() {
	
	srand(time(NULL));
	
	mpz_t p;
	mpz_init(p);

	char done = 0;
	char intStr[NUM_DIGITS_PQ];
	while(!done) {
		for(int i=0; i<NUM_DIGITS_PQ; i++) {
			intStr[i] = rand() % (57 - 48 + 1) + 48; // generates a random ascii-digit			
		}
		// make integer odd
		if(intStr[NUM_DIGITS_PQ-1] % 2 == 0) intStr[NUM_DIGITS_PQ]++;
	
		// Check if p is prime (Solovay and Strassen algorithm)
	}
		
	return p;	
}
