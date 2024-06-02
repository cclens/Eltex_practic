#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h> // Для структуры sockaddr_in
#include <arpa/inet.h>

#define PORT_UDP 4444
#define PORT_TCP 1111
int main(){

        int client_socket = socket(AF_INET, SOCK_STREAM, 0);

        if (client_socket < 0){
                perror("ошибка привязки сокета(bind tcp)");
                exit(EXIT_FAILURE);
        }

        char ip_addr_client[] = "127.0.0.5";
		char ip_addr_server[] = "127.0.0.2";
        struct sockaddr_in client_addr, server_addr;

        client_addr.sin_family = AF_INET;
        server_addr.sin_family = AF_INET;
        client_addr.sin_port = htons(PORT_TCP);
        server_addr.sin_port = htons(PORT_TCP);
		
		
        
        if (inet_pton(AF_INET, ip_addr_client, &client_addr.sin_addr) < 0){
                perror("ошибка преобразования ip(inet_pton client)");
                exit(EXIT_FAILURE);
        }
        
        if (inet_pton(AF_INET, ip_addr_server, &server_addr.sin_addr) < 0){
                perror("ошибка преобразования ip(inet_pton pool)");
                exit(EXIT_FAILURE);
        }
        
        if (bind(client_socket, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0){
                perror("ошибка привязки ip + port(bind)");
                exit(EXIT_FAILURE);
        }
        

        if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
                perror("ошибка подключения (connect)");
                exit(EXIT_FAILURE);
        }
        printf("соединение успешно установленно\n");
        char service_message[] = "1";

        close(client_socket);
}

