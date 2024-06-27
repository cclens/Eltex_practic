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
    char *shm;
    
    key = ftok(".", 'S');
    if (key == -1) {
        perror("ftok");
        exit(1);
    }
    
    shmid = shmget(key, SHM_SIZE, 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }
    
    shm = shmat(shmid, NULL, 0);
    if (shm == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    
    printf("Client received: %s\n", shm);
    
    // Отвечаем серверу
    char *msg = "Hello!";
    strncpy(shm, msg, SHM_SIZE);
    
    printf("Client sent: %s\n", shm);
    
    if (shmdt(shm) == -1) {
        perror("shmdt");
        exit(1);
    }
    
    return 0;
}

