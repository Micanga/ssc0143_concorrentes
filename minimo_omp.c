#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

#define N 1000		// numero de iteracoes
#define N_THREADS 10

int A[N];


int main(int argc, char **argv){
	int i, minimum = 999999;
	
	// 1. Generating the numbers
	// a. parallel generating
    #pragma omp parallel private (i) num_threads(N_THREADS)
    {
		for (i = 0; i < N; i++) {
		    //A[i] = (N - 1) - i;
			A[i] = rand() % 100;
		}
	}

	// b. printing result
	for(i = 0; i < N ; i++)
		printf("%d ",A[i]);
	printf("\n");

	// 2. Searching the minimum number
	#pragma omp parallel num_threads(N_THREADS) reduction(min:minimum)
	{
		minimum = A[0]; 
		for (i = 1; i< N; i++) 
		    if (A[i] < minimum) 
				minimum = A[i]; 
	}
	
	printf("min = %d, N = %d SEQ \n", minimum, N);
	return 0;
}
