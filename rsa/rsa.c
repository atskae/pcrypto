#include <stdio.h>
#include <stdlib.h> // srand(), rand()
#include <stdint.h>
#include <time.h> // time()
#include <math.h>

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
uint64_t get_rand_prime() {
	
	srand(time(NULL));
	
	uint64_t p = 0;
	char done = 0;
	while(!done) {
		p = rand() + pow(10, 99); // 1 digit + 99 digits = 100 digits
		if(p % 2 == 0) p++; // obtain an odd number
		
		// Check if p is prime (Solovay and Strassen algorithm)
		uint64_t b = rand() % p;	
	}
		
	return p;	
}
