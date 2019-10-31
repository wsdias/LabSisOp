#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define nMSG 20

int contaPostIt;
sem_t mutex;
sem_t cheia;
sem_t enchendo;

void * usuario();
void * pombo();

int main (){

	pthread_t threads[2];

	contaPostIt = 0;
	sem_init(&mutex, 0, 1);
	sem_init(&cheia, 0, 0);
	sem_init(&enchendo, 0, nMSG);

	pthread_create(&threads[0], NULL, pombo, NULL);
	pthread_create(&threads[1], NULL, usuario, NULL);

	pthread_exit(NULL);
	return 0;
}

void * usuario(){

	printf("Entrou em usuário.\n");

	while(1){
		sem_wait(&enchendo); // Decrementa número de mensagem disponíveis. s0 = 20 --> s1 = 19 --> s2 = 18 ...
		sem_wait(&mutex); // Decrementa semário de exclusão mútua. Faz o pombo esperar
		printf("Colando Post-it: %d\n", contaPostIt+1);
		contaPostIt++; // Incrementa número de post-its colados
		sleep(1);
		if (contaPostIt == nMSG) // Quando mensagens chegam a 20
			sem_post(&cheia); // Incrementa, liberando o pombo
		sem_post(&mutex); // Incrementa, liberando o pombo
	}
}

void * pombo(){

	printf("Entrou em pombo.\n");

	int i;

	while(1){
		sem_wait(&cheia); // Decrementa, pombo é liberado quando mensagens chegam a 20 (sem_post(&cheia))
		sem_wait(&mutex); // Decrementa, pombo é liberado (sem_post(&mutex))
		printf("Levando mensagens...\n");
		contaPostIt = 0; // Faz reset no número de post-its colados
		sleep(1);
		for(i = 0; i < nMSG; i++)
			sem_post(&enchendo);
		printf("Voltando...\n");
		sem_post(&mutex);
	}
}
