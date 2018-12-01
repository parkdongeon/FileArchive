typedef struct{
    int prefix;
    int character;
} DictElement;

void dictionaryArrayAdd(int prefix, int character, int value);
int dictionaryArrayPrefix(int value);
int dictionaryArrayCharacter(int value);

DictElement dictionaryArray[4095];

void dictionaryArrayAdd(int prefix, int character, int value) {
    dictionaryArray[value].prefix = prefix;
    dictionaryArray[value].character = character;
}

int dictionaryArrayPrefix(int value) {
    return dictionaryArray[value].prefix;
}

int dictionaryArrayCharacter(int value) {
    return dictionaryArray[value].character;
}
