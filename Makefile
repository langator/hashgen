###
###
###
CC=gcc
CFLAGS=
all:
	$(CC) $(CFLAGS) -O3 -o hashgen -lmhash main.c

clean:
	rm hashgen

