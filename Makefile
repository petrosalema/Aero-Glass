CC = gcc

all: aeroglass

aeroglass: aeroglass.o
	$(CC) -o aeroglass aeroglass.o -lpsapi 

aeroglass.o: clean
	$(CC) -Wall -c aeroglass.c

clean: aeroglass.c
	rm -f aeroglass aeroglass.o
