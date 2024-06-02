all: Client Client1 Server Client3

Client: Client.o
	gcc -o Client Client.o

Client.o: Client.c
	gcc -c Client.c

Client1: Client1.o
	gcc -o Client1 Client1.o

Client1.o: Client1.c
	gcc -c Client1.c

Server: Server.o
	gcc -o Server Server.o

Server.o: Server.c
	gcc -c Server.c

Client3: Client3.o
	gcc -o Client3 Client3.o

Client3.o: Client3.c
	gcc -c Client3.c

clean:
	rm -f *.o
