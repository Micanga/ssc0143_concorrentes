#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define N 1000		// numero de iteracoes
#define N_THREADS 8

int A[N], min, seq[N_THREADS];
pthread_t thread[N_THREADS];
pthread_mutex_t mutex;

void *search_func(void *args){
	int i = ((int *) args)[0];
	int start_end[2] = {(int) (N/N_THREADS)*i,(int) (N/N_THREADS)*(i+1)};

	for(i = start_end[0]; i < start_end[1]; i++){
		pthread_mutex_lock(&mutex);
	    if (A[i] < min) 
			min = A[i]; 
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

void *numgen_func(void *args){
	int i = ((int *) args)[0];
	int start_end[2] = {(int) (N/N_THREADS)*i,(int) (N/N_THREADS)*(i+1)};

	for(i = start_end[0]; i < start_end[1]; i++){
	    A[i] = (N - 1) - i;
	    //A[i] = rand() % 10000;
	}
	return NULL;
}

void create_mutex(){
	if(pthread_mutex_init(&mutex, NULL) != 0){
		printf("::mutex creation error ::\n");
		exit(1);
	}
	return;
}

int main(int argc, char **argv) {
	int i;
	create_mutex();
	min = 999999;

	// 1. Generating the numbers
	// a. creating threads
	for(i = 0; i < N_THREADS ; i++){
		seq[i] = i;
		if(pthread_create(&(thread[i]),NULL,numgen_func,(void *) &(seq[i])) != 0){
			printf("::numgen thread creation error ::\n");
			exit(1);
		}
	}

	// b. waiting the process
	for(i = 0; i < N_THREADS ; i++){
		pthread_join(thread[i],NULL);
	}

	// c. printing result
	for(i = 0; i < N ; i++)
		printf("%d ",A[i]);
	printf("\n");

	// 2. Searching the minimum value
	// a. creating threads
	for(i = 0; i < N_THREADS ; i++){
		if(pthread_create(&(thread[i]),NULL,search_func,(void *) &(seq[i])) != 0){
			printf("::search thread creation error ::\n");
			exit(1);
		}
	}
	
	// b. waiting the process
	for(i = 0; i < N_THREADS ; i++){
		pthread_join(thread[i],NULL);
	}

	// 3. Printing result
	printf("min = %d, N = %d SEQ \n", min, N);

	return 0;
}
