all: main
CC=gcc
CFLAGS=-Wall -Werror

main.o: main.c utils.h
	$(CC) $(CFLAGS) -c main.c


utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

main: main.o utils.o
	$(CC) $(CFLAGS) -o main main.o utils.o

clean:
	rm *.o main
