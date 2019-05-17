CC=gcc
CFLAGS=-std=c99
DBFLAGS=-g

make: btree.c 
	$(CC) $(CFLAGS) -o myBashProj myBashProj.c

debug: btree.c
	$(CC) $(CFLAGS) $(DBFLAGS) -o myBashProj myBashProj.c

clean: 
	rm myBashProj