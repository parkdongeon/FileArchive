
#include <stdio.h>
#include <stdlib.h>

enum {
    emptyPrefix = -1
};


struct DictNode {
    int value; 
    int prefix; 
    int character; 
    struct DictNode *next;
};

void dictionaryInit();
void appendNode(struct DictNode *node);
void dictionaryDestroy();
int dictionaryLookup(int prefix, int character);
int dictionaryPrefix(int value);
int dictionaryCharacter(int value);
void dictionaryAdd(int prefix, int character, int value);


struct DictNode *dictionary, *tail;


void dictionaryInit() {
    int i;
    struct DictNode *node;
    for (i = 0; i < 256; i++) { 
        node = (struct DictNode *)malloc(sizeof(struct DictNode));
        node->prefix = emptyPrefix;
        node->character = i;
        appendNode(node);
    }       
}


void appendNode(struct DictNode *node) {
    if (dictionary != NULL) tail->next = node;
    else dictionary = node;
    tail = node;
    node->next = NULL;
}


void dictionaryDestroy() {
    while (dictionary != NULL) {
        dictionary = dictionary->next; 
    }
}


int dictionaryLookup(int prefix, int character) {
    struct DictNode *node;
    for (node = dictionary; node != NULL; node = node->next) { 
        if (node->prefix == prefix && node->character == character) return node->value;
    }
    return emptyPrefix;
}

int dictionaryPrefix(int value) {
    struct DictNode *node;
    for (node = dictionary; node != NULL; node = node->next) { 
        if (node->value == value) return node->prefix;
    }
    return -1;
}

int dictionaryCharacter(int value) {
    struct DictNode *node;
    for (node = dictionary; node != NULL; node = node->next) { 
        if (node->value == value) {
            
            return node->character;
        }
    }
    return -1;
}


void dictionaryAdd(int prefix, int character, int value) {
    struct DictNode *node;
    node = (struct DictNode *)malloc(sizeof(struct DictNode));
    node->value = value;
    node->prefix = prefix;
    node->character = character;
    
    appendNode(node);
}
