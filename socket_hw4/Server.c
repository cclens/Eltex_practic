#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h> // Для структуры sockaddr_in
#include <arpa/inet.h>
#include <netinet/udp.h>

#define PORT_SRC 7777
#define PORT_DST 8888
int main() {

    int server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket < 0){
            perror("server_socket");
            exit(EXIT_FAILURE);
    }

        char ip_addr[] = "127.0.0.5";
        struct sockaddr_in client_addr, server_addr;
    	socklen_t addr_len = sizeof(client_addr);

        server_addr.sin_port = htons(PORT_SRC);
        server_addr.sin_family = AF_INET;

		client_addr.sin_port = htons(PORT_DST);

        if (inet_pton(AF_INET, ip_addr, &server_addr.sin_addr) < 0){
                perror("inet_pton");
                exit(EXIT_FAILURE);
        }

        if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
                perror("bind");
                exit(EXIT_FAILURE);
        }


    char buffer[256];
    int bytes_recived = recvfrom(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &addr_len);
    
    if (bytes_recived < 0) {
        perror("recvfrom");
        exit(EXIT_FAILURE);
    }

    buffer[bytes_recived] = '\0';
    printf("Received packet from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    printf("Data: %s\n", buffer);      
    strcpy(buffer, "hello client");
	sleep(2);
    if (sendto(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0){
        perror("sendto");
        exit(EXIT_FAILURE);
    }
	sleep(1);

}
