CC=gcc
CFLAGS=-Wall -g


build: main.c utils.h
	$(CC) $(CFLAGS) main.c -o tema1

run: build
	./tema1

clean:
	rm -f *.o exec

archive:
	zip -r tema1.zip Makefile README *.c *.h

valgrind:
	valgrind --leak-check=yes --track-origins=yes ./tema1