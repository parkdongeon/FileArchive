#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NODE 0
#define LEAF 1
#define END 2

#define COMPRESS 0
#define EXTRACT 1

enum { FALSE, TRUE };
char *help = "Usage:\n"\
"\tCompress: hf -c <input file> -o <compressed output file>\n"\
"\tExtract:  hf -x <compressed input file> -o <decompressed output file>\n";

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

int main(int argc, char *argv[]) {
    int i;
    char *ifname = NULL, *ofname = NULL, op;
    unsigned char d, offset, buffer, buffer_index, mask;
    long int bitcount = 0, last;
    FILE *ifile, *ofile;
    Node *tree, *pos;
    Node *symbols[256];
    Code *codes[256];
    int prefix[256]; /* holds the codes during encoding */
    
    /* Files */
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
        }
    }
    
    fclose(ifile);
    fclose(ofile);        
    exit(EXIT_SUCCESS);
}
