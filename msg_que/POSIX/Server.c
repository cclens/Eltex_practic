#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define QUEUE_NAME "/test_queue"
#define MAX_MSG_SIZE 256

int main() {
    mqd_t mq;
    struct mq_attr attr;
    char buffer[MAX_MSG_SIZE + 1]; // Увеличиваем размер буфера на 1 для '\0'
    int msg_len;

    // Открываем очередь сообщений
    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDWR, 0666, NULL);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        exit(1);
    }

    // Получаем атрибуты очереди, чтобы узнать максимальный размер сообщения
    if (mq_getattr(mq, &attr) == -1) {
        perror("mq_getattr");
        exit(1);
    }

    // Читаем сообщение от клиента
    msg_len = mq_receive(mq, buffer, attr.mq_msgsize, NULL);
    if (msg_len == -1) {
        perror("mq_receive");
        exit(1);
    }

    buffer[msg_len] = '\0';
    printf("Сервер получил сообщение: %s\n", buffer);

    // Отправляем ответ клиенту
    sprintf(buffer, "Hello!");
    if (mq_send(mq, buffer, strlen(buffer) + 1, 0) == -1) {
        perror("mq_send");
        exit(1);
    }

    // Закрываем очередь
    if (mq_close(mq) == -1) {
        perror("mq_close");
        exit(1);
    }

    // Удаляем очередь
    if (mq_unlink(QUEUE_NAME) == -1) {
        perror("mq_unlink");
        exit(1);
    }

    return 0;
}

