

Beta_mc: main.o graphics.o navigation.o 
	gcc -o Beta_mc main.o graphics.o navigation.o -g -lncurses
    
main.o : main.c
	gcc -c main.c

graphics.o : graphics.c
	gcc -c graphics.c

navigation.o : navigation.c
	gcc -c navigation.c
