#include "symbol.h"
#include "global.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Using a chained hash table
struct entry {
	int token;
	char *entry;
	struct entry *next;
};



struct entry *symbol_table[SYMBOL_TABLE_SIZE];


void init_symbol_table(){
	int i;
	// Clear the table
	for(i = 0; i<SYMBOL_TABLE_SIZE; ++i) {
		symbol_table[i] = NULL;
	}
}

int hash(char *string) {
	int i;
	int len = strlen(string);
	int product = 1;

	for(i = 0; i<len; ++i){
		product *= (string[i]);
		product %= SYMBOL_TABLE_SIZE;
	}
	

	return product;

}


struct entry *insert(char *string, int token) {
	int index = hash(string);
	int len = strlen(string);

	if(symbol_table[index] == NULL || symbol_table[index]->entry == NULL) {

		if(symbol_table[index] == NULL) symbol_table[index] = malloc(sizeof(struct entry));

		symbol_table[index]->entry = malloc(sizeof(char)*(len+1));
		symbol_table[index]->next = NULL;
		symbol_table[index]->token = token;

		strcpy(symbol_table[index]->entry, string);

		return symbol_table[index];

	} else {
		struct entry *symbol = symbol_table[index];
		int comparison;

		while((comparison = strcmp(string,symbol->entry))!= 0 && symbol->next != NULL){
			symbol = symbol->next;
		}

		if(comparison == 0) return symbol;

		struct entry *next;
		next = malloc(sizeof(struct entry));
		next->entry = malloc(sizeof(char)*(len+1));
		strcpy(next->entry, string);
		next->next = NULL;
		next->token = token;

		symbol->next = next;
		return next;

		}

	}


struct entry *lookup(char *string) {
	int index = hash(string);
	int comparison;

	struct entry* symbol = symbol_table[index];

	while(symbol && (comparison = strcmp(symbol->entry, string)) != 0)
		symbol = symbol->next;

	return symbol;
}


void printtoken(const char *fmt_string, struct entry *entry) {
	printf(fmt_string, entry->entry);
}

int get_token(struct entry *entry) {
	return entry->token;
}
