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
    
    // Генерируем ключ для сегмента разделяемой памяти
    key = ftok(".", 'S');
    if (key == -1) {
        perror("ftok");
        exit(1);
    }
    
    // Создаем сегмент разделяемой памяти
    shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }
    
    // Подключаемся к сегменту разделяемой памяти
    shm = shmat(shmid, NULL, 0);
    if (shm == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    
    // Записываем сообщение в сегмент разделяемой памяти
    msg = "Hi!";
    strncpy(shm, msg, SHM_SIZE);
    
    printf("Server sent: %s\n", shm);
    
    // Ждем ответа от клиента
    printf("Waiting for client's response...\n");
    
    // Ждем, пока клиент не запишет свой ответ
    while (strcmp(shm, "Hello!") != 0) {
        sleep(1);  // Подождем 1 секунду перед проверкой снова
    }
    
    // Клиент записал свой ответ, выводим его на экран
    printf("Server received: %s\n", shm);
    
    // Отключаемся от сегмента разделяемой памяти
    if (shmdt(shm) == -1) {
        perror("shmdt");
        exit(1);
    }
    
    // Удаляем сегмент разделяемой памяти
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }
    
    return 0;
}

