all: Server Client


Server: Server.o
	gcc -o Server Server.o -lpthread

Server.o: Server.c
	gcc -c Server.c

Client: Client.o
	gcc -o Client Client.o -lpthread -lncurses

Client.o: Client.c
	gcc -c Client.c

