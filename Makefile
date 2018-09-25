CFLAGS=-Wall -Wextra -Werror -std=c11 -I./include/
CC=gcc $(CFLAGS)


all: neat

neat: net.o src/main.c
	$(CC) -o neat src/main.c *.o

net.o: list.o gene.o genome.o node.o src/net.c
	$(CC) -c src/net.c

genome.o: list.o gene.o src/genome.c
	$(CC) -c src/genome.c

list.o: src/list.c
	$(CC) -c src/list.c

gene.o: src/gene.c
	$(CC) -c src/gene.c

node.o: src/node.c
	$(CC) -c src/node.c

clean:
	rm *.o
	rm neat
