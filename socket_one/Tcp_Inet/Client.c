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

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0){
        perror("ошибка создания сокета");
		exit(EXIT_FAILURE);
    }
	
	struct sockaddr_in server_addr;
	int addr_len = (sizeof(server_addr));
	char buffer[128];
	char message[] = "hello_world1";

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);

	if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) < 0){
		perror("ошибка задания адреса(ip)");
		exit(EXIT_FAILURE);
	}

	if ((connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr))) < 0){
		perror("ошибка подключения(connect)");
		exit(EXIT_FAILURE);
	}
	printf("соединение установленно\n");	

	if (send(client_socket, message, sizeof(message), 0) < 0){
		perror("ошибка отправки сообщения");
		exit(EXIT_FAILURE);
	}
	printf("ожидание сообщения от серверa\n");
	int bytes_recived = recv(client_socket, buffer, sizeof(buffer), 0);
	printf("%s%s\n", "сообщение от пользователя:", buffer);

	close(client_socket);
}







	
	

