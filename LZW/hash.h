// hash.h

struct entry_s;
typedef struct entry_s entry_t; 
struct hashtable_s; 
typedef struct hashtable_s hashtable_t;

hashtable_t *ht_create(int size);
int ht_hash(hashtable_t *hashtable, char *key);
entry_t *ht_newpair(char *key, char *value);
void ht_set(hashtable_t *hashtable, char *key, char *value);
char *ht_get(hashtable_t *hashtable, char *key);

