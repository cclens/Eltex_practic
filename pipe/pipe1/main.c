#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>


int main(){
	int fd[2];
	pid_t sub_pid;
	int wstatus = 0;
	char data[] = "Hi!";
	char buffer[100];
	if (pipe(fd) == -1){
		perror("pipe");
	}

	sub_pid = fork();

	if (sub_pid < 0){
		printf("ошибка создания процесса\n");
		exit(EXIT_FAILURE);
	}	
	else if (sub_pid == 0){
		printf("вы вызвали дочерний процесс\n");
		close(fd[1]);
		read(fd[0], buffer, sizeof(buffer));
		printf("%s\n", buffer);
		close(fd[0]);
		exit(EXIT_SUCCESS);
	}
	else{
		printf("вы вызвали родительский процесс\n");
		close(fd[0]);
		write(fd[1], data, sizeof(data));
		close(fd[1]);
		wait(&wstatus);
		if (WIFEXITED(wstatus)){
			printf("родительский процесс успешно завершен после дочернего\n");
		}
		else {
			printf("что то пошло не так\n");
		}
	}
	exit(0);

}
