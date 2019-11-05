#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <linux/sched.h>
#include <string.h>

unsigned int nThreads, bufferSize, priority, howMany;
char *buffer, *initBufferPos, *bufferPos, *policy;
sem_t mutex, toPrint, threadSync;

void * threadFunction(unsigned int threadId);
void printBuffer();
void print_sched(int policy);
int setpriority(pthread_t *thr, int newpolicy, int newpriority);
int getPolicyCode(char* policy);

int main (int argc, char *argv[]){

	unsigned int i;
	int policyCode;

	if (argc == 5){

		nThreads = atoi(argv[1]);
        bufferSize = atoi(argv[2]);
        policy = argv[3];
        priority = atoi(argv[4]);

        printf("nThreads: %u\n", nThreads);
        printf("bufferSize: %u\n", bufferSize);
        printf("policy: %s\n", policy);
        printf("priority: %u\n", priority);

		howMany = 0;
		buffer = malloc(sizeof(char) * 1024 * bufferSize); // Aloca o buffer
		bufferPos = initBufferPos = &buffer; // Guarda posicao inicial do buffer

		printf("%lu\n", &buffer);
		//printf("%lu\n", );

		// Inicializa semaforos
		sem_init(&mutex, 0, 1);
		sem_init(&toPrint, 0, 0);
		sem_init(&threadSync, 0, 0);

		// Define codigo da politica
		policyCode = getPolicyCode(policy);

		// Cria as threads
		pthread_t threads[nThreads];
		for (i = 0; i < nThreads; i++)
			pthread_create(&threads[i], NULL, threadFunction, i);

		// Define prioridade de cada thread
		for (i = 0; i < nThreads; i++)
			setpriority(&threads[i], policyCode, priority);

		printf("Definiu a prioridade de cada thread...\n");

		// Libera threads para execucao
		for (i = 0; i < nThreads; i++)
			 sem_post(&threadSync);

		// Imprime buffer ja resumido
		printBuffer();
	}

	// Encerra main, mantem demais threads em execucao
	pthread_exit(NULL);
	return 0;
}

void * threadFunction(unsigned int threadId){

	unsigned int i;

	sem_wait(&threadSync);
	printf("Running %c\n", (char)(threadId+65));

	//for (i = 0; i < 10; i++){

		sem_wait(&mutex);
		*bufferPos = (char)(threadId+65);
		bufferPos++;
		*bufferPos = (char)(threadId+65);
		bufferPos++;
		*bufferPos = (char)(threadId+65);
		bufferPos++;

		sem_post(&mutex);
	//}

	howMany += 1;
	if (howMany == nThreads) sem_post(&toPrint);
}

void printBuffer(){

	sem_wait(&toPrint);
	printf("Printing buffer...\n");
	while (bufferPos - initBufferPos != 0){

		printf("%c\n", *initBufferPos);
		initBufferPos++;
	}
	printf("Done!\n");
}

void print_sched(int policy)
{
	int priority_min, priority_max;

	switch(policy){
		case SCHED_DEADLINE:
			printf("SCHED_DEADLINE");
			break;
		case SCHED_FIFO:
			printf("SCHED_FIFO");
			break;
		case SCHED_RR:
			printf("SCHED_RR");
			break;
		case SCHED_NORMAL:
			printf("SCHED_OTHER");
			break;
		case SCHED_BATCH:
			printf("SCHED_BATCH");
			break;
		case SCHED_IDLE:
			printf("SCHED_IDLE");
			break;
		default:
			printf("unknown\n");
	}
	priority_min = sched_get_priority_min(policy);
	priority_max = sched_get_priority_max(policy);
	printf(" PRI_MIN: %d PRI_MAX: %d\n", priority_min, priority_max);
}

int setpriority(pthread_t *thr, int newpolicy, int newpriority)
{
	int policy, ret;
	struct sched_param param;

	if (newpriority > sched_get_priority_max(newpolicy) || newpriority < sched_get_priority_min(newpolicy)){
		printf("Invalid priority: MIN: %d, MAX: %d", sched_get_priority_min(newpolicy), sched_get_priority_max(newpolicy));

		return -1;
	}

	pthread_getschedparam(*thr, &policy, &param);
	printf("current: ");
	print_sched(policy);

	param.sched_priority = newpriority;
	ret = pthread_setschedparam(*thr, newpolicy, &param);
	if (ret != 0)
		perror("perror(): ");

	pthread_getschedparam(*thr, &policy, &param);
	printf("new: ");
	print_sched(policy);

	return 0;
}

int getPolicyCode(char* policy){

	int policyCode;

	if (strcmp(policy, "SCHED_DEADLINE") == 0) policyCode = SCHED_DEADLINE;
	else if (strcmp(policy, "SCHED_FIFO") == 0) policyCode = SCHED_FIFO;
	else if (strcmp(policy, "SCHED_RR") == 0) policyCode = SCHED_RR;
	else if (strcmp(policy, "SCHED_OTHER") == 0) policyCode = SCHED_NORMAL;
	else if (strcmp(policy, "SCHED_BATCH") == 0) policyCode = SCHED_BATCH;
	else if (strcmp(policy, "SCHED_IDLE") == 0) policyCode = SCHED_IDLE;
	else
	{
		policyCode = -1;
		printf("POLITICA NAO ENCONTRADA!\n");
	}
	return policyCode;
}


