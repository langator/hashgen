###
###
###
CC=gcc
CFLAGS=
all:
	$(CC) $(CFLAGS) -o hashgen -lmhash -lcrypt main.c

