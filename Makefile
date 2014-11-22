CC=icc

all: main

main: main.c
	$(CC) $^ -o $@