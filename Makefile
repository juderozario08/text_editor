CC = gcc

all:
	gcc client.c -o client -lwayland-client
	chmod u+x client
	./client
