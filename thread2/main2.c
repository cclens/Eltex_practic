#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
int a = 0;


void *calc(void *arg){
	
	for (int i = 0; i <= 10000; i++){
		pthread_mutex_lock(&m1);
		a++;
		pthread_mutex_unlock(&m1);
	}
	
}

int main(){
        pthread_t theard[5];
		int sum = 0;
		
        for (int i = 0; i < 5; i ++){
                pthread_create(&theard[i], NULL, calc, NULL);
        }
        for (int j = 0; j <5; j++){
                pthread_join(theard[j], NULL);
        }
        printf("%d\n", a);
		pthread_mutex_destroy(&m1);
}

