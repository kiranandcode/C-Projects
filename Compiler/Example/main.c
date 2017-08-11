#include "global.h"
#include "symbol.h"
#include "parser.h"
#include "init.h"

#include <stdio.h>



int main() {

	init();
	/*
	char statement[15];

	printf("Enter your statement: ");
	scanf("%15s ", statement);

	struct entry * entry = insert(statement, ID);

	printf("%p == %p\n", entry, lookup(statement));

	printf("Hello world!");
	*/
	parse();
	return 0;

}
