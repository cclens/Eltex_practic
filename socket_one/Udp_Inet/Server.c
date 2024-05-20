#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h> // Для структуры sockaddr_in
#include <arpa/inet.h>


#define PORT 12345

int main() {

	int server_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (server_socket < 0){ 
 		perror("ошибка создания сокета");
       	exit(EXIT_FAILURE);
    }
	
	char message[] = "hello world2";
	char buffer[128];
	struct sockaddr_in server_addr, client_addr;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_family = AF_INET;
	socklen_t client_addr_len = sizeof(client_addr);
	if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) < 0 ){
		perror("Ошибка привязки ip(inet_pton)");
		exit(EXIT_FAILURE);
	}

	if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
		perror("ошибка привязки сокета");
		exit(EXIT_FAILURE);
	}
	
	printf("Сервер запущен и ожидает сообщений...\n");
	
	while(1){

	int bytes_recived = recvfrom(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &client_addr_len);	
	if (bytes_recived < 0){
		perror("ошибка принятия данных(recvfrom)");
		exit(EXIT_FAILURE);
	}
	
	printf("%s%s\n", "полученно сообщение: ", buffer);	
	if (sendto(server_socket, message, sizeof(message), 0, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0){
		perror("ошибка отправки сообщения(sendto)");
		exit(EXIT_FAILURE);
	}

	}
	close(server_socket);
}

