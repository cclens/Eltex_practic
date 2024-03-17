#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
int main(){
	
	pid_t ret;
	ret = fork();
	int wstatus;	

	if (ret == -1 ){
		perror("fork");
		printf("Bad call fork");
	}
	else if (ret == 0){
		printf("Вы вызвали дочерний процесс\n");
		printf("%d\n", getpid());
		printf("дочерний процесс успешно завершенн\n");
		exit(EXIT_SUCCESS);
	}
	else{
		wait(&wstatus);
		printf("%s\n", "Вы вызвали родительский процесс", "%d\n", getpid());
		
		if (WIFEXITED(wstatus)){ 
			printf("родительский процесс успешно завершен после дочернего");
		}
		else{
			printf("что то пошло не так при завершении");
		} 
	}
	exit(0);
}
