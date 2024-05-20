#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> // Для структуры sockaddr_in
#include <arpa/inet.h>

#define PORT 12345

int main() {
    int client_socket = socket(AF_INET, SOCK_DGRAM, 0); // Используем AF_INET вместо AF_UNIX
    if (client_socket < 0) {
        perror("ошибка создания сокета (socket)");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr)); // Инициализируем структуру нулями

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("ошибка привязки IP адреса для структуры сервера (inet_pton)");
        exit(EXIT_FAILURE);
    }

    char message[] = "hello world1";
    if (sendto(client_socket, message, strlen(message), 0, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("ошибка отправки сообщения (sendto)");
        exit(EXIT_FAILURE);
    }

    char buffer[128];
    socklen_t server_addr_len = sizeof(server_addr); // Длина структуры адреса сервера
    int bytes_received = recvfrom(client_socket, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&server_addr, &server_addr_len);
    if (bytes_received < 0) {
        perror("ошибка принятия сообщения (recvfrom)");
        exit(EXIT_FAILURE);
    }

    buffer[bytes_received] = '\0'; // Нулевой символ в конце строки
    printf("получено сообщение: %s\n", buffer);

    close(client_socket);
    return 0;
}
