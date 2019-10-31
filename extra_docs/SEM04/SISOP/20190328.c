#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int vetor[50];

void *inserir (){

	int i;

	for (i = 0; i < 50; i++){
		vetor[i] = rand () % 100;
		printf ("%d ", i);
		//sleep (1);
	}

	return NULL;
}

void *ordenar (){

}

void *imprimir (){

	int i;

	for (i = 0; i < 50; i++){
		printf ("%d ", i);
		printf ("%d ", vetor[i]);
		if (i%10 == 0 && i != 0){
			printf ("\n");
		}
		sleep (2);
	}

	return NULL;
}

int main (){

	pthread_t threads[3];

	pthread_create (&threads[0], NULL, inserir (), NULL);

	//pthread_create (&threads[2], NULL, imprimir (), NULL);

	printf ("\n");

	return 0;
}