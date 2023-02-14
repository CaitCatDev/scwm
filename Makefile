.POSIX:
.SUFFIXES:
CC=clang
CFLAGS=-I./includes -Wall -Werror -pedantic -Wextra 
LDLIBS=-lxcb

scwm: scwm.o main.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS) 

scwm.o: src/scwm.c includes/scwm.h
	$(CC) $(CFLAGS) -c $<

main.o: src/main.c 
	$(CC) $(CFLAGS) -c $<
