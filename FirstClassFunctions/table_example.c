#include "table.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void printString(const void * v_str) {
	const char *str = v_str;
	printf("%s", str);
}

void printInt(void *v_str) {
	int *in = v_str;
	printf("%d", *in);
}

int keycmp(const void *v_strA, const void *v_strB) {
	const char *strA = v_strA;
	const char *strB = v_strB;

	return strcmp(strA, strB);
}

unsigned keyhash(const void *v_str) {
	const char *str = v_str;

	unsigned res = 1;

	while(*str) {
		res = (((unsigned )str * 23) % 17) * res;
		str++;
	}

	return res;
}

#define SET(i,v) (i) = malloc(sizeof(*(i))); *(i) = v

int main() {
	printf("Running\n");
	int *i;
	table_T table = table_new(3, keycmp, keyhash);
	printf("Created table\n");
	table_print(table, printString, printInt);
	printf("printed table\n");

	SET(i,3);
	printf("inserting %d\n", *i);
	table_put(table, "word", i);
	printf("inserted word: 3 into table\n");

	SET(i,10);
	printf("inserting %d\n", *i);
	table_put(table, "logo", i);
	printf("inserted logo: 10 into table\n");

	SET(i,23);
	printf("inserting %d\n", *i);
	table_put(table, "fire", i);
	printf("inserted fire: 23 into table\n");

	table_print(table, printString, printInt);
	printf("printed table\n");

	char *str = "fire";
	printf("removing %s\n", str);
	i = table_remove(table, str);
	printf("returned %d\n", *i);

	table_print(table, printString, printInt);
	printf("printed table\n");

	str = "logo";
	printf("removing %s\n", str);
	i = table_remove(table, str);
	printf("returned %d\n", *i);

	table_print(table, printString, printInt);
	printf("printed table\n");
}
