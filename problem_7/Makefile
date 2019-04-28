CC = gcc
CFLAGS = -std=c99 -g -Wall -Wextra -Wpedantic
LDFLAGS = -lpthread


all: taxis

clean:
	rm -vf *.o
	rm -vf taxis

queue.o: queue.c queue.h
	${CC} ${CFLAGS} -c queue.c

main.o: main.c
	${CC} ${CFLAGS} -c main.c

taxis: queue.o main.o
	${CC} ${CFLAGS} queue.o main.o -o taxis ${LDFLAGS}