#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define N 1000000		// numero de iteracoes
#define N_THREADS 10

double sum = 0.0;

int main(void) {
	double factor = 1.0;
	long long i;
	
	#pragma omp parallel for private(i,factor) num_threads(N_THREADS) reduction(+:sum)
	for (i = 0; i < N; i++) {
		factor = (double) pow(-1,i);
		sum = sum + (factor / (2 * i + 1));
	}

	sum = 4 * sum;
	printf("pi = %.15f, N = %d  SEQ \n", sum, N);

	return 0;
}
