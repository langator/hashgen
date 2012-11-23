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
	cp doc/hashgen.1 /usr/share/man/man1/hashgen.1
	bzip2 /usr/share/man/man1/hashgen.1

clean:
	rm hashgen

