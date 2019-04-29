CC = gcc
CFLAGS = -std=c99 -g -Wall -Wextra -Wpedantic
LDFLAGS = -lpthread

all: philos philos2

philos: philosophers.c monitor.h monitor.c
	$(CC) $(CFLAGS) -o philos philosophers.c monitor.c $(LDFLAGS)

philos2: philosophers-sem.c
	$(CC) $(CFLAGS) -o philos2 philosophers-sem.c $(LDFLAGS)

clean:
	rm -vf philos philos2
