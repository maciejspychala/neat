CFLAGS=-Wall -Wextra -Werror -std=c11

all: neat

neat: net.o
	gcc -o neat src/main.c *.o

net.o:
	gcc -c src/net.c

clean:
	rm *.o
	rm neat
