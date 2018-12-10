CC = gcc
HEADER = -I../../include/compress
OBJFILES = compress.o

libcompress.a: $(OBJFILES)
	ar rv libcompress.a $(OBJFILES)

%.o: %.c
	$(CC) -c $< -o $@ $(HEADER)

clean:
	rm -f *.o *.a
