#include "G_List.h"
#include "G_VArray.h"
#include "G_Heap.h"
#include <stdio.h>

int main() {
/*
  intListEntry **temp = createList(15);
  intListEntry *List = *temp;
  intListInsertInOrder(List, 20);
  intListInsertInOrder(List, 15);
  intListAppend(List, 302);
  intListAppend(List, 12);
  printIntList(List);

  temp = intListSubList(List, 2,4);
  List = *temp;
  printIntList(List);
*/

///*	
VArray *array = createVArray(10);
int i;
for(i = 0; i<array->length; i++) setVArray(array, i, i%5+8);
for(i = 0; i<array->length; i++) printf("Value at [%d]: %d\n", i, getVArray(array, i));
setVArray(array, 14, 10);
setVArray(array, 12, 4);

setVArray(array, 21, 8);


for(i = 0; i<array->length; i++) printf("Value at [%d]: %d\n", i, getVArray(array, i));
//*/
	/*
	Heap *baby = createHeap(1, 0);
	addHeap(20, baby);
	addHeap(20, baby);
	addHeap(30, baby);
	addHeap(21, baby);
	addHeap(2, baby);
	popHeap(baby);
	popHeap(baby);
	addHeap(15, baby);
	addHeap(100, baby);
	addHeap(1, baby);
	printHeap(baby);
	printf("Greatest value is: %d\n", popHeap(baby));
	printf("Greatest value is: %d\n", popHeap(baby));
	printf("Greatest value is: %d\n", popHeap(baby));
	printf("Greatest value is: %d\n", popHeap(baby));
	printf("Greatest value is: %d\n", popHeap(baby));
	printf("Greatest value is: %d\n", popHeap(baby));
	printf("Greatest value is: %d\n", popHeap(baby));
	printHeap(baby);
	*/
}
