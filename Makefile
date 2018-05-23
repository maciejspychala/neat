CFLAGS=-Wall -Wextra -Werror -std=c11

all: neat

neat: net.o src/main.c
	gcc -o neat src/main.c *.o

net.o: list.o src/net.c
	gcc -c src/net.c

list.o: src/list.c
	gcc -c src/list.c

clean:
	rm *.o
	rm neat
