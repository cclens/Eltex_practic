#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>

#define MAX_TEXT 512
#define SHM_KEY 1234
#define SEM_KEY 5678
#define NUM_CLIENTS 3

struct shared_memory {
    char messages[NUM_CLIENTS][MAX_TEXT];
    int num_messages;
};

void sem_wait(int semid) {
    struct sembuf sb = {0, -1, 0};
    semop(semid, &sb, 1);
}

void sem_signal(int semid) {
    struct sembuf sb = {0, 1, 0};
    semop(semid, &sb, 1);
}

int main() {
    int shmid, semid;

    shmid = shmget(SHM_KEY, sizeof(struct shared_memory), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }

    semid = semget(SEM_KEY, 1, 0666 | IPC_CREAT);
    if (semid == -1) {
        perror("semget failed");
        exit(EXIT_FAILURE);
    }

    if (semctl(semid, 0, SETVAL, 1) == -1) {
        perror("semctl failed");
        exit(EXIT_FAILURE);
    }

    struct shared_memory *shm = (struct shared_memory *)shmat(shmid, NULL, 0);
    shm->num_messages = 0;
    shmdt(shm);

    printf("Server started. Shared memory ID: %d, Semaphore ID: %d\n", shmid, semid);

    while (1) {
        // Server loop doing nothing, shared memory and semaphores are used by clients
        sleep(1);
    }

    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);

    return 0;
}

