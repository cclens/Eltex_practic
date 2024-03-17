#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t proc1 = fork();
    pid_t proc2, proc3, proc4, proc5;
    int wstatus = 0;

    if (proc1 == -1) {
        printf("bad call fork1");
        exit(EXIT_FAILURE);
    } else if (proc1 == 0) {
        printf("Был порожден процесс 1\nPID: %d\nPPID: %d\n", getpid(), getppid()); 
        proc3 = fork(); 
        if (proc3 == -1) {
            printf("ошибка вызова процесса 3");
            exit(EXIT_FAILURE);
        } else if (proc3 == 0) {
            printf("Был порожден процесс 3\nPID: %d\nPPID: %d\n", getpid(), getppid()); 
            sleep(2); // имитация работы
            printf("процесс 3 завершен\n");
            exit(EXIT_SUCCESS);
        }

        if (proc3 != 0) {
            proc4 = fork();
            if (proc4 == -1) {
                printf("bad call fork3");
                exit(EXIT_FAILURE);
            } else if (proc4 == 0) {
                printf("Был порожден процесс 4\nPID: %d\nPPID: %d\n", getpid(), getppid()); 
                sleep(2); // имитация работы
                printf("процесс 4 завершен\n");
                exit(EXIT_SUCCESS);
            }
        }
        wait(&wstatus);
        if (WIFEXITED(wstatus) != 0) {
            printf("процесс 1 успешно завершен\n");
        } else {
            printf("ошибка завершения дочерних процессов у процесса 1\n");
        }
        exit(EXIT_SUCCESS);
    }

    proc2 = fork();
    if (proc2 < 0) {
        printf("ошибка вызова процесса 2\n");
        exit(EXIT_FAILURE);
    } else if (proc2 == 0) {
        printf("Был порожден процесс 2\nPID: %d\nPPID: %d\n", getpid(), getppid()); 
        proc5 = fork();

        if (proc5 < 0) {
            printf("ошибка вызова процесса 5\n");
            exit(EXIT_FAILURE);
        } else if (proc5 == 0) {
            printf("Был порожден процесс 5\nPID: %d\nPPID: %d\n", getpid(), getppid()); 
            sleep(2); // имитация работы
            printf("процесс 5 успешно завершен\n");
            exit(EXIT_SUCCESS);
        }
        wait(&wstatus);
        if (WIFEXITED(wstatus) != 0) {
            printf("процесс 2 успешно завершен\n");
        } else {
            printf("ошибка завершения дочерних процессов у процесса 2\n");
        }
        exit(EXIT_SUCCESS);
    } else {
        wait(&wstatus); // Ждем завершения дочернего процесса proc1
        wait(&wstatus); // Ждем завершения дочернего процесса proc2
        printf("дочерние процессы успешно завершены\n");
        printf("завершение родительского процесса\nPID: %d\nPPID: %d\n", getpid(), getppid());
        exit(EXIT_SUCCESS);
    }
}
