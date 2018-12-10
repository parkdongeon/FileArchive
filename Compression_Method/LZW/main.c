#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "lzw.h"

#define BITS 16
#define LEN 256

#if BITS == 12
	#define HASHSIZE 4096
#endif 
#if BITS == 14 
	#define HASHSIZE 16384 
#endif 
#if BITS == 16
	#define HASHSIZE 65536 
#endif


int main(int argc, char *argv[])
{

	handle_command(argv[1][0], argv[2]);
	return 0;
}
