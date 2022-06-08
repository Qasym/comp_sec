CC = clang-11
override CFLAGS += -g -w -O3 -fsanitize=fuzzer,address

all: libpngparser fuzzer_load_png fuzzer_load_png_name

clean:
	rm -f libpngparser.a fuzzer_load_png *.o fuzzer_load_png_name fuzzer_store_png_rgba fuzzer_store_png_palette fuzzer_store_png_name

.PHONY: all clean

libpngparser: pngparser.h pngparser.c crc.c crc.h
	$(CC) $(CFLAGS) -c pngparser.c crc.c
	ar rcs libpngparser.a pngparser.o crc.o

fuzzer_load_png:
	$(CC) $(CFLAGS) -o fuzzer_load_png fuzzer_load_png.c libpngparser.a -lz

fuzzer_load_png_name: libpngparser
	$(CC) $(CFLAGS) -o fuzzer_load_png_name fuzzer_load_png_name.c libpngparser.a -lz

fuzzer_store_png_rgba: libpngparser
# Fuzzes store_png RGBA file interface (no palette).
	$(CC) $(CFLAGS) -o fuzzer_store_png_rgba fuzzer_store_png_rgba.c libpngparser.a -lz

fuzzer_store_png_palette: libpngparser
# Fuzzes store_png palette file interface (fixed palette).
	$(CC) $(CFLAGS) -o fuzzer_store_png_palette fuzzer_store_png_palette.c libpngparser.a -lz

fuzzer_store_png_name: libpngparser
# Fuzzes the filename argument of store_png.
	$(CC) $(CFLAGS) -o fuzzer_store_png_name fuzzer_store_png_name.c libpngparser.a -lz
