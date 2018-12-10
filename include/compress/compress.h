#ifndef COMPRESS_H
#define COMPRESS_H

#include <stdio.h>
#include <stdint.h>


#define COMPRESS 0
#define EXTRACT 1

char *help;

typedef struct Node {
  unsigned char data;
  int freq;
  struct Node *left;
  struct Node *right;
} Node;

typedef struct Code {
  unsigned char value[256];
  unsigned char size;
} Code;

int get_min(Node **symbols, int discard);
Node *assemble_tree(Node **symbols);
void gen_code(Node *node, int *prefix, int index, Code **codes);
void write_tree(Node *node, FILE *ofile, int root, unsigned char *offset);
Node *read_tree(FILE *ifile, int root, unsigned char *offset);



#endif
