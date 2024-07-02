#include <locale.h>
#include <ncursesw/ncurses.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

struct mq_attr attr;

mqd_t mq_in;
mqd_t mq_out;
mqd_t mq_adm_in;
mqd_t mq_adm_out;
mqd_t mq_list_in;
mqd_t mq_list_out;

pthread_t thread_in;
pthread_t thread_out;
pthread_t thread_adm;
pthread_t thread_list;

char buffer[1024];
char name[30];
char sub_name[30];

WINDOW *chat_window;
WINDOW *input_window;
WINDOW *users_window;

void check_mq_open(mqd_t mq, const char* mq_name) {
    if (mq == (mqd_t)-1) {
        perror(mq_name);
        exit(EXIT_FAILURE);
    }
}

void* User_adm(void* arg) {
    mq_send(mq_adm_in, name, strlen(name) + 1, 0);
    mq_receive(mq_adm_out, name, sizeof(name), NULL);
    return NULL;
}

void* List_count(void* arg) {
    while(1) {
        ssize_t bytes_read = mq_receive(mq_list_in, sub_name, sizeof(sub_name), NULL);
        if (bytes_read >= 0) {
            sub_name[bytes_read] = '\0'; // Null-terminate the string
            werase(users_window);
            box(users_window, 0, 0);
            mvwprintw(users_window, 1, 1, "Users:");
            wprintw(users_window, "%s\n", sub_name);
            wrefresh(users_window);
        }
        usleep(300000); // Используем usleep вместо sleep для более точного управления временем
    }
    return NULL;
}

void* Output_chat(void* arg) {
    while(1) {
        ssize_t bytes_read = mq_receive(mq_out, buffer, sizeof(buffer), NULL);
        if (bytes_read >= 0) {
            buffer[bytes_read] = '\0'; // Null-terminate the string
            wprintw(chat_window, "%s\n", buffer);
            wrefresh(chat_window);
            // Отладочное сообщение
            printf("Received message: %s\n", buffer);
            fflush(stdout); // Чтобы отладочное сообщение сразу выводилось на экран
        } else {
            perror("mq_receive in Output_chat");
        }
    }
    return NULL;
}

void* Input_chat(void* arg) {
    char message[1024];
    sprintf(message, "%s присоединился к сессии", name); 
    mq_send(mq_in, message, strlen(message) + 1, 0);
    while (1) {
        wgetstr(input_window, buffer);
        werase(input_window);
        box(input_window, 0, 0);
        wrefresh(input_window);
        if (strcmp(buffer, "exit") == 0) {
            mq_send(mq_adm_in, "exit", strlen("exit") + 1, 0);
            sleep(1);
            endwin(); // Завершаем ncurses перед выходом
            exit(0);
        }
        sprintf(message, "%s: %s", name, buffer);
        mq_send(mq_in, message, strlen(message) + 1, 0);
    }
    return NULL;
}

void create_windows(int chat_win_height, int users_win_width, int input_win_height) {
    chat_window = newwin(chat_win_height, COLS - users_win_width, 0, 0);
    users_window = newwin(chat_win_height, users_win_width, 0, COLS - users_win_width);
    input_window = newwin(input_win_height, COLS, chat_win_height, 0);

    scrollok(chat_window, TRUE);
    box(chat_window, 0, 0);
    box(users_window, 0, 0);
    box(input_window, 0, 0);

    mvwprintw(chat_window, 0, 1, " Chat ");
    mvwprintw(users_window, 0, 1, " Users ");
    mvwprintw(input_window, 0, 1, " Input ");

    wrefresh(chat_window);
    wrefresh(users_window);
    wrefresh(input_window);
}

int main() {
    attr.mq_flags = 0;        // no flags
    attr.mq_maxmsg = 10;      // maximum number of messages
    attr.mq_msgsize = 1024;   // maximum message size
    attr.mq_curmsgs = 0;      // number of messages currently queued

    mq_in = mq_open("/flag.txt1", O_CREAT | O_RDWR, 0666, &attr);
    check_mq_open(mq_in, "mq_in");
    mq_out = mq_open("/flag.txt2", O_CREAT | O_RDWR, 0666, &attr);
    check_mq_open(mq_out, "mq_out");
    mq_adm_in = mq_open("/flag.txt3", O_CREAT | O_RDWR, 0666, &attr);
    check_mq_open(mq_adm_in, "mq_adm_in");
    mq_adm_out = mq_open("/flag.txt4", O_CREAT | O_RDWR, 0666, &attr);
    check_mq_open(mq_adm_out, "mq_adm_out");
    mq_list_in = mq_open("/flag.txt5", O_CREAT | O_RDWR, 0666, &attr);
    check_mq_open(mq_list_in, "mq_list_in");
    mq_list_out = mq_open("/flag.txt6", O_CREAT | O_RDWR, 0666, &attr);
    check_mq_open(mq_list_out, "mq_list_out");

    setlocale(LC_ALL, "");

    initscr();
    clear();
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);

    attron(COLOR_PAIR(2));
    attron(A_BOLD);
    printw("Введите свой никнейм: ");
    attroff(A_BOLD);
    getstr(name);
    attroff(COLOR_PAIR(2));

    int chat_win_height = LINES - 4;
    int users_win_width = COLS / 4;
    int input_win_height = 3;

    create_windows(chat_win_height, users_win_width, input_win_height);

    pthread_create(&thread_in, NULL, Input_chat, NULL);
    pthread_create(&thread_out, NULL, Output_chat, NULL);
    pthread_create(&thread_adm, NULL, User_adm, NULL);
    pthread_create(&thread_list, NULL, List_count, NULL);

    pthread_join(thread_in, NULL);
    pthread_join(thread_out, NULL);
    pthread_join(thread_adm, NULL);
    pthread_join(thread_list, NULL);

    mq_close(mq_adm_in);
    mq_close(mq_adm_out);
    mq_close(mq_in);
    mq_close(mq_out);
    mq_close(mq_list_in);
    mq_close(mq_list_out);
    endwin();
    return 0;
}
