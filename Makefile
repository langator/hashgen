###
###
###
CC=gcc
CFLAGS=
HASHGEN_PATH_INSTALL="/usr/bin/"

all:
	$(CC) $(CFLAGS) -O3 -o hashgen -lmhash main.c

install:
	cp hashgen ${HASHGEN_PATH_INSTALL}

clean:
	rm hashgen

