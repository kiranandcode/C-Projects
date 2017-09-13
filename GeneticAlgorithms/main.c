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
//	free(item);
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

unsigned int hash(void *item) {
	int *in  = item;

	return *in * 13;
}

int main() {
/*	
	list_L item = list_new();
	list_print(item, print);	
*/
	set_S set = set_newcustomhash(100, hash);
	set_print(set, print);
	printf("\n");

	int i = 10;
	int j = 15;
	int k = 28;
	int l = 32;


	set_insert(set, &i);

	set_print(set, print);
	printf("\n");

	set_insert(set, &i);

	set_print(set, print);
	printf("\n");
	
	if(set_contains(set, &i))
		printf("set contains %d\n", i);
	else 
		printf("set does not contain %d\n",i);
	if(set_contains(set, &j))
		printf("set contains %d\n", j);
	else 
		printf("set does not contain %d\n",j);
	
	set_insert(set, &j);

	if(set_contains(set, &j))
		printf("set contains %d\n", j);
	else 
		printf("set does not contain %d\n",j);
	set_insert(set, &k);

	if(set_contains(set, &k))
		printf("set contains %d\n", k);
	else 
		printf("set does not contain %d\n",k);
	set_insert(set, &l);

	set_print(set, print);
	printf("\n");

	set_remove(set, &k, NULL);

	if(set_contains(set, &k))
		printf("set contains %d\n", k);
	else 
		printf("set does not contain %d\n",k);
	
	printf("making a copy: \n");
	set_S copy = set_copy(set, NULL);

	set_print(set, print);
	printf("\n");

	set_remove(set, &i,  NULL);
	set_remove(set, &i,  NULL);

	set_print(set, print);
	printf("\n");

	printf("unioning copy and current\n");
	set_S uni = set_union(copy, set, NULL);

	printf("union:\n");
	set_print(uni, print);
	printf("\n");

	printf("intersecting copy and current\n");
	set_S inter = set_intersect(copy, set, NULL);

	printf("intersection:\n");
	set_print(inter, print);
	printf("\n");



	printf("subtracting copy and current\n");
	set_S subt = set_difference(copy, set, NULL);

	printf("subtracted (copy - set):\n");
	set_print(subt, print);
	printf("\n");



	set_remove(set, &i, NULL);
	set_remove(set, &l, NULL);
	set_remove(set, &j, NULL);

	set_print(set, print);
	printf("\n");

	set_delete(set, delete);
	set_delete(copy, delete);

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
