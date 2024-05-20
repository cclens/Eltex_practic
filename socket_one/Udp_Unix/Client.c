#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>

int main() {
    char buffer[1024];
    char message[] = "hello from client!";
    int client_socket = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (client_socket < 0) {
        perror("ошибка создания сокета");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_un server_addr, client_addr;

    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, "server_socket_udp");

    client_addr.sun_family = AF_UNIX;
    strcpy(client_addr.sun_path, "client_socket_udp");

    if (bind(client_socket, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0) {
        perror("ошибка привязки сокета клиента");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    if (sendto(client_socket, message, strlen(message), 0, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("ошибка отправки данных");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    socklen_t server_addr_len = sizeof(server_addr);
    int bytes_received = recvfrom(client_socket, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&server_addr, &server_addr_len);
    if (bytes_received < 0) {
        perror("ошибка приёма данных");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    buffer[bytes_received] = '\0'; // Завершить строку
    printf("Получено сообщение от сервера: %s\n", buffer);

    close(client_socket);
    return 0;
}
