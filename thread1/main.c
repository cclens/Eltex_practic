#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define N 5


void* Pthread_shop(void *arg){
	int *i = (int*) arg;
	printf("pthread num %d\n", *i);


}
// ПЕРЕИМЕНОВАТЬ ФУНКЦИЮ%

int main(){
	pthread_t thread[N];
	int arr[N];
	for (int i = 0; i <= N-1; i++){
		arr[i] = i+1;
		pthread_create(&thread[i], NULL, Pthread_shop,  (void*)&arr[i]);
	
	}
	for (int i1 = 0; i1 <= N-1; i1++){
		pthread_join(thread[i1], NULL); 
	}
}

