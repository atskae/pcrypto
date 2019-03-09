/*
	Practicing GMP Library
	https://gmplib.org/

	Tutorials
	- https://www.cs.colorado.edu/~srirams/courses/csci2824-spr14/gmpTutorial.html
	- http://web.mit.edu/gnu/doc/html/gmp_4.html
*/

#include <stdio.h>
#include <stdlib.h> // rand()
#include <time.h> // time()
#include <assert.h>
#include <gmp.h>

#define ASCII_0 48
#define ASCII_9 57

#define NUM_DIGITS 100

int main(int argc, char* argv[]) {
	
	// generate a random number
	char intStr[NUM_DIGITS + 1]; // +1 null terminator
	for(int i=0; i<NUM_DIGITS; i++) {
		// (rand() % (upper - lower + 1)) + lower
		intStr[i] = (char)(rand() % (ASCII_9 - ASCII_0 + 1)) + ASCII_0;	
	}
	intStr[NUM_DIGITS] = '\0';

	// mpz_t is a type defined by GMP
	mpz_t n;

	// initialize number with limb space and sets value to 0
	// "limb": amount of data about the multi-precision number that fits
	// in a word
	mpz_init(n);
	mpz_set_ui(n, 0); // sets value to 0 (redundant?)

	// convert intStr to base 10 number
	int flag = mpz_set_str(n, intStr, 10);
	assert(flag == 0); // if flag is not zero, failed

	printf("n= ");
	mpz_out_str(stdout, 10, n);
	printf("\n");

	// let's do some math on this number!
	mpz_add_ui(n, n, 1); // n = n + 1
	printf("n+1= ");
	mpz_out_str(stdout, 10, n);
	printf("\n");

	mpz_mul(n, n, n); // n = n*n
	printf("n*n= ");
	mpz_out_str(stdout, 10, n);
	printf("\n");

	// must always clean up after done
	// will cause memory leaks if not cleaned up
	mpz_clear(n);

	/*
		More notes
	
		- mpz_t is a pointer ; a "handle" to the int
	*/

	return 0;
}
