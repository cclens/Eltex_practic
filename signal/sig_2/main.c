#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(){

	struct sigaction sa;

	sa.sa_handler = SIG_IGN;

	sigaction(SIGINT, &sa, NULL);

	while(1){
		sleep(2);
	}

}

