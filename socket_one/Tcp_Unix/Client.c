#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>
#include <stdio.h>

int main(){
	char buffer[1024];
	char message[] = "lovi";
	struct sockaddr_un server_addr;
	int client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
	
	server_addr.sun_family = AF_UNIX;
	strcpy(server_addr.sun_path, "server_socket");
	
	if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
		perror("ошибка при установке соединения");
		exit(EXIT_FAILURE);
	}
	
	if (send(client_socket, message, strlen(message), 0) < 0){
		perror("ошибка при отправке сообщения");
		exit(EXIT_FAILURE);
	}
	
	int bytes_recived = recv(client_socket, buffer, sizeof(buffer), 0);
	buffer[bytes_recived] = '\n'; 
	printf("%s\n", buffer);
	close(client_socket); 
}
