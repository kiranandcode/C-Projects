#ifndef SYMBOL_H
#define SYMBOL_H

#define SYMBOL_TABLE_SIZE 100

void init_symbol_table();
struct entry *insert(char *string, int token);
struct entry *lookup(char *string);

void printtoken(const char *fmt_string, struct entry *entry);
int get_token(struct entry *entry);

#endif //SYMBOL_H
