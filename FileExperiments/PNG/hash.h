#ifndef HASH_H
#define HASH_H

struct h_hash_table;

void h_delete_table(struct h_hash_table *table);
struct h_hash_table *h_init_table(
		int (*compare)(void *,void *),
		void (*delete)(void *),
		int (*hash)(void *, int)); 

int h_hash_get_count(struct h_hash_table *table);
void h_hash_get_entries(struct h_hash_table *table,void **entries, int size);

int h_hash_insert(struct h_hash_table *table, void *value);

#endif //HASH_H
