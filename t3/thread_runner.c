#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <linux/sched.h>
#include <string.h>

int nThreads, bufferSize, priority, bufferIndex, *occurrences;
char *buffer, *policy;
sem_t mutex, sync;

void * thread(int id);
void print();
void print_sched(int policy);
int setpriority(pthread_t *thr, int newpolicy, int newpriority);
int policyCode(char* policy);

int main (int argc, char *argv[]){

	int i;

	if (argc == 5){

		nThreads = atoi(argv[1]);
		bufferSize = atoi(argv[2]) * 1024;
		policy = argv[3];
		priority = atoi(argv[4]);

		//printf("nThreads: %u\n", nThreads);
		//printf("bufferSize: %u bytes\n", bufferSize);
		//printf("policy: %s\n", policy);
		//printf("priority: %u\n", priority);

		buffer = malloc(sizeof(char) * bufferSize);
		occurrences = malloc(sizeof(int) * nThreads);
		for (i = 0; i < nThreads; i++)
			occurrences[i] = 0;
		pthread_t threads[nThreads];
		sem_init(&mutex, 0, 1);
		sem_init(&sync, 0, 0);
		bufferIndex = 0;

		sleep(3);

		for (i = 0; i < nThreads; i++)
			pthread_create(&threads[i], NULL, thread, i);

		for (i = 0; i < nThreads; i++)
			setpriority(&threads[i], policyCode(policy), priority);

		for (i = 0; i < nThreads; i++)
			sem_post(&sync);

		for (i = 0; i < nThreads; i++)
			pthread_join(threads[i], NULL);

		print();
	}

	pthread_exit(NULL);
	return 0;
}

void * thread(int id){

	sem_wait(&sync);
	while(1){

		sem_wait(&mutex);
		if (bufferIndex == bufferSize - 1){

			//printf("Saiu: %d  |  %d\n", id, bufferIndex);
			sem_post(&mutex);
			break;
		}
		buffer[bufferIndex] = (char)(id + 65);
		bufferIndex++;
		sem_post(&mutex);
	}
}

void print(){

	char old;
	int i;

	i = 0;
	printf("\n\n");
	while(1){

		if (i == bufferSize - 1) break;
		old = buffer[i];
		printf("%c ", old);
		occurrences[(int)(old - 65)] += 1;
		i++;
		while(buffer[i] == old) i++;
	}
	printf("\n\n");

	for (i = 0; i < nThreads; i++)
		printf("%c = %d\n", (i + 65), occurrences[i]);
	printf("\n");

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

int policyCode(char* policy){

	if (strcmp(policy, "SCHED_DEADLINE") == 0) return SCHED_DEADLINE;
	else if (strcmp(policy, "SCHED_FIFO") == 0) return SCHED_FIFO;
	else if (strcmp(policy, "SCHED_RR") == 0) return SCHED_RR;
	else if (strcmp(policy, "SCHED_OTHER") == 0) return SCHED_NORMAL;
	else if (strcmp(policy, "SCHED_BATCH") == 0) return SCHED_BATCH;
	else if (strcmp(policy, "SCHED_IDLE") == 0) return SCHED_IDLE;
	else return -1;
}


