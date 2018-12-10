main: lzw.o hash.o main.o
	gcc lzw.o hash.o main.o -o LZW

main.o: main.c
	gcc -c main.c

lzw.o: lzw.c lzw.h 
	gcc -c lzw.c

hash.o: hash.c hash.h
	gcc -c hash.c

clean:
	rm *.LZW -f Lzw *.o compressed expanded




