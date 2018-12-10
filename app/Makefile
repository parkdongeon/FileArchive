CC = gcc
AR_HEADER = -I../include/archive
CO_HEADER = -I../include/compress
AR_PATH = -L../lib/archive
CO_PATH = -L../lib/compress

all: MyArchive MyCompress

MyArchive: ../lib/archive/libarchive.a
	$(CC) -o MyArchive MyArchive.c $(AR_HEADER) $(AR_PATH) -larchive

MyCompress: ../lib/compress/libcompress.a
	$(CC) -o MyCompress MyCompress.c $(CO_HEADER) $(CO_PATH) -lcompress

clean:
	rm -f *.o *.a
	rm MyArchive
	rm MyCompress
