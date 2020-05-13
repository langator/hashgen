CFLAGS=
TARGET = hashgen
HASHGEN_PATH_INSTALL= /usr/bin

.PHONY: all clean install uninstall

all:
	$(CC) $(CFLAGS) -O3 -o ${TARGET} -lmhash main.c

install:
	cp ${TARGET} ${HASHGEN_PATH_INSTALL}
	cp doc/hashgen.1 /usr/share/man/man1/hashgen.1
	bzip2 -f /usr/share/man/man1/hashgen.1

clean:
	rm ${TARGET}

uninstall:
	rm ${HASHGEN_PATH_INSTALL}/${TARGET}
	rm /usr/share/man/man1/hashgen.1.bz2
