/*
	05 Module 5	
	Integral from 0 to 1 of: 4.0/(1+x^2) = pi
    - uses a loop construct to parallelize
*/
#include <stdio.h>
#include <omp.h>

// a step is a rectangle under the curve
static long num_steps = 100000;
double step;	

int main() {
	int i;
	double pi, sum=0.0;

	step = 1.0/(double)num_steps;
	#pragma omp parallel
    {
        double x;
        #pragma omp for reduction (+:sum)
        for(i=0; i<num_steps; i++) {
	    	//printf("%i!\n", omp_get_thread_num());
            x = (i+0.5) * step;
	    	sum = sum + 4.0/(1.0 + x * x);
	    }
    }
	pi = step * sum;
	printf("Result: %f\n", pi);
	
	return 0;
}
