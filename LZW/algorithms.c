#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.c"
#include "file.c" 
#include "array.c" 

enum {
    dictionarySize = 4095, 
    codeLength = 12, 
    maxValue = dictionarySize - 1
};


void compress(FILE *inputFile, FILE *outputFile);
void decompress(FILE *inputFile, FILE *outputFile);
int decode(int code, FILE * outputFile);


void compress(FILE *inputFile, FILE *outputFile) {    
    int prefix = getc(inputFile);
    if (prefix == EOF) {
        return;
    }
    int character;

    int nextCode;
    int index;
    
   
    nextCode = 256;
    dictionaryInit();
    
   
    while ((character = getc(inputFile)) != (unsigned)EOF) { 
        if ((index = dictionaryLookup(prefix, character)) != -1) prefix = index; 
        else { /
            writeBinary(outputFile, prefix);
            
            
            if (nextCode < dictionarySize) dictionaryAdd(prefix, character, nextCode++);
            
          
            prefix = character; 
        }
    }
    
    writeBinary(outputFile, prefix);
    
    if (leftover > 0) fputc(leftoverBits << 4, outputFile);
    
    
    dictionaryDestroy();
}


void decompress(FILE * inputFile, FILE * outputFile) {
    
    int previousCode; int currentCode;
    int nextCode = 256; 
    int firstChar;
    
  
    previousCode = readBinary(inputFile);
    if (previousCode == 0) {
        return;
    }
    fputc(previousCode, outputFile);
    
    
    while ((currentCode = readBinary(inputFile)) > 0) { 
    
        if (currentCode >= nextCode) {
            fputc(firstChar = decode(previousCode, outputFile), outputFile); 
            
        } else firstChar = decode(currentCode, outputFile); 
        
      
        if (nextCode < dictionarySize) dictionaryArrayAdd(previousCode, firstChar, nextCode++);
        
      
        previousCode = currentCode;
    }
   
}

int decode(int code, FILE * outputFile) {
    int character; int temp;

    if (code > 255) { 
        character = dictionaryArrayCharacter(code);
        temp = decode(dictionaryArrayPrefix(code), outputFile); 
    } else {
        character = code;
        temp = code;
    }
    fputc(character, outputFile);
   
    return temp;
}
