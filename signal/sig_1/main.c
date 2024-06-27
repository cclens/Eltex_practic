#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
void print_sig(int sig){

	printf("%s%d\n","поступил сигнал с номером: ", sig);

}

int main(){

	struct sigaction sa;
	sa.sa_handler = print_sig;

	if (sigaction(10, &sa, NULL ) < 0){
		exit(EXIT_FAILURE);
	}	

	while(1){
		sleep(3);
	}

}
