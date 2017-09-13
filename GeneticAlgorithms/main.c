#include "classifier.h"
#include "list.h"
#include "set.h"

#include <stdio.h>
#include <stdlib.h>
int *v;
#define integral(x) \
	((v = malloc(sizeof(int))), *v = x, v)

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
	free(item);
}

void *add_3(void *item) {
	int *in = item;
//	printf("item was [%d]\n", *in);
	*in += 3;
//	printf("item is now [%d]\n", *in);
	return in;
}

int greater_than_15(void *item) {
	int *in = item;

	return *in > 15;
}

int main() {
/*	
	list_L item = list_new();
	list_print(item, print);	
*/
	set_S set = set_new(100);
	set_print(set, print);
	printf("\n");

	int i = 10;
	int j = 15;
	int k = 28;
	int l = 32;


	set_insert(set, &i, NULL);

	set_print(set, print);
	printf("\n");

	set_insert(set, &i, NULL);

	set_print(set, print);
	printf("\n");
	
	if(set_contains(set, &i, NULL))
		printf("set contains %d\n", i);
	else 
		printf("set does not contain %d\n",i);
	if(set_contains(set, &j, NULL))
		printf("set contains %d\n", j);
	else 
		printf("set does not contain %d\n",j);
	
	set_insert(set, &j, NULL);

	if(set_contains(set, &j, NULL))
		printf("set contains %d\n", j);
	else 
		printf("set does not contain %d\n",j);
	set_insert(set, &k, NULL);

	if(set_contains(set, &k, NULL))
		printf("set contains %d\n", k);
	else 
		printf("set does not contain %d\n",k);
	set_insert(set, &l, NULL);

	set_print(set, print);
	printf("\n");


/*	
	list_append(item, integral(i));
	list_append(item, integral(j));
	list_print(item, print);	
	printf("\n");

	list_insert(item,integral(k), 2);
	list_append(item,integral(l));
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

	list_map(item, add_3);
	list_map(item, add_3);

	list_print(item, print);	
	printf("\n");

	list_L copy = list_filter(item, greater_than_15, NULL);

	list_print(copy, print);	
	printf("\n");
	
	list_delete(item, delete);

        bitstring_B stream = bitstring_generate("0110101110");
	pattern_B patternA =   pattern_random(10); 
	pattern_B patternB =   pattern_random(10);
	pattern_B patternC = pattern_crossover(patternA, patternB);
	
	bitstring_print(stream);
	printf("\n");
	pattern_print(patternA);
	printf("\n");
	pattern_print(patternB);
	printf("\n");
	pattern_print(patternC);
	printf("\n");
	if(pattern_matches(patternA, stream, NULL)) {
		printf("A matches\n");
	} else {
		printf("A does not match\n");
	}


	if(pattern_matches(patternB, stream, NULL)) {
		printf("B matches\n");
	} else {
		printf("B does not match\n");
	}

	if(pattern_matches(patternC, stream, NULL)) {
		printf("C matches\n");
	} else {
		printf("C does not match\n");
	}

	*/

}
