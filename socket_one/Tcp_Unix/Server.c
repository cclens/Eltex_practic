#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

int main() {
    char message[] = "hello world!";
    char buffer[1024];
	int server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Ошибка создания сокета");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_un server_addr, client_addr;
    socklen_t client_addr_len;

    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, "server_socket");
    
	if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Ошибка при привязке сокета");
        exit(EXIT_FAILURE);
    }

    listen(server_socket, 5);
    printf("Сервер ждет подключения\n");

    client_addr_len = sizeof(client_addr);
    int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
    if (client_socket < 0) {
        perror("Ошибка при принятии соединения");
        exit(EXIT_FAILURE);
    }
	
	if(send(client_socket, message, strlen(message), 0) < 0){
		perror("ошибка при отправке сообщения");
		exit(EXIT_FAILURE);
	}	

	int bytes_recived = recv(client_socket, buffer, sizeof(buffer), 0);
    buffer[bytes_recived] = '\0';
	printf("%s\n", buffer);
	close(client_socket);
    close(server_socket);


    return 0;
}
