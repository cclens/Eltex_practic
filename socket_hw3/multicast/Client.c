#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h> // Для структуры sockaddr_in
#include <arpa/inet.h>
#define MULTICAST_GROUP "224.0.0.1"


#define PORT 7777

int main() {

    int client_socket = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in client_addr, server_addr;
	struct ip_mreq mreq;
	socklen_t server_addr_len = sizeof(server_addr);

	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(PORT);
	client_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if (bind (client_socket, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0){
		perror("bind");
		exit(EXIT_FAILURE);
	}
	
	mreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_GROUP);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	
	if (setsockopt(client_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }	

	while(1){
	
		char buffer[64];
		int bytes_recived = recvfrom(client_socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&server_addr, &server_addr_len) < 0;
		if (bytes_recived < 0){
			perror("recvfrom");
			exit(EXIT_FAILURE);
		}
	
		printf("%s\n", buffer);		

		close(client_socket);
		exit(EXIT_SUCCESS);
	}
}



