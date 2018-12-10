CC = gcc
HEADER = -I../../lib/archive/
OBJFILES = archive.o command.o file.o

libarchive.a: $(OBJFILES)
	ar rv libarchive.a $(OBJFILES)

%.o: %.c
	$(CC) -c $< -o $@ $(HEADER)

clean:
	rm -f *.o *.a
