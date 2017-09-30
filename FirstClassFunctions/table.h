#ifndef TABLE_H
#define TABLE_H
#define T table_T
#include "list.h"

struct T;
typedef struct T *T;

struct T_iterator {
	void *val;
};

T table_new(int hint,
	    int cmp(const void *x, const void *y),
	    unsigned hash(const void *key));
T table_delete(T *table, void delete(void *));

int table_length(T table);
void *table_put(T table, const void *key, void *value);
void *table_get(T table, const void *key);
void *table_remove(T table, const void *key);
void table_map(T table, void apply(const void *key, void **value, void *cl),void *cl);
void table_print(T table, void printKey(const void *key), void printValue(void *value));
list_L table_toList(T table);
void table_fromList(T table, list_L keys, list_L values);

struct T_iterator table_iterator(T table);
const void *table_iteratorgetkey(struct T_iterator *iterator);
void *table_iteratornext(struct T_iterator *iterator);
int table_iteratorhasnext(struct T_iterator *iterator);


#undef T
#endif //TABLE_H
