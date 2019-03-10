#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <stdlib.h>

#include "utils.h"

// this assumes that intStr has room for num_digit digits plus the null terminator... 
void get_rand_intStr(char* intStr, int num_digits) {
	for(int i=0; i<num_digits; i++) {
		// (rand() % (upper - lower + 1)) + lower
		intStr[i] = (char)(rand() % (ASCII_9 - ASCII_0 + 1)) + ASCII_0;	
	}
	intStr[num_digits] = '\0';
}

void print_elapsed(char* msg, clock_t start, clock_t stop) {
	printf("%s: %.2f sec\n", msg, ( (double)(stop-start) ) / CLOCKS_PER_SEC); 
}

void print_bits8(char n) {
	printf("%c (%-3i) = ", n, n);
	for(int i=7; i>=0; i--) {
		if(n & (1 << i)) printf("1");
		else printf("0");
		if(i % 4 == 0) printf(" ");
	}
	printf("\n");
}

void print_bits32(uint32_t n) {
	//printf("%u = ", n);
	printf("| ");
	for(int i=31; i>=0; i--) {
		if(n & (1 << i)) printf("1");
		else printf("0");
		if(i % 4 == 0) printf(" ");
		if(i % 8 == 0) printf("| ");
	}
	printf("\n");
}

void print_blocks(uint32_t* blocks, int num_blocks) {
	for(int i=0; i<num_blocks; i++) {
		printf("Block %i (%u): ", i, blocks[i]);
		print_bits32(blocks[i]);
	}
}
