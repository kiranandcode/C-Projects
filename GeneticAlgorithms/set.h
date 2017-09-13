#ifndef SET_H
#define SET_H
#define S set_S

struct S;
typedef struct S *S;

struct S_iterator {
	void *current;
};

S set_new(unsigned int table_size);
void set_insert(S set, void *item, unsigned int (*hash)(void *));
int set_contains(S set, void *item, unsigned int (*hash)(void *));

void set_print(S set, void (*print)(void *));

struct S_iterator set_iterator(S set);
int set_iteratorhasnext(struct S_iterator *iter);
void *set_iteratornext(struct S_iterator *iter);
#undef S
#endif //SET_H
