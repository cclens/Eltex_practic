all: Server Client


Server: Server.o
	gcc -o Server Server.o -lrt

Server.o: Server.c
	gcc -c Server.c

Client: Client.o
	gcc -o Client Client.o -lrt

Client.o: Client.c
	gcc -c Client.c

