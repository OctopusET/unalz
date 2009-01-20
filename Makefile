CPP = g++
CC  = gcc
OBJ = main.o UnAlz.o UnAlzUtils.o UnAlzBz2decompress.o UnAlzBzip2.o UnAlzbzlib.o zlib/adler32.o zlib/crc32.o zlib/infback.o zlib/inffast.o zlib/inflate.o zlib/inftrees.o zlib/zutil.o bzip2/blocksort.o bzip2/compress.o bzip2/crctable.o bzip2/huffman.o bzip2/randtable.o
BIN = unalz
LDFLAGS = 
CFLAGS = -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 

all:
	@echo ""
	@echo "USAGE : make TARGET_SYSTEM"
	@echo ""
	@echo ""
	@echo "TARGET_SYSTEM is one of"
	@echo ""
	@echo " posix-utf8        : POSIX with utf8 filesystem(Most of modern OS, e.g. OSX/Ubuntu)"
	@echo " posix             : POSIX system (FreeBSD/linux/OSX/sparc/Win32)"
	@echo " linux-utf8        : LINUX with utf8 filesystem(without -liconv option)"
	@echo " posix-noiconv     : POSIX without libiconv (Windows(MINGW32,CYGWIN) or EUC-KR file system)"
	@echo ""
	@echo " 'install' for copy unalz to /usr/local/bin and  "
	@echo " 'clean' for clean"
	@echo ""

posix: unalz
	$(CPP) -c UnAlz.cpp -c UnAlzUtils.cpp -c main.cpp -D_UNALZ_ICONV $(CFLAGS) 
	$(CPP) $(OBJ) $(LDFLAGS) -liconv -o $(BIN) 

posix-utf8: unalz
	$(CPP) -c UnAlz.cpp -c UnAlzUtils.cpp -c main.cpp -D_UNALZ_ICONV -D_UNALZ_UTF8 $(CFLAGS)
	$(CPP) $(OBJ) $(LDFLAGS) -liconv -o $(BIN)

posix-noiconv: unalz
	$(CPP) -c UnAlz.cpp -c UnAlzUtils.cpp -c main.cpp $(CFLAGS)
	$(CPP) $(OBJ) $(LDFLAGS) -o $(BIN)

linux-utf8: unalz
	$(CPP) -c UnAlz.cpp -c UnAlzUtils.cpp -c main.cpp -D_UNALZ_ICONV -D_UNALZ_UTF8 $(CFLAGS)
	$(CPP) $(OBJ) $(LDFLAGS) -o $(BIN)

install:
	cp unalz /usr/local/bin/

clean:
	rm -f $(OBJ) $(BIN)

$(BIN): $(OBJ)


zlib/adler32.o: zlib/adler32.c
	$(CC) -c zlib/adler32.c -o zlib/adler32.o $(CFLAGS)

zlib/crc32.o: zlib/crc32.c
	$(CC) -c zlib/crc32.c -o zlib/crc32.o $(CFLAGS)

zlib/infback.o: zlib/infback.c
	$(CC) -c zlib/infback.c -o zlib/infback.o $(CFLAGS)

zlib/inffast.o: zlib/inffast.c
	$(CC) -c zlib/inffast.c -o zlib/inffast.o $(CFLAGS)

zlib/inflate.o: zlib/inflate.c
	$(CC) -c zlib/inflate.c -o zlib/inflate.o $(CFLAGS)

zlib/inftrees.o: zlib/inftrees.c
	$(CC) -c zlib/inftrees.c -o zlib/inftrees.o $(CFLAGS)

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

