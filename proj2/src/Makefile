CC = afl-gcc
CFLAGS = -g -w -O3

all: libpngparser size

clean:
	rm -f libpngparser.a size *.o

.PHONY: all clean fix_all_bugs tests

libpngparser: pngparser.h pngparser.c crc.c crc.h
	$(CC) -fsanitize=address -g -c pngparser.c crc.c
	ar rcs libpngparser.a pngparser.o crc.o


size: libpngparser size.c
	$(CC) -fsanitize=address -g -o size size.c libpngparser.a -lz

