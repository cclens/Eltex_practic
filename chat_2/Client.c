#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
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

int shmid, semid;
char username[50];
WINDOW *chatwin, *inputwin, *userwin;

void sem_wait(int semid) {
    struct sembuf sb = {0, -1, 0};
    semop(semid, &sb, 1);
}

void sem_signal(int semid) {
    struct sembuf sb = {0, 1, 0};
    semop(semid, &sb, 1);
}

void *receive_messages(void *arg) {
    struct shared_memory *shm = (struct shared_memory *)arg;
    int last_num_messages = 0;

    while (1) {
        sem_wait(semid);
        if (shm->num_messages != last_num_messages) {
            for (int i = last_num_messages; i < shm->num_messages; ++i) {
                wprintw(chatwin, "%s\n", shm->messages[i % NUM_CLIENTS]);
            }
            wrefresh(chatwin);
            last_num_messages = shm->num_messages;
        }
        sem_signal(semid);
        sleep(0.1);  
    }
    pthread_exit(NULL);
}

void send_message(const char *text) {
    struct shared_memory *shm = (struct shared_memory *)shmat(shmid, NULL, 0);

    sem_wait(semid);
    snprintf(shm->messages[shm->num_messages % NUM_CLIENTS], MAX_TEXT, "%s: %s", username, text);
    shm->num_messages++;
    sem_signal(semid);

    shmdt(shm);
}

void update_user_list() {
    werase(userwin);
    box(userwin, 0, 0);
    mvwprintw(userwin, 1, 1, "Connected users:");
    mvwprintw(userwin, 2, 1, "%s", username);
    wrefresh(userwin);
}

int main() {
    pthread_t recv_thread;
    char input[MAX_TEXT];

    shmid = shmget(SHM_KEY, sizeof(struct shared_memory), 0666);
    if (shmid == -1) {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }

    semid = semget(SEM_KEY, 1, 0666);
    if (semid == -1) {
        perror("semget failed");
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

    struct shared_memory *shm = (struct shared_memory *)shmat(shmid, NULL, 0);

    if (pthread_create(&recv_thread, NULL, receive_messages, (void *)shm) != 0) {
        perror("pthread_create failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        werase(inputwin);
        box(inputwin, 0, 0);
        mvwprintw(inputwin, 1, 1, "> ");
        wrefresh(inputwin);
        echo();
        wgetnstr(inputwin, input, MAX_TEXT - 1);
        noecho();
        send_message(input);
    }

    endwin();
    return 0;
}

