#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>

int main() {
    char buffer[1024];
    char message[] = "hello world!";
    int server_socket = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (server_socket < 0) {
        perror("ошибка создания сокета");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_un server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, "server_socket_udp");

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("ошибка привязки сокета");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Сервер запущен и ожидает сообщений...\n");

    while (1) {
        int bytes_received = recvfrom(server_socket, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&client_addr, &client_addr_len);

        if (bytes_received < 0) {
            perror("ошибка приёма данных");
            close(server_socket);
            exit(EXIT_FAILURE);
        }

        buffer[bytes_received] = '\0'; 
        printf("Получено сообщение: %s\n", buffer);

        if (client_addr.sun_path[0] == '\0') {
            fprintf(stderr, "Ошибка: клиентский адрес пуст\n");
        } else {
            printf("Клиентский адрес: %s\n", client_addr.sun_path);
        }

        if (sendto(server_socket, message, strlen(message), 0, (struct sockaddr*)&client_addr, client_addr_len) < 0) {
            perror("ошибка отправки данных");
            close(server_socket);
            exit(EXIT_FAILURE);
        }
    }

    close(server_socket);
    return 0;
}
