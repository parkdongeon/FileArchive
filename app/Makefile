CC = gcc
HEADER = -I../include/archive
PATH = -L../lib/archive

MyArchive: ../include/archive
	$(CC) -o MyArchive -c MyArchive.c $(HEADER) $(PATH) -larchive

clean:
	rm -f *.o *.a
	rm MyArchive
