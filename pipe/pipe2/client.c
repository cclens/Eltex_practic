#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
	char buffer[100];
	int fd = open("pipe1", O_RDONLY);
	sleep(1);
	read(fd, buffer, sizeof(buffer));
	printf("%s\n", buffer);
	close(fd);
}

