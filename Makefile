all: p1 p2
CC=gcc
CFLAGS=-Wall -Werror



main1.o: main1.c utils.h
	$(CC) $(CFLAGS) -c main1.c

main2.o: main2.c utils.h fusion.h
	$(CC) $(CFLAGS) -c main2.c



utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

fusion.o: fusion.c fusion.h
	$(CC) $(CFLAGS) -c fusion.c



p1: main1.o utils.o 
	$(CC) $(CFLAGS) -o p1 main1.o utils.o

p2: main2.o utils.o fusion.o
	$(CC) $(CFLAGS) -o p2 main2.o utils.o fusion.o



clean:
	rm *.o p1 p2
