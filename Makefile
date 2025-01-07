CC = gcc

all:
	gcc -o server server.c -lwayland-server
	gcc -o client client.c -lwayland-client
