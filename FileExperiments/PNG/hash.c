#include <stdlib.h>
#define H_TABLE_SIZE 100


struct h_entry {
	void *value;
	struct h_entry *next;
};


struct h_hash_table {
	struct h_entry *table[H_TABLE_SIZE];
	int (*compare)(void *, void *);
	void (*delete)(void *);
	int (*hash)(void *, int);
};


void h_delete_chain(struct h_entry *entry, void (*delete)(void *));

struct h_hash_table *h_init_table(
		int (*compare)(void *,void *),
		void (*delete)(void *),
		int (*hash)(void *, int)){ 

	struct h_hash_table * table = malloc(sizeof(struct h_hash_table));
	table->compare = compare;
	table->delete = delete;
	table->hash = hash;


	int i;
	for(i = 0; i<H_TABLE_SIZE; i++) {
		table->table[i] = NULL;
	}
	
	return table;
}


void h_delete_table(struct h_hash_table *table) {

	int i;
	for(i = 0; i<H_TABLE_SIZE; i++) {
		h_delete_chain(table->table[i], table->delete);
	}

	free(table);
}


void h_delete_chain(struct h_entry *entry, void (*delete)(void *)) {
	if(entry == NULL) return;


	while(entry != NULL) {
		if(entry->value != NULL)
			delete(entry->value);

		struct h_entry *next = entry->next;
		free(entry);
		entry = next;


	}
}



int h_hash_get_count(struct h_hash_table *table) {
	int count = 0;
	int i;

	for(i = 0; i < H_TABLE_SIZE; i++) {
		if(table->table[i] != NULL) {
			struct h_entry *entry = table->table[i];

			while(entry != NULL){
				count++;
				entry = entry->next;
			}
		}
	}
	return count;
}


void h_hash_get_entries(struct h_hash_table *table,void **entries, int size) {
	int count = 0;
	int i = 0;

	for(i = 0; count < size && i < H_TABLE_SIZE; ++i) {
		if(table->table[i] != NULL) {

			struct h_entry *entry = table->table[i];

			while(entry != NULL && count < size) {
				entries[count++] = entry->value;
				entry = entry->next;
			}

		}
	}

}


int h_hash_insert(struct h_hash_table *table, void *value) {
	int index = table->hash(value, H_TABLE_SIZE);


	struct h_entry *entry = table->table[index];

	if(entry == NULL) {
		entry = malloc(sizeof(struct h_entry));

		entry->next = NULL;
		entry->value = value;
		
		table->table[index]  = entry;

		return 1;

	} else {

		struct h_entry *prev = entry;
		while(entry != NULL && table->compare(entry->value, value)) prev = entry,entry = entry->next;
		if(entry == NULL) {
			entry = malloc(sizeof(struct h_entry));

			entry->next = NULL;
			entry->value = value;
			
			 prev->next = entry;

			return 1;

		

		} else return 0;

	}

}
