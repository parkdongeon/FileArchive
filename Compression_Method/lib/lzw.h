// lzw.h


FILE *open_file(char *filename, char *mode);
void ctos(char *string, char c);
int get_val_for_string(char *string, hashtable_t *hashtable);
void output_code(FILE *output, unsigned int code);
void compress_file(FILE *ifp, FILE *ofp);
void get_translation(char *string, int c, hashtable_t *hashtable);
unsigned int input_code(FILE *input);

void decompress_file(FILE *ifp, FILE *ofp);
void handle_command(char flag, char *filename);




