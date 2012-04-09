CC = gcc

all: aeroglass

aeroglass: aeroglass.o
	$(CC) -o aeroglass aeroglass.o -lpsapi 

aeroglass.o: clean
	$(CC) -c aeroglass.c

clean: aeroglass.c
	rm -f aeroglass aeroglass.o
