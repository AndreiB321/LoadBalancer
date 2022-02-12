# Copyright 2021 <Andrei Bulica>
CC=gcc
CFLAGS=-std=c99 -Wall -Wextra
LOAD=load_balancer
SERVER=server
LL=linked_list
HELP=help

.PHONY: build clean

build: tema2

tema2: main.o $(LOAD).o $(SERVER).o $(LL).o $(HELP).o
	$(CC) $^ -o $@

main.o: main.c
	$(CC) $(CFLAGS) $^ -c

$(LL).o: $(LL).c $(LL).h
	$(CC) $(CFLAGS) $^ -c

$(SERVER).o: $(SERVER).c $(SERVER).h
	$(CC) $(CFLAGS) $^ -c

$(LOAD).o: $(LOAD).c $(LOAD).h
	$(CC) $(CFLAGS) $^ -c

$(HELP).o: $(HELP).c $(HELP).h
	$(CC) $(CFLAGS) $^ -c

clean:
	rm -f *.o tema2 *.h.gch
