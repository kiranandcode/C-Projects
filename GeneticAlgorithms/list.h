#ifndef LIST_H
#define LIST_H

#define L list_L

struct L;
typedef struct L *L;

struct L_iterator {
	void *current;
};

L list_new();
void list_delete(L list, void (*delete)(void *));
void *list_get(L list, unsigned int position);
void list_insert(L list, void *item, unsigned int position);
void *list_removeat(L list, unsigned int position);
void *list_remove(L list, void *item);
void list_append(L list, void *item);
void list_print(L list, void (*print)(void *));
struct L_iterator list_iterator(L list);
int list_iteratorhasnext(struct L_iterator *list_iterator);
void *list_iteratornext(struct L_iterator *list_iterator);
void *list_iteratorcurrent(struct L_iterator *list_iterator);
unsigned int list_length(L list);

#undef L
#endif
