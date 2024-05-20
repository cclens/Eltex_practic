#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
	
	
	char buffer[] = "Hi!";
	mkfifo ("pipe1", 0666);
	int fd = open("pipe1", O_WRONLY);
	
	write(fd, buffer, sizeof(buffer));
	close (fd);

}
