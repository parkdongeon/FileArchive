#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

enum { FALSE, TRUE };

int main(int argc, char *argv[]) {
    int i;
    char *ifname = NULL, *ofname = NULL, op;
    unsigned char d, offset, buffer, buffer_index, mask;
    long int bitcount = 0, last;
    FILE *ifile, *ofile;
    Node *tree, *pos;
    Node *symbols[256];
    Code *codes[256];
    int prefix[256];
    
    
    if (argc < 5) {
        fprintf(stderr, "Not enough arguments\n%s", help);
        exit(EXIT_FAILURE);
    }
    
    for (i = 0; i < 5; i++)
    {
        if (!strcmp(argv[i], "-c")) {
            op = COMPRESS;
            ifname = argv[++i];
        }
        else if (!strcmp(argv[i], "-x")) {
            op = EXTRACT;
            ifname = argv[++i];
        }
        else if (!strcmp(argv[i], "-o"))
            ofname = argv[++i];
    }
    if (ifname == NULL || ofname == NULL) {
        printf("File names not specified\n%s", help);
        exit(EXIT_FAILURE);
    }
    if ((ifile = fopen(ifname, "rb")) == NULL) {
        fprintf(stderr, "Can't open input file %s\n", ifname);
        exit(EXIT_FAILURE);
    }
    if ((ofile = fopen(ofname, "wb")) == NULL) {
        fprintf(stderr, "Can't open output file %s\n", ofname);
        exit(EXIT_FAILURE);
    }
    
    /* Reset the prefix array */
    for (i = 0; i < 256; i++)
        prefix[i] = -1;
        
    /* Create and set the nodes */
    for (i = 0; i < 256; i++) {
        Node *new = malloc(sizeof(Node));
        new->data = i;
        new->freq = 0;
        new->left = new->right = NULL;
        symbols[i] = new;
    }
    
    /* Compression */
    if (op == COMPRESS) {   
        while (fread(&d, sizeof(unsigned char), 1, ifile))
            symbols[d]->freq++;
        
        tree = assemble_tree(symbols);
        gen_code(tree, prefix, 0, codes);
        
        fseek(ifile, 0, SEEK_SET);
        while (fread(&d, sizeof(unsigned char), 1, ifile))
            bitcount += codes[d]->size;
        offset = bitcount % 8;
        write_tree(tree, ofile, TRUE, &offset);
        buffer = 0;
        buffer_index = 0;
        fseek(ifile, 0, SEEK_SET);
        while (fread(&d, sizeof(unsigned char), 1, ifile)) {
            for (i = 0; i < codes[d]->size; i++) {
                buffer <<= 1;
                buffer |= codes[d]->value[i];
                buffer_index++;
                if (buffer_index == 8) {
                    fwrite(&buffer, sizeof(unsigned char), 1, ofile);
                    buffer = 0;
                    buffer_index = 0;
                }
            }
        }
        if (buffer_index > 0)
            fwrite(&buffer, sizeof(unsigned char), 1, ofile);
            
        fseek(ifile, 0, SEEK_END);
        fseek(ofile, 0, SEEK_END);
        printf("Input file size: %ld bytes\n", ftell(ifile));
        printf("Output file size: %ld bytes\n", ftell(ofile));
        printf("Compression rate: %.2f\n", 1 - ((float) ftell(ofile) / 
                                               (float) ftell(ifile)));
    }
    /* Extraction */
    else {
        fseek(ifile, 0, SEEK_END);
        last = ftell(ifile);
        fseek(ifile, 0, SEEK_SET);
        
        tree = read_tree(ifile, TRUE, &offset);
        pos = tree;
        while (fread(&d, sizeof(unsigned char), 1, ifile)) {
            mask = (ftell(ifile) == last && offset)? 0x1 << (offset - 1) : 0x80;
            while (mask) {
                pos = (d & mask)? pos->right : pos->left;
                if (pos->left == NULL && pos->right == NULL) {
                    fwrite(&pos->data, sizeof(unsigned char), 1, ofile);
                    pos = tree;
                }
                mask >>= 1;
            }
        remove(ifname);
        }
    }
    
    fclose(ifile);
    fclose(ofile);        
    exit(EXIT_SUCCESS);
}
