// lzw.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

// BITS = 12, 14, or 16
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

FILE *open_file(char *filename, char *mode)  
{
	FILE *fp = fopen(filename, mode);
	if(fp == NULL)
	{
		printf("** ERROR: file does not exist\n");
		return NULL;
	}
	return fp;
}

void ctos(char *string, char c)
{
	string[0] = c;
	string[1] = '\0';
}

// COMPRESSION ROUTINES //////////////////////////////////////////////////// 
int get_val_for_string(char *string, hashtable_t *hashtable)
{
	int result; 
	char *str_value = ht_get(hashtable, string); 

	if(str_value == NULL) result = string[0];	
	else result = atoi(str_value);

	//printf("*output: %s(%d)\n",string,result);
	return result;
}

void output_code(FILE *output, unsigned int code)
// function to output variable length codes
{
	static int output_bit_count = 0;
	static unsigned long output_bit_buffer = 0L;
	
	output_bit_buffer |= (unsigned long) code << (32-BITS-output_bit_count);
	output_bit_count += BITS;
	while(output_bit_count >= 8)
	{
		putc(output_bit_buffer >> 24,output);
		output_bit_buffer <<= 8;
		output_bit_count -= 8;
	}
}

void compress_file(FILE *ifp, FILE *ofp)
{
	hashtable_t *hashtable = ht_create(HASHSIZE);
	unsigned int inch, count = 256;
	
	char string[LEN];
	ctos(string, fgetc(ifp));
	
	while((inch = fgetc(ifp)) != EOF)
	{
		char c[LEN]; char new_string[LEN];
		ctos(c,inch);
		ctos(new_string, '\0');

		strcpy(new_string, string);
		strcat(new_string, c);
		
		if(ht_get(hashtable, new_string) != NULL) 
			strcat(string, c);
		else 
		{
			// output the code for STRING 
			int output = get_val_for_string(string, hashtable); 
			output_code(ofp, output);


			char value[LEN]; 
			sprintf(value, "%d", count); 
			ht_set(hashtable, new_string, value);

			strcpy(string, c);
			count++;
		}
	}
	// output the code for STRING 
	output_code(ofp, get_val_for_string(string, hashtable));
}


// DECOMPRESSION ROUTINES ///////////////////////////////////////////////////
void get_translation(char *string, int c, hashtable_t *hashtable)
{
	if(c < 256) ctos(string, c);
	else
	{
		char key[LEN];
		sprintf(key, "%d", c);
		strcpy(string, ht_get(hashtable, key));
	}
}

unsigned int input_code(FILE *input)
// function to read variable length input codes
{
	unsigned int return_value;
	static int input_bit_count = 0;
	static unsigned long input_bit_buffer = 0L;

	while(input_bit_count <= 24) 
	{
		input_bit_buffer |= (unsigned long) getc(input) << (24-input_bit_count);
		input_bit_count += 8;
	}
	return_value = input_bit_buffer >> (32-BITS);
	input_bit_buffer <<= BITS;
	input_bit_count -= BITS;

	return(return_value);
}

void decompress_file(FILE *ifp, FILE *ofp)
{
	hashtable_t *hashtable = ht_create(HASHSIZE);
	unsigned int inch, count = 256; 
	unsigned int old_code, new_code;
	char c[LEN];

	old_code = input_code(ifp) % HASHSIZE;

	putc(old_code, ofp); // output old code 
	ctos(c, old_code);	
	
	while((inch = input_code(ifp)) != EOF) 
	{
		char string[LEN], key[LEN], value[LEN];
		new_code = inch % HASHSIZE;

		sprintf(key, "%d", new_code);
							
		if(new_code > 255 && ht_get(hashtable, key) == NULL) // exception handling
		{
			get_translation(string, old_code, hashtable);
			strcat(string, c);	
		}
		else get_translation(string, new_code, hashtable);

		fputs(string, ofp); // output string 

		ctos(c, string[0]);
		sprintf(key, "%d", count);

		get_translation(value, old_code, hashtable);
		strcat(value, c);
		ht_set(hashtable, key, value);		

		old_code = new_code;
		count++;
	}
}

void handle_command(char flag, char *filename)
{
	FILE *ifp, *ofp; 
	if((ifp = open_file(filename, "r")) == NULL) exit(1) ;	

	switch(flag)
	{
		case 'c':
			printf("** compressing %s to: compressed.... ", filename);
			ofp = open_file("compressed", "w");
			compress_file(ifp, ofp);
			break;
		case 'd':
			printf("** decompressing %s to: expanded.... ", filename);
			ofp = open_file("expanded", "w");
			decompress_file(ifp, ofp);
			break;
		default:
			printf("** unrecognized format, please use:\n");
			printf("** lzw <c/d> <filename>\n");
			exit(1);
	}	
	printf("DONE!!\n\n");
	fclose(ifp); fclose(ofp);
}

int main(int argc, char *argv[]) 
{
	// FORMAT: lzw <c/d> <filename>
	handle_command(argv[1][0], argv[2]);
	return 0;	
}
