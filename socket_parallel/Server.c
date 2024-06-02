#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define N 5
#define PORT_UDP 4444
#define PORT_TCP 1111

char ip_listen[] = "127.0.0.2";

struct free_client {
    char ip_addr[32];
    int socket_fd; // Дескриптор сокета для TCP
    struct sockaddr_in udp_addr; // Адрес клиента для UDP
    int type_connection; // 1 - tcp, 0 - udp
    int condition; // 1 - not free, 0 - free
};

struct free_client client_queue[N];
int queue_start = 0;
int queue_end = 0;
int queue_size = 0;

pthread_mutex_t connection_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queue_not_empty = PTHREAD_COND_INITIALIZER;

void *listen_func(void *arg) {
    int listen_socket_tcp = socket(AF_INET, SOCK_STREAM, 0);
    int listen_socket_udp = socket(AF_INET, SOCK_DGRAM, 0);
    if (listen_socket_tcp < 0 || listen_socket_udp < 0) {
        perror("ошибка создания сокета прослушки (socket)");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in listen_addr, client_addr;
    memset(&listen_addr, 0, sizeof(listen_addr));
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_port = htons(PORT_TCP);

    if (inet_pton(AF_INET, ip_listen, &listen_addr.sin_addr) <= 0) {
        perror("ошибка привязки ip слушающего сервера (inet_pton)");
        exit(EXIT_FAILURE);
    }

    if (bind(listen_socket_tcp, (struct sockaddr *)&listen_addr, sizeof(listen_addr)) < 0) {
        perror("TCP bind failed");
        exit(EXIT_FAILURE);
    }

    listen_addr.sin_port = htons(PORT_UDP);
    if (bind(listen_socket_udp, (struct sockaddr *)&listen_addr, sizeof(listen_addr)) < 0) {
        perror("UDP bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(listen_socket_tcp, N) < 0) {
        perror("TCP listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Tcp и Udp соединение прослушивается\n");

    fd_set readfds;
    int max_sd = listen_socket_tcp > listen_socket_udp ? listen_socket_tcp : listen_socket_udp;

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(listen_socket_tcp, &readfds);
        FD_SET(listen_socket_udp, &readfds);

        if (select(max_sd + 1, &readfds, NULL, NULL, NULL) < 0) {
            perror("select error");
            exit(EXIT_FAILURE);
        }

        if (FD_ISSET(listen_socket_tcp, &readfds)) {
            socklen_t addr_len = sizeof(client_addr);
            int new_listen_socket = accept(listen_socket_tcp, (struct sockaddr*)&client_addr, &addr_len);
            if (new_listen_socket < 0) {
                perror("ошибка прослушивания (accept)");
                exit(EXIT_FAILURE);
            }

            pthread_mutex_lock(&connection_mutex);

            if (inet_ntop(AF_INET, &client_addr.sin_addr, client_queue[queue_end].ip_addr, sizeof(client_queue[queue_end].ip_addr)) == NULL) {
                perror("ошибка преобразования IP-адреса (inet_ntop)");
            } else {
                client_queue[queue_end].socket_fd = new_listen_socket;
                client_queue[queue_end].type_connection = 1;
                client_queue[queue_end].condition = 1; // Помечаем как несвободное
                printf("Заявка на tcp соединение с айпи: %s\n", client_queue[queue_end].ip_addr);
                queue_end = (queue_end + 1) % N;
                queue_size++;

                pthread_cond_broadcast(&queue_not_empty);
            }

            pthread_mutex_unlock(&connection_mutex);
        }

        if (FD_ISSET(listen_socket_udp, &readfds)) {
            char buffer[1024];
            socklen_t addr_len = sizeof(client_addr);
            int recv_len = recvfrom(listen_socket_udp, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &addr_len);
            if (recv_len < 0) {
                perror("ошибка получения данных по UDP (recvfrom)");
                continue;
            }

            buffer[recv_len] = '\0';
            printf("Получено сообщение по UDP от %s: %s\n", inet_ntoa(client_addr.sin_addr), buffer);

            pthread_mutex_lock(&connection_mutex);

            if (inet_ntop(AF_INET, &client_addr.sin_addr, client_queue[queue_end].ip_addr, sizeof(client_queue[queue_end].ip_addr)) == NULL) {
                perror("ошибка преобразования IP-адреса (inet_ntop)");
            } else {
                client_queue[queue_end].udp_addr = client_addr;
                client_queue[queue_end].type_connection = 0;
                client_queue[queue_end].condition = 1; // Помечаем как несвободное
                printf("Заявка на udp соединение с айпи: %s\n", client_queue[queue_end].ip_addr);
                queue_end = (queue_end + 1) % N;
                queue_size++;

                pthread_cond_broadcast(&queue_not_empty);
            }

            pthread_mutex_unlock(&connection_mutex);
        }
    }

    close(listen_socket_tcp);
    close(listen_socket_udp);
    return NULL;
}

int client_queue_is_empty() {
    return queue_size == 0;
}

struct free_client get_next_client() {
    struct free_client client = client_queue[queue_start];
    queue_start = (queue_start + 1) % N;
    queue_size--;
    return client;
}

void *handle_connections(void *arg) {
    while (1) {
        pthread_mutex_lock(&connection_mutex);

        while (client_queue_is_empty()) {
            pthread_cond_wait(&queue_not_empty, &connection_mutex);
        }

        struct free_client client = get_next_client();

        pthread_mutex_unlock(&connection_mutex);

        if (client.type_connection == 1) { // TCP
            // Обрабатываем TCP соединение
            send(client.socket_fd, "Hello, TCP client!", 18, 0);
            close(client.socket_fd);
        } else if (client.type_connection == 0) { // UDP
            // Обрабатываем UDP соединение
            int udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
            if (udp_socket < 0) {
                perror("ошибка создания UDP сокета");
                continue;
            }
            sendto(udp_socket, "Hello, UDP client!", 18, 0, (struct sockaddr*)&client.udp_addr, sizeof(client.udp_addr));
            close(udp_socket);
        }
    }

    return NULL;
}

int main() {
    pthread_t listen_thread;
    pthread_t handle_threads[N];

    if (pthread_create(&listen_thread, NULL, &listen_func, NULL) != 0) {
        perror("ошибка создания потока прослушивания");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < N; i++) {
        if (pthread_create(&handle_threads[i], NULL, &handle_connections, NULL) != 0) {
            perror("ошибка создания потока обработки соединений");
            exit(EXIT_FAILURE);
        }
    }

    pthread_join(listen_thread, NULL);
    for (int i = 0; i < N; i++) {
        pthread_join(handle_threads[i], NULL);
    }

    pthread_mutex_destroy(&connection_mutex);
    pthread_cond_destroy(&queue_not_empty);

    return 0;
}
