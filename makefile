CPP = g++
CC  = gcc
OBJ = main.o UnAlz.o UnAlzBz2decompress.o UnAlzBzip2.o UnAlzbzlib.o zlib/adler32.o zlib/crc32.o zlib/infblock.o zlib/infcodes.o zlib/inffast.o zlib/inflate.o zlib/inftrees.o zlib/infutil.o zlib/zutil.o bzip2/blocksort.o bzip2/compress.o bzip2/crctable.o bzip2/huffman.o bzip2/randtable.o
LINKOBJ = main.o UnAlz.o UnAlzBz2decompress.o UnAlzBzip2.o UnAlzbzlib.o zlib/adler32.o zlib/crc32.o zlib/infblock.o zlib/infcodes.o zlib/inffast.o zlib/inflate.o zlib/inftrees.o zlib/infutil.o zlib/zutil.o bzip2/blocksort.o bzip2/compress.o bzip2/crctable.o bzip2/huffman.o bzip2/randtable.o
BIN  = unalz
CFLAGS =

all: unalz

clean:
	rm -f $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o "unalz"

main.o: main.cpp
	$(CPP) -c main.cpp -o main.o

UnAlz.o: UnAlz.cpp
	$(CPP) -c UnAlz.cpp -o UnAlz.o

UnAlzBz2decompress.o: UnAlzBz2decompress.c
	$(CC) -c UnAlzBz2decompress.c -o UnAlzBz2decompress.o $(CFLAGS)

UnAlzBzip2.o: UnAlzBzip2.cpp
	$(CPP) -c UnAlzBzip2.cpp -o UnAlzBzip2.o

UnAlzbzlib.o: UnAlzbzlib.c
	$(CC) -c UnAlzbzlib.c -o UnAlzbzlib.o $(CFLAGS)

zlib/adler32.o: zlib/adler32.c
	$(CC) -c zlib/adler32.c -o zlib/adler32.o $(CFLAGS)

zlib/crc32.o: zlib/crc32.c
	$(CC) -c zlib/crc32.c -o zlib/crc32.o $(CFLAGS)

zlib/infblock.o: zlib/infblock.c
	$(CC) -c zlib/infblock.c -o zlib/infblock.o $(CFLAGS)

zlib/infcodes.o: zlib/infcodes.c
	$(CC) -c zlib/infcodes.c -o zlib/infcodes.o $(CFLAGS)

zlib/inffast.o: zlib/inffast.c
	$(CC) -c zlib/inffast.c -o zlib/inffast.o $(CFLAGS)

zlib/inflate.o: zlib/inflate.c
	$(CC) -c zlib/inflate.c -o zlib/inflate.o $(CFLAGS)

zlib/inftrees.o: zlib/inftrees.c
	$(CC) -c zlib/inftrees.c -o zlib/inftrees.o $(CFLAGS)

zlib/infutil.o: zlib/infutil.c
	$(CC) -c zlib/infutil.c -o zlib/infutil.o $(CFLAGS)

zlib/zutil.o: zlib/zutil.c
	$(CC) -c zlib/zutil.c -o zlib/zutil.o $(CFLAGS)

bzip2/blocksort.o: bzip2/blocksort.c
	$(CC) -c bzip2/blocksort.c -o bzip2/blocksort.o $(CFLAGS)

bzip2/compress.o: bzip2/compress.c
	$(CC) -c bzip2/compress.c -o bzip2/compress.o $(CFLAGS)

bzip2/crctable.o: bzip2/crctable.c
	$(CC) -c bzip2/crctable.c -o bzip2/crctable.o $(CFLAGS)

bzip2/huffman.o: bzip2/huffman.c
	$(CC) -c bzip2/huffman.c -o bzip2/huffman.o $(CFLAGS)

bzip2/randtable.o: bzip2/randtable.c
	$(CC) -c bzip2/randtable.c -o bzip2/randtable.o $(CFLAGS)

