CC      = gcc
CFLAGS  = -Wall -pedantic -g -std=c99

.SUFFIXES: .c .o .h

.PHONY: all clean

all: main

map.o: map.c map.h

exam.o: exam.c exam.h map.h

main.o: exam.h main.c map.h

main: main.o exam.o map.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o
	rm -f main
