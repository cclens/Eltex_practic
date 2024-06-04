#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h> // Для структуры sockaddr_in
#include <arpa/inet.h>


#define PORT 7777

int main() {

    int client_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (client_socket < 0){
		perror("socket");
		exit(EXIT_FAILURE);
	}
    char ip_addr[] = "255.255.255.255";
	struct sockaddr_in client_addr, server_addr;
	socklen_t server_addr_len = sizeof(server_addr);	

	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(PORT);
	
	if (inet_pton(AF_INET, ip_addr, &client_addr.sin_addr) < 0){
		perror("inet_pton");
		exit(EXIT_FAILURE);
	}
	
	if (bind(client_socket, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0){
		perror("bind");
		exit(EXIT_FAILURE);
	}

	while(1){
		char buffer[64];
		int bytes_recived = recvfrom(client_socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &server_addr_len);
		if (bytes_recived < 0){
			perror("bytes_recived");
			exit(EXIT_FAILURE);
		}
		
		printf("%s\n", buffer);
		close(client_socket);
		exit(EXIT_SUCCESS);
	}
}	





