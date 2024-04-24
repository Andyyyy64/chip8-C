CFLAGS=-std=c17 -Wall -Wextra -Werror
CC=gcc

all:
	$(CC) main.c -o main $(CFLAGS) `sdl2-config --cflags --libs`
clean:
	rm -f main.c main.o
