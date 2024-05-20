#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <wait.h>

char buff[100][100];
char buff2[100][100];

void splitString(char* input){
    char* token;
    int index = 0;

    // Разбиваем строку по символу '|'
    token = strtok(input, "|");
    while (token != NULL) {
        while (*token == ' ') {
            token++;
        }
        int len = strlen(token);
        while (len > 0 && (token[len - 1] == ' ' || token[len - 1] == '\n')) {
            token[len - 1] = '\0';
            len--;
        }
        
        if (index == 0) {
            strcpy(buff[0], token); 
        } else if (index == 1) {
            char* word = strtok(token, " ");
            int word_index = 0;
            while (word != NULL) {
                int word_len = strlen(word);
                while (word_len > 0 && word[word_len - 1] == '\n') {
                    word[word_len - 1] = '\0';
                    word_len--;
                }
                strcpy(buff2[word_index], word);
                word_index++;
                word = strtok(NULL, " ");
            }
        }
        index++;
        token = strtok(NULL, "|");
    }
}

int main() {
    int fd[2];
    int wstatus1;
    int wstatus2;
    pid_t pid_1;
    pid_t pid_2;
    char path[100] = "/bin/";
    char input_string[100];
    
    while (1) { // Бесконечный цикл для ожидания команд
        printf("Введите команду: ");
        fgets(input_string, sizeof(input_string), stdin);
        
        // Удаляем символ новой строки из введенной строки
        input_string[strcspn(input_string, "\n")] = 0;

        // Проверяем, является ли введенная команда "exit"
        if (strcmp(input_string, "exit") == 0) {
            printf("Выход из программы.\n");
            break;
        }

        // Проверяем наличие символа '|'
        if (strstr(input_string, "|") != NULL) {
            // Если символ '|' есть, обрабатываем команду как раньше
            splitString(input_string);
            
            if (pipe(fd) == -1) {
                perror("Ошибка создания канала");
                exit(EXIT_FAILURE);
            }

            pid_1 = fork();

            if (pid_1 < 0) {
                perror("Ошибка создания процесса");
                exit(EXIT_FAILURE);
            } else if (pid_1 == 0) {
                strcat(path, buff[0]);
                dup2(fd[1], 1); // Перенаправляем stdout в канал
                execl(path, buff[0], NULL);
                perror("Ошибка выполнения execl");
                exit(EXIT_FAILURE);
            } else {
                close(fd[1]); // Закрываем ненужный дескриптор в родительском процессе
                wait(&wstatus1);
                if (WIFEXITED(wstatus1) != 0) {
                    pid_2 = fork();
                    if (pid_2 < 0) {
                        perror("Ошибка вызова процесса 2");
                        exit(EXIT_FAILURE);
                    } else if (pid_2 == 0) {
                        dup2(fd[0], 0); 
                        strcpy(path, "/bin/");
                        strcat(path, buff2[0]);
                        execl(path, buff2[0], buff2[1], NULL);
                        perror("Ошибка выполнения execl");
                       // exit(EXIT_FAILURE);
                    } else {
                        close(fd[0]); 
                        wait(&wstatus2);
                        if (WIFEXITED(wstatus2) != 0) {
                            //exit(EXIT_SUCCESS);
                        }
                    }
                } else {
                    perror("Ошибка завершения родительского процесса");
                    exit(EXIT_FAILURE);
                }
            }
        } else {
            // Если символ '|' отсутствует, выполняем команду напрямую
            char* args[2];
            args[0] = strtok(input_string, " ");
            args[1] = NULL;
            
            pid_t pid = fork();
            if (pid < 0) {
                perror("Ошибка создания процесса");
                exit(EXIT_FAILURE);
            } else if (pid == 0) {
                execvp(args[0], args);
                perror("Ошибка выполнения execvp");
                exit(EXIT_FAILURE);
            } else {
                wait(NULL);
            }
        }
    }
    
    return 0;
}
