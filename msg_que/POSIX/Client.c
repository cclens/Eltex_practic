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
    char buffer[MAX_MSG_SIZE + 1]; // Увеличиваем размер буфера на 1 для '\0'
    struct mq_attr attr;

    // Открываем очередь сообщений для записи (к серверу)
    mq = mq_open(QUEUE_NAME, O_WRONLY);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        exit(1);
    }

    // Отправляем сообщение серверу
    sprintf(buffer, "Hi serv!");
    if (mq_send(mq, buffer, strlen(buffer) + 1, 0) == -1) {
        perror("mq_send");
        exit(1);
    }

    // Закрываем очередь для записи
    if (mq_close(mq) == -1) {
        perror("mq_close");
        exit(1);
    }

    // Открываем очередь сообщений для чтения (ответа от сервера)
    mq = mq_open(QUEUE_NAME, O_RDONLY);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        exit(1);
    }

    // Получаем атрибуты очереди, чтобы узнать максимальный размер сообщения
    if (mq_getattr(mq, &attr) == -1) {
        perror("mq_getattr");
        exit(1);
    }

    // Читаем ответ от сервера
    ssize_t bytes_read = mq_receive(mq, buffer, attr.mq_msgsize, NULL);
    if (bytes_read == -1) {
        perror("mq_receive");
        exit(1);
    }

    buffer[bytes_read] = '\0'; // Убедимся, что строка заканчивается нулевым символом
    printf("Клиент получил ответ: %s\n", buffer);

    // Закрываем очередь
    if (mq_close(mq) == -1) {
        perror("mq_close");
        exit(1);
    }

    return 0;
}

