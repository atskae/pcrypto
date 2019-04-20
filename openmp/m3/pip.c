/*
	05 Module 3	
	Integral from 0 to 1 of: 4.0/(1+x^2) = pi
	- parallel version of pi.c
*/
#include <stdio.h>
#include <omp.h>

// a step is a rectangle under the curve
#define NUM_STEPS 100000
#define NUM_THREADS 4
#define STEPS_PER_THREAD (NUM_STEPS/NUM_THREADS)

double sums[NUM_THREADS] = {0.0}; // each thread will compute and place their results in a separate spot in array
double step;	

int main() {
	int i;
	double pi, sum=0.0;

	step = 1.0/(double)NUM_STEPS;

	omp_set_num_threads(NUM_THREADS);	
	double start = omp_get_wtime();
    #pragma omp parallel
	{	
		int id = omp_get_thread_num();	
		double x, tsum=0.0;

		int start = id * STEPS_PER_THREAD;
		for(int i=start; i<start+STEPS_PER_THREAD; i++) {
			x = (i+0.5) * step;
			tsum = tsum + 4.0/(1.0 + x * x);
		}
		sums[id] = tsum;	
		printf("t%i: i=%i to %i, sum=%f\n", id, start, start+STEPS_PER_THREAD, sums[id]);
	}
	for(i=0; i<NUM_THREADS; i++) {
		sum += sums[i];
	}
	pi = step * sum;	
    double elapsed = omp_get_wtime() - start; 
	printf("Result: %f (%.5f sec)\n", pi, elapsed);
	
	return 0;
}
