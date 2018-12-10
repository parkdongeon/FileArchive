CC = gcc
HEADER = -I../include/archive
LIB_PATH = -L../lib/archive

MyArchive: ../lib/archive/libarchive.a
	$(CC) -o MyArchive MyArchive.c $(HEADER) $(LIB_PATH) -larchive

clean:
	rm -f *.o *.a
	rm MyArchive
