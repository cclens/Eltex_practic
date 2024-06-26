#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define QUEUE_KEY 1234

struct msg_buffer {
    long msg_type;
    char msg_text[256];
};

int main() {
    key_t key;
    int msg_id;
    struct msg_buffer msg;

    // Генерируем ключ для очереди сообщений
    key = ftok("/tmp", QUEUE_KEY);
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    // Получаем доступ к очереди сообщений
    msg_id = msgget(key, 0666);
    if (msg_id == -1) {
        perror("msgget");
        exit(1);
    }

    // Отправляем сообщение серверу
    msg.msg_type = 1;
    strcpy(msg.msg_text, "Hi!");
    if (msgsnd(msg_id, &msg, sizeof(msg.msg_text), 0) == -1) {
        perror("msgsnd");
        exit(1);
    }

    // Получаем ответ от сервера
    if (msgrcv(msg_id, &msg, sizeof(msg.msg_text), 2, 0) == -1) {
        perror("msgrcv");
        exit(1);
    }

    printf("Клиент получил ответ: %s\n", msg.msg_text);

    return 0;
}

