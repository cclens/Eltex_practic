#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT_UDP 4444

int main() {
    int client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket < 0) {
        perror("ошибка создания сокета (socket)");
        exit(EXIT_FAILURE);
    }

    char ip_addr_server[] = "127.0.0.2";
    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT_UDP);

    if (inet_pton(AF_INET, ip_addr_server, &server_addr.sin_addr) <= 0) {
        perror("ошибка преобразования ip (inet_pton)");
        exit(EXIT_FAILURE);
    }

    char message[] = "Hello, UDP server!";
    if (sendto(client_socket, message, strlen(message), 0, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("ошибка отправки сообщения (sendto)");
        exit(EXIT_FAILURE);
    }

    printf("Сообщение отправлено серверу: %s\n", message);

    char buffer[1024];
    socklen_t addr_len = sizeof(server_addr);
    int recv_len = recvfrom(client_socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&server_addr, &addr_len);
    if (recv_len < 0) {
        perror("ошибка получения ответа (recvfrom)");
        exit(EXIT_FAILURE);
    }

    buffer[recv_len] = '\0'; // Завершаем строку
    printf("Ответ от сервера: %s\n", buffer);

    close(client_socket);
    return 0;
}
