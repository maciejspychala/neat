CFLAGS=-Wall -Wextra -Werror -std=c11

all: neat

neat: net.o src/main.c
	gcc -o neat src/main.c *.o

net.o: list.o gene.o src/net.c
	gcc -c src/net.c

list.o: src/list.c
	gcc -c src/list.c

gene.o: src/gene.c
	gcc -c src/gene.c

clean:
	rm *.o
	rm neat
