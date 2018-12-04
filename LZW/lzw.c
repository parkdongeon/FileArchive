
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "algorithms.c" 
FILE *inputFile;
FILE *outputFile;

int main(int argc, char** argv) { 
    if (argc > 2) {
        if (strcmp(argv[1], "c") == 0) { 
            inputFile = fopen(argv[2], "r"); 
            outputFile = fopen(strcat(argv[2], ".lzw"), "w+b");
            
            if (outputFile == NULL || inputFile == NULL) {
                printf("Can't open files\n'"); return 0;
            }
 
            compress(inputFile, outputFile);
        } else { 
            inputFile = fopen(argv[2], "rb"); 
            char temp[20]; int length = strlen(argv[2])-4;
            strncpy(temp, argv[2], length);
            temp[length] = '\0';
            outputFile = fopen(temp, "w");
            
            if (outputFile == NULL || inputFile == NULL) {
                printf("Can't open files\n'"); return 0;
            }
            
            decompress(inputFile, outputFile);
        }
        
        fclose(inputFile); fclose(outputFile); 
    } else {

        printf("Usage:    lzw <command> <input file>\n\n");
        printf("<Commands>\n  c       Compress\n  d       Decompress\n");
    }
    
    return 0;
}
