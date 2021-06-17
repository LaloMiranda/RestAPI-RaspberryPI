CFLAGS = -g -pedantic -Werror -Wextra -Wconversion -std=gnu11
CC = gcc
ULFIUS = -lulfius -ljansson

all: clean main

main:
	$(CC) $(CFLAGS) ./src/main.c -o ./bin/main.o $(ULFIUS) 

clean: 
	rm -f ./bin/*.o