#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t s1, s2, s3;

void *procA (){

	int i, nProc = 0;

	printf("Processo %d iniciou.\n", nProc+1);
	for (i = nProc*200+1; i <= (nProc+1)*200; i++){
		//printf("%d ", i);
	}
	sem_post(&s1);
	printf("\nProcesso %d terminou.\n", nProc+1);
}

void *procB (){

	int i, nProc = 1;

	printf("Processo %d iniciou.\n", nProc+1);
	for (i = nProc*200+1; i <= (nProc+1)*200; i++){
		//printf("%d ", i);
	}
	printf("\nProcesso %d terminou.\n", nProc+1);
	sem_post(&s2);
}

void *procC (){

	int i, nProc = 2;

	sem_wait(&s1);
	sem_wait(&s2);
	printf("Processo %d iniciou.\n", nProc+1);
	for (i = nProc*200+1; i <= (nProc+1)*200; i++){
		//printf("%d ", i);
	}
	printf("\nProcesso %d terminou.\n", nProc+1);
	sem_post(&s3);
	sem_post(&s3);
}

void *procD (){

	int i, nProc = 3;

	sem_wait(&s3);
	printf("Processo %d iniciou.\n", nProc+1);
	for (i = nProc*200+1; i <= (nProc+1)*200; i++){
		//printf("%d ", i);
	}
	printf("\nProcesso %d terminou.\n", nProc+1);
}

void *procE (){

	int i, nProc = 4;

	sem_wait(&s3);
	printf("Processo %d iniciou.\n", nProc+1);
	for (i = nProc*200+1; i <= (nProc+1)*200; i++){
		//printf("%d ", i);
	}
	printf("\nProcesso %d terminou.\n", nProc+1);
}

int main (){

	pthread_t threads[5];

	sem_init(&s1, 0, 0);
	sem_init(&s2, 0, 0);
	sem_init(&s3, 0, 0);

	pthread_create (&threads[0], NULL, procA, NULL);
	pthread_create (&threads[1], NULL, procB, NULL);
	pthread_create (&threads[2], NULL, procC, NULL);
	pthread_create (&threads[3], NULL, procD, NULL);
	pthread_create (&threads[4], NULL, procE, NULL);

	pthread_exit(NULL);

	return 0;
}