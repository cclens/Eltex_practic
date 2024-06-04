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

	int server_socket = socket(AF_INET, SOCK_DGRAM, 0);
	char ip_addr[] = "255.255.255.255";
	int flag = 1;

	if (server_socket < 0){
		perror("socket");
		exit(EXIT_FAILURE);
	}
	
	struct sockaddr_in client_addr;
	
	client_addr.sin_port = htons(PORT);
	client_addr.sin_family = AF_INET;
	
	if (inet_pton(AF_INET, ip_addr, &client_addr.sin_addr) < 0){
		perror("inet_pton");
		exit(EXIT_FAILURE);
	}

	if (setsockopt(server_socket, SOL_SOCKET, SO_BROADCAST, &flag, sizeof(flag)) < 0){
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	char message[] = "hello world";
	if (sendto(server_socket, message, sizeof(message), 0, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0){
		perror("sendto");
		exit(EXIT_FAILURE);
	}	


}
