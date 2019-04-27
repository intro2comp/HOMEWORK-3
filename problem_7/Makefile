CC = gcc
CFLAGS = -std=c99 -g -Wall -Wextra -Wpedantic
LDFLAGS = -lpthread


all: taxis

clean:
	rm -vf taxis

taxis: main.c
	${CC} ${CFLAGS} main.c -o taxis ${LDFLAGS}