#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_TEXT 512
#define SERVER_QUEUE_KEY 1234

struct message {
    long msg_type;
    char text[MAX_TEXT];
};

int main() {
    int server_msgid;
    struct message msg;
    key_t key = SERVER_QUEUE_KEY;
    char users[10][50];
    int num_users = 0;

    server_msgid = msgget(key, 0666 | IPC_CREAT);
    if (server_msgid == -1) {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }

    printf("Server started. Server message queue ID: %d\n", server_msgid);

    while (1) {
        if (msgrcv(server_msgid, (void *)&msg, sizeof(msg.text), 0, 0) == -1) {
            perror("msgrcv failed");
            continue;
        }

        if (msg.msg_type == 2) {
            // Сообщение о подключении нового пользователя
            strcpy(users[num_users], msg.text);
            num_users++;
            for (int i = 0; i < num_users; ++i) {
                struct message user_list_msg;
                user_list_msg.msg_type = 2;
                strcpy(user_list_msg.text, users[i]);
                if (msgsnd(server_msgid, (void *)&user_list_msg, sizeof(user_list_msg.text), 0) == -1) {
                    perror("msgsnd failed");
                }
            }
        } else {
            printf("Received message: %s\n", msg.text);
            for (int i = 0; i < num_users; ++i) {
                if (msgsnd(server_msgid, (void *)&msg, sizeof(msg.text), 0) == -1) {
                    perror("msgsnd failed");
                }
            }
        }
    }

    msgctl(server_msgid, IPC_RMID, NULL);

    return 0;
}

