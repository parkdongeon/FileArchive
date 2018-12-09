lzw: lzw.o hash.o
	gcc lzw.o hash.o -o Lzw

lzw.o: lzw.c
	gcc -c lzw.c

hash.o: hash.c hash.h
	gcc -c hash.c

clean:
	rm *.LZW -f Lzw lzw.o hash.o compressed expanded
