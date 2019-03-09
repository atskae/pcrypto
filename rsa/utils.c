#include <stdio.h>
#include <time.h>

#include "utils.h"

void print_elapsed(char* msg, clock_t start, clock_t stop) {
	printf("%s: %.2f sec\n", msg, ( (double)(stop-start) ) / CLOCKS_PER_SEC); 
}
