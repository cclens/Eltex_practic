#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t  m1;

void *calc(void *arg){
	int *sum = (int*)arg;
	while(*sum < 1000){
		*sum += 1;	
	}
}

int main(){
	pthread_t theard[5];
	int sum = 0;

	for (int i = 0; i < 5; i ++){
		pthread_create(&theard[i], NULL, calc, &sum);
	}
	for (int j = 0; j <5; j++){
		pthread_join(theard[j], NULL);
	}
	printf("%d\n", sum);
}
