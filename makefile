# Makefile for Laboration 1 5DV088 HT19
# Max Malmer, c17mmr@cs.umu.se
# Version 1.0, 2019-09-12

CC = gcc
CFLAGS = -std=gnu11 -g -lm -Wall -Wextra -Werror -Wmissing-declarations -Wmissing-prototypes -Werror-implicit-function-declaration -Wreturn-type -Wparentheses -Wunused -Wold-style-definition -Wundef -Wshadow -Wstrict-prototypes -Wswitch-default -Wunreachable-code

all:			mpasswdsort

mpasswdsort:	mpasswdsort.o list.o
	$(CC) -lm list.o mpasswdsort.o -o mpasswdsort

mpasswdsort.o:	mpasswdsort.c mpasswdsort.h list.h
	$(CC) $(CFLAGS) -lm -c mpasswdsort.c

list.o: 		list.c list.h
	$(CC) $(CFLAGS) -c list.c

run:			mpasswdsort
	./mpasswdsort pass.txt

valgrind: 		mpasswdsort
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./mpasswdsort pass.txt

clean:
	rm -f mpasswdsort *.o core
