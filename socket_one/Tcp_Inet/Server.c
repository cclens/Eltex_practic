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

	int server_socket = socket(AF_INET, SOCK_STREAM, 0);	
	if (server_socket < 0){
		perror("ошибка создания сокета");
		exit(EXIT_FAILURE);	
	}
	
	struct sockaddr_in server_addr, client_addr;
	int addr_len = sizeof(server_addr);
	char buffer[128];

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);

	if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) < 0){
		perror("ошибка задания ip адресса");
		exit(EXIT_FAILURE);
	}
	
	if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
		perror("ошибка создания endpoint servera");
		exit(EXIT_FAILURE);		
	}
	
	if ((listen(server_socket, 3)) < 0){
		perror("ошибка прослушивания(listen)");
		close(server_socket);	
		exit(EXIT_FAILURE);
	}
	int new_socket = accept(server_socket, (struct sockaddr *)&client_addr, (socklen_t *)&addr_len);

	if (new_socket < 0){
		perror("ошибка принятия сообщения(Accept)");
		close(server_socket);
		exit(EXIT_FAILURE);
	}
	printf("ожидается сообщение от пользователя...\n");
	int bytes_recived = recv(new_socket, buffer, sizeof(buffer), 0);
	printf("%s%s\n", "сообщение от пользователя:", buffer);
	
	char message[] = "hello world2";

	if (send(new_socket, message, sizeof(message), 0) < 0){
		perror("ошибка отправки сообщения");
		exit(EXIT_FAILURE);
	}
	
	close(new_socket);
	close(server_socket);

}
