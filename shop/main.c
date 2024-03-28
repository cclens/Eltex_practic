#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define N_SHOP 4
#define N_BUYER 3
pthread_mutex_t mutex_shop[N_SHOP] = PTHREAD_MUTEX_INITIALIZER;
pthread_once_t once = PTHREAD_ONCE_INIT;

int shop_products[5];
int buyer_requests[3];
int flag = N_BUYER;

void filling(){
	for (int j = 0; j < N_BUYER; j++){
		buyer_requests[j] = rand() % (100000-80001)+80000;
		pthread_mutex_init(&mutex_shop[j], NULL); // Инициализация мьютексов для каждого элемента магазина
	}
	for (int i = 0; i < N_SHOP; i++){
    	shop_products[i] = rand() % (10000-8001)+8000;
    }
}

void *loading(){
	while (flag != 0){
		for (int i = 0; i <= N_BUYER; i++){
			if (shop_products[i] <= 300){ // если в магазине малое значение товара
				shop_products[i] += rand() % (6000-4001)+4000;
				 printf("%s%d%s\n", "магазин номер ", i, " пополнен" );
			}
		}
		sleep(3);
	}
}

void *buyer_work(void *arg){
	pthread_once(&once, filling);
	int *inc = (int*)arg;
	int random_shop;
	while (flag != 0){
		random_shop = rand() % N_SHOP; // Выбор случайного магазинaа
		while (pthread_mutex_trylock(&mutex_shop[random_shop]) != 0); // вход в случайный магазин, что бы не образовывались очереди
		printf("%s%d%s%d\n","покупатель номер ", *inc, " зашел в магазин номер ", random_shop);
		if (buyer_requests[*inc] == 0){
			flag -= 1;
			pthread_exit(NULL);
		}
		if (buyer_requests[*inc] > shop_products[*inc]){
			buyer_requests[*inc] -= shop_products[*inc];
			shop_products[*inc] = 0;
			printf("%s%d%s%d\n", "запрос покупателя номер ", *inc, " = ",buyer_requests[*inc]);
		}
		else if (buyer_requests[*inc] <= shop_products[*inc]){
			buyer_requests[*inc] = 0;
			shop_products[*inc] -= buyer_requests[*inc];	
		}
		sleep(3);
		pthread_mutex_unlock(&mutex_shop[random_shop]);
		
	}
}


int main(){
	pthread_t thread_loader;
	pthread_t thread_buyer[N_BUYER];
	int value[N_SHOP];

	for (int i = 0; i < N_BUYER; i++){
		value[i] = i;
		pthread_create(&thread_buyer[i], NULL, &buyer_work, &value[i]);
	}
	pthread_create(&thread_loader, NULL, loading, NULL);

	for (int j = 0; j < N_BUYER; j++){
		pthread_join(thread_buyer[j], NULL);
	}
	pthread_join(thread_loader, NULL);
	pthread_mutex_destroy(mutex_shop);	
}
