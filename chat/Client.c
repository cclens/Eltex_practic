#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_TEXT 512
#define SERVER_QUEUE_KEY 1234
#define NUM_CLIENTS 3  // Количество клиентов задается здесь

struct message {
    long msg_type;
    char text[MAX_TEXT];
};

int msgid;
char username[50];
WINDOW *chatwin, *inputwin, *userwin;

void *receive_messages(void *arg) {
    struct message msg;
    while (1) {
        if (msgrcv(msgid, (void *)&msg, sizeof(msg.text), 0, 0) == -1) {
            perror("msgrcv failed");
            break;
        }
        wprintw(chatwin, "%s\n", msg.text);
        wrefresh(chatwin);
    }
    pthread_exit(NULL);
}

void send_message(const char *text) {
    struct message msg;
    msg.msg_type = 1;
    snprintf(msg.text, MAX_TEXT, "%s: %s", username, text);
    
    // Отправляем сообщение NUM_CLIENTS раз
    for (int i = 0; i < NUM_CLIENTS; ++i) {
        if (msgsnd(msgid, (void *)&msg, sizeof(msg.text), 0) == -1) {
            perror("msgsnd failed");
            exit(EXIT_FAILURE);
        }
    }
}

void update_user_list() {
    werase(userwin);
    box(userwin, 0, 0);
    mvwprintw(userwin, 1, 1, "Connected users:");
    // Пока добавляем только одного пользователя
    mvwprintw(userwin, 2, 1, "%s", username);
    wrefresh(userwin);
}

int main() {
    pthread_t recv_thread;
    char input[MAX_TEXT];
    key_t key = SERVER_QUEUE_KEY;

    msgid = msgget(key, 0666);
    if (msgid == -1) {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }

    printf("Enter your username: ");
    scanf("%s", username);

    initscr();
    cbreak();
    noecho();

    int height, width;
    getmaxyx(stdscr, height, width);

    chatwin = newwin(height - 3, width - 20, 0, 0);
    inputwin = newwin(3, width - 20, height - 3, 0);
    userwin = newwin(height, 20, 0, width - 20);

    scrollok(chatwin, TRUE);
    box(chatwin, 0, 0);
    box(inputwin, 0, 0);
    box(userwin, 0, 0);

    mvwprintw(inputwin, 1, 1, "> ");
    wrefresh(chatwin);
    wrefresh(inputwin);
    wrefresh(userwin);

    update_user_list();

    if (pthread_create(&recv_thread, NULL, receive_messages, NULL) != 0) {
        perror("pthread_create failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        werase(inputwin);
        box(inputwin, 0, 0);
        mvwprintw(inputwin, 1, 1, "> ");
        wrefresh(inputwin);
        // Включаем echo для отображения ввода
        echo();
        wgetnstr(inputwin, input, MAX_TEXT - 1);
        noecho();  // Отключаем echo после ввода
        send_message(input);
    }

    endwin();
    return 0;
}

