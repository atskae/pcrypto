/*
	05 Module 3	
	Integral from 0 to 1 of: 4.0/(1+x^2) = pi
*/
#include <stdio.h>

// a step is a rectangle under the curve
static long num_steps = 100000;
double step;	

int main() {
	int i;
	double x, pi, sum=0.0;

	step = 1.0/(double)num_steps;
	for(i=0; i<num_steps; i++) {
		x = (i+0.5) * step;
		sum = sum + 4.0/(1.0 + x * x);
	}
	pi = step * sum;
	printf("Result: %f\n", pi);
	
	return 0;
}
