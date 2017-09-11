//#include "classifier.h"
#include "list.h"

#include <stdio.h>
#include <stdlib.h>

void print(void *item) {
	if(item != NULL) {
		int i = *(int *)item;
		printf("%d", i);
	}
}
void delete(void *item) {
	printf("deleting ");
	print(item);
	printf("\n");
	//free(item);
}

int main() {

	list_L item = list_new();
	list_print(item, print);	
	int i = 10;
	int j = 15;
	int k = 28;
	int l = 32;
	list_append(item, &i);
	list_append(item, &j);
	list_print(item, print);	
	printf("\n");

	list_insert(item, &k, 2);
	list_append(item, &l);
	list_print(item, print);	
	
	printf("\n");


	list_print(item, print);	
	printf("\n");

	list_removeat(item, 1);


	list_print(item, print);	
	printf("\n");

	list_remove(item, &k);

	list_print(item, print);	
	printf("\n");


	void *i_ptrcopy = list_get(item, 1);
	int i_copy = *(int *)i_ptrcopy;

	printf("item in position [1] is %d\n", i_copy);

	list_delete(item, delete);

/*	bitstring_B stream = bitstring_generate("0110101110");
	pattern_B pattern =    pattern_generate("0###001110");
	
	bitstring_print(stream);
	printf("\n");
	pattern_print(pattern);
	printf("\n");
	if(pattern_matches(pattern, stream, NULL)) {
		printf("matches\n");
	} else {
		printf("does not match\n");
	}
	*/

}
