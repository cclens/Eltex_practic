#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#define SHM_SIZE 128  // Размер разделяемой памяти

int main() {
    int shmid;
    key_t key;
    char *shm, *msg;
    
    key = ftok(".", 'S');
    if (key == -1) {
        perror("ftok");
        exit(1);
    }
    
    shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }
    
    shm = shmat(shmid, NULL, 0);
    if (shm == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    
    msg = "Hi!";
    strncpy(shm, msg, SHM_SIZE);
    
    printf("Server sent: %s\n", shm);
    
    printf("Waiting for client's response...\n");
    sleep(5);  
    
    printf("Server received: %s\n", shm);
    
    if (shmdt(shm) == -1) {
        perror("shmdt");
        exit(1);
    }
    
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }
    
    return 0;
}

