#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

#define NODE 0
#define LEAF 1
#define END 2

enum { FALSE, TRUE };
char *help = "Usage:\n"\
"\tCompress: hf -c <input file> -o <compressed output file>\n"\
"\tExtract:  hf -x <compressed input file> -o <decompressed output file>\n";


int get_min(Node **symbols, int discard) {
  int i;
  int min = -1;

  i = 0;
  for (i = 0; i < 256 && min == -1; i++)
    min = (symbols[i] != NULL && 
      i != discard && 
      symbols[i]->freq != 0)? i : -1;
  if (min == -1) return min;

  for (i = 0; i < 256; i++) {
    if (symbols[i] == NULL || i == discard || symbols[i]->freq == 0)
      continue;
    if (symbols[i]->freq < symbols[min]->freq)
      min = i;
  }

  return min;
}

Node *assemble_tree(Node **symbols) {
  int min_a, min_b;

  while (TRUE) {
    min_a = get_min(symbols, -1);
    min_b = get_min(symbols, min_a);

    if (min_b == -1)
      return symbols[min_a];


    Node *new = malloc(sizeof(Node));
    new->data = 0;
    new->freq = symbols[min_a]->freq + symbols[min_b]->freq;
    new->left = symbols[min_a];
    new->right = symbols[min_b];
    symbols[min_a] = new;
    symbols[min_b] = NULL;
  }
}

void gen_code(Node *node, int *prefix, int index, Code **codes) {
  int i = 0;
  Code *code;
  if (node->left != NULL || node->right != NULL) {
    prefix[index] = 0;
    prefix[index + 1] = -1;
    gen_code(node->left, prefix, index + 1, codes);

    prefix[index] = 1;
    prefix[index + 1] = -1;
    gen_code(node->right, prefix, index + 1, codes);
  }
  else {
    code = malloc(sizeof(Code));
    for (i = 0; i < index || prefix[i] != -1; i++)
      code->value[i] = prefix[i];
    code->size = i;
    codes[node->data] = code;
  }
}

void write_tree(Node *node, FILE *ofile, int root, unsigned char *offset) {
  unsigned char tree_node = NODE, tree_leaf = LEAF, tree_end = END;
  if (node->left != NULL || node->right != NULL) {
    fwrite(&tree_node, sizeof(unsigned char), 1, ofile);
    write_tree(node->left, ofile, FALSE, offset);
    write_tree(node->right, ofile, FALSE, offset);
  }
  else {
    fwrite(&tree_leaf, sizeof(unsigned char), 1, ofile);
    fwrite(&node->data, sizeof(unsigned char), 1, ofile);
  }

  if (root) {
    fwrite(&tree_end, sizeof(unsigned char), 1, ofile);
    fwrite(offset, sizeof(unsigned char), 1, ofile);
  }
}

Node *read_tree(FILE *ifile, int root, unsigned char *offset) {
  unsigned char d;
  Node *left, *right, *new;

  new = malloc(sizeof(Node));
  fread(&d, sizeof(unsigned char), 1, ifile);

  if (d == NODE) {
    left = read_tree(ifile, FALSE, offset);
    right = read_tree(ifile, FALSE, offset);
    new->left = left;
    new->right = right;
  }
  else if (d == LEAF) {
    fread(&d, sizeof(unsigned char), 1, ifile);
    new->data = d;
    new->left = NULL;
    new->right = NULL;
  }

  if (root) {
    fread(&d, sizeof(unsigned char), 1, ifile);
    if (d == END)
      fread(offset, sizeof(unsigned char), 1, ifile);
  }

  return new;
}
