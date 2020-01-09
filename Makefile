all: partie1 partie2
CC=gcc
CFLAGS=-Wall -Werror



main1.o: main1.c read_func.h
	$(CC) $(CFLAGS) -c main1.c

main2.o: main2.c read_func.h fusion.h
	$(CC) $(CFLAGS) -c main2.c

read_func.o: read_func.c read_func.h read.h print.h
	$(CC) $(CFLAGS) -c read_func.c

print.o: print.c print.h read_func.h read.h
	$(CC) $(CFLAGS) -c print.c
	
fusion.o: fusion.c fusion.h read_func.h read.h
	$(CC) $(CFLAGS) -c fusion.c

read.o: read.c read.h 
	$(CC) $(CFLAGS) -c read.c



partie1: main1.o read_func.o read.o print.o
	$(CC) $(CFLAGS) -o partie1 main1.o read_func.o read.o print.o

partie2: main2.o read_func.o fusion.o read.o print.o
	$(CC) $(CFLAGS) -o partie2 main2.o read_func.o fusion.o read.o print.o



clean:
	rm *.o partie1 partie2
