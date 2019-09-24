#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>

#define N 10000000		// numero de iteracoes
#define N_THREADS 10

int seq[N_THREADS];
double sum = 0.0;
pthread_t thread[N_THREADS];
pthread_mutex_t mutex;

void *pi_calc(void *args){
	long long i = ((int *) args)[0];
	double l_sum = 0.0, factor = (double) pow((-1),i);
	long long start_end[2] = {(long long) (N/N_THREADS)*i,(long long) (N/N_THREADS)*(i+1)};

	for (i = start_end[0]; i < start_end[1]; i++, factor = -factor){
		l_sum += factor / (2 * i + 1);
	}

	pthread_mutex_lock(&mutex);
	sum += l_sum;
	pthread_mutex_unlock(&mutex);
	return NULL;
}

void create_mutex(){
	if(pthread_mutex_init(&mutex, NULL) != 0){
		printf("::mutex creation error ::\n");
		exit(1);
	}
	return;
}

int main(void) {
	long long i;
	create_mutex();

	// 1. Calculating Maclaurin series Pi value
	// a. creating threads
	for(i = 0; i < N_THREADS ; i++){
		seq[i] = i;
		if(pthread_create(&(thread[i]),NULL,pi_calc,(void *) &(seq[i])) != 0){
			printf("::thread creation error ::\n");
			exit(1);
		}
	}

	// b. waiting the process
	for(i = 0; i < N_THREADS ; i++){
		pthread_join(thread[i],NULL);
	}

	// 2. Calculating Pi value and printing
	sum = 4 * sum;
	printf("pi = %.15f, N = %d  SEQ \n", sum, N);

	return 0;
}
