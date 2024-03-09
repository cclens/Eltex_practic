
	
Reverse_string: test.o
	gcc -o Reverse_string test.o

test.o : test.c
	gcc -c test.c
