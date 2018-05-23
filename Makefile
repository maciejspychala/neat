CFLAGS=-Wall -Wextra -Werror -std=c11

all: neat

neat: net.o
	gcc -o neat src/main.c *.o

net.o: list.o
	gcc -c src/net.c

list.o:
	gcc -c src/list.c

clean:
	rm *.o
	rm neat
