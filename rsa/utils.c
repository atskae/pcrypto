#include <stdio.h>
#include <time.h>
#include <stdint.h>

#include "utils.h"

void print_elapsed(char* msg, clock_t start, clock_t stop) {
	printf("%s: %.2f sec\n", msg, ( (double)(stop-start) ) / CLOCKS_PER_SEC); 
}

void print_bits8(char n) {
	printf("%c (%i) = ", n, n);
	for(int i=7; i>=0; i--) {
		if(n & (1 << i)) printf("1");
		else printf("0");
		if(i % 4 == 0) printf(" ");
	}
	printf("\n");
}

void print_bits32(uint32_t n) {
	printf("%i = ", n);
	for(int i=31; i>=0; i--) {
		if(n & (1 << i)) printf("1");
		else printf("0");
		if(i % 4 == 0) printf(" ");
	}
	printf("\n");
}
