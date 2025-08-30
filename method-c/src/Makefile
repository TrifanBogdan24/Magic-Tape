CC=gcc
CFLAGS=-Wall -g


build: tema-1.c tema-1.h
	$(CC) $(CFLAGS) tema-1.c -o tema1

run: build
	./tema1

clean:
	rm -f *.o exec

archive:
	zip -r tema1.zip Makefile README *.c *.h

valgrind:
	valgrind --leak-check=yes --track-origins=yes ./tema1