all: shell.c util.o build
	gcc shell.c -o shell util.o

util.o: util.c util.h
	gcc -c util.c

build: grep du
	echo "Build completed"

grep: grep.c util.o
	gcc grep.c -o grep util.o

du: du.c
	gcc du.c -o du

clean:
	rm -f *.o shell grep du

cleantxt:
	rm -f *.o shell grep du *txt
