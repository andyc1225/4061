OPT_GCC = -std=c99 -Wall -Wextra
#compiler options and libraries for Linux
OPT = -D_XOPEN_SOURCE=700

all: clients server

clients: client1.c client2.c
	gcc $(OPT_GCC) $(OPT) -o client1 client1.c
	gcc $(OPT_GCC) $(OPT) -o client2 client2.c

server: server.c
	gcc $(OPT_GCC) $(OPT) -o server server.c

clean:
	rm -f client1 client2 server