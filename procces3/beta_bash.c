#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

int main(){
	
	char command[15];
	
	while(strncmp(command, "exit", 4) != 0){
		printf("введите команду которую хотите исполнить, для выхода введите exit \n");
		fgets(command, sizeof(command), stdin);
		command[strcspn(command, "\n")] = 0;
		pid_t main_pid = fork();
		
		if (main_pid < 0){
			printf("ошибка вызова процесса main_pid\n");
			exit(EXIT_FAILURE);
		}
		else if (main_pid == 0){
			if (system(command) == (-1)){
				printf("ошибка выполнения команды :) \n");
			}
		 	exit(EXIT_SUCCESS);
		}
		 else {
            int status = 0; 
            waitpid(main_pid, &status, 0); // Ожидание завершения дочернего процесса
        }
	}

}
