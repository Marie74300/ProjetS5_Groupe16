all: main
CC=gcc
CFLAGS=-Wall -Werror

main.o: main.c utils.h fusion.h
	$(CC) $(CFLAGS) -c main.c


utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

fusion.o: fusion.c fusion.h
	$(CC) $(CFLAGS) -c fusion.c

main: main.o utils.o fusion.o
	$(CC) $(CFLAGS) -o main main.o utils.o fusion.o

clean:
	rm *.o main
