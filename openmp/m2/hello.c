#include <stdio.h>
#include <omp.h> // openMP

int main() {
	# pragma omp parallel
	{
		int id = omp_get_thread_num();	
		printf("Hallo Welt!\n %d", id);
	}
	return 0;
}
