#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

unsigned int bufferPos;
char *buffer;
sem_t mutex;

void * threadFunction(unsigned int threadId){

	sem_wait(&mutex);
	printf("threadId: %lu\n", threadId-48);

	buffer[bufferPos] = (char)threadId;
	bufferPos++;

	sem_post(&mutex);
}

void printBuffer(unsigned int size){

	unsigned int i;
	char *ptr;

	//for (i = 0; i < size; i++)
	printf("Printing buffer...\n");
}

int main (int argc, char *argv[]){

	unsigned int i, nThreads, bufferSize, priority;
	char *pol;

	/*if (argc > 1){

		nThreads = (unsigned int)argv[1] - 48;
		bufferSize = (unsigned int)argv[2] - 48;
		pol = argv[3];
		priority = (unsigned int)argv[4] - 48;
		printf("nThreads: %lu\nbufferSize: %lu\npriority: %lu\npol: %s\n", nThreads, bufferSize, priority, pol);
	}*/


	nThreads = 2;
	bufferSize = 1;

	buffer = malloc(sizeof(char) * 1024 * bufferSize);
	bufferPos = 0;

	sem_init(&mutex, 0, 1);

	pthread_t threads[nThreads];
	for (i = 0; i < nThreads; i++)
		pthread_create(&threads[i], NULL, threadFunction, (i+48));

	pthread_join(threads[nThreads-1]); // Arrumar, qual thread devemos esperar

	printBuffer(bufferSize);
	pthread_exit(NULL);
	return 0;
}
