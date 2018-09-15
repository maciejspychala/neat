CFLAGS=-Wall -Wextra -Werror -std=c11

all: neat

neat: net.o src/main.c
	gcc -o neat src/main.c *.o

net.o: list.o gene.o genome.o node.o src/net.c
	gcc -c src/net.c

genome.o: list.o gene.o src/genome.c
	gcc -c src/genome.c

list.o: src/list.c
	gcc -c src/list.c

gene.o: src/gene.c
	gcc -c src/gene.c

node.o: src/node.c
	gcc -c src/node.c

clean:
	rm *.o
	rm neat
