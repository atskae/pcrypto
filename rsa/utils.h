#ifndef UTILS_H
#define UTILS_H

#include <time.h>

void print_elapsed(char* msg, clock_t start, clock_t stop);
void print_bits8(char n);
void print_bits32(uint32_t n);

void print_blocks(uint32_t* blocks, int num_blocks);

#endif // UTILS_H
