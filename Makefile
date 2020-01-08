all: p1 p2
CC=gcc
CFLAGS=-Wall -Werror



main1.o: main1.c utils.h
	$(CC) $(CFLAGS) -c main1.c

main2.o: main2.c utils.h fusion.h
	$(CC) $(CFLAGS) -c main2.c



utils.o: utils.c utils.h es.h
	$(CC) $(CFLAGS) -c utils.c

fusion.o: fusion.c fusion.h utils.h es.h
	$(CC) $(CFLAGS) -c fusion.c

es.o: es.c es.h 
	$(CC) $(CFLAGS) -c es.c



p1: main1.o utils.o es.o
	$(CC) $(CFLAGS) -o p1 main1.o utils.o es.o

p2: main2.o utils.o fusion.o es.o
	$(CC) $(CFLAGS) -o p2 main2.o utils.o fusion.o es.o



clean:
	rm *.o p1 p2
