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

    key = ftok("/tmp", QUEUE_KEY);
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    msg_id = msgget(key, IPC_CREAT | 0666);
    if (msg_id == -1) {
        perror("msgget");
        exit(1);
    }

    if (msgrcv(msg_id, &msg, sizeof(msg.msg_text), 1, 0) == -1) {
        perror("msgrcv");
        exit(1);
    }

    printf("Сервер получил сообщение: %s\n", msg.msg_text);

    strcpy(msg.msg_text, "Hello!");
    msg.msg_type = 2;
    if (msgsnd(msg_id, &msg, sizeof(msg.msg_text), 0) == -1) {
        perror("msgsnd");
        exit(1);
    }

    if (msgctl(msg_id, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(1);
    }

    return 0;
}

