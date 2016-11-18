#include <stdlib.h>
#include <stdio.h>

#include "G_Heap.h"

//Swaps 2 pointers so the location they point to is swapped
void swap(int *a, int *b) {
  int c;
  c = *a;
  *a = *b;
  *b = c;
  return;
}

Heap *createHeap(int value, int type) {
  Heap * Output = malloc(sizeof(Heap) * 1);

  Output->type = type;
  Output->length = 1;
  Output->array = malloc(sizeof(int) * (Output->length));
  *(Output->array) = value;

  return Output;
}


void addHeap(int value, Heap *heap) {
  int current = heap->length;

  heap->length += 1;                                             //Increase length by 1 to insert element
  realloc(heap->array, heap->length * sizeof(int));              //Increase size of array by 1
  *(heap->array + current) = value;                              //Set the new position to value


  if (heap->type == 1) {                                         //1 for Min, 0 for Max
    while (1) {
      if (current == 0) break;
      if (heap->array[current] > heap->array[(current - 1) / 2]) break;
      swap((heap->array + current), (heap->array + (current - 1) / 2));
      current = (current - 1) / 2;
    }
  }
  else if (heap->type == 0) {
    while (1) {
      if (current < 1) break;
      if (heap->array[current] < heap->array[(current - 1) / 2]) break;
      swap((heap->array + current), (heap->array + (current - 1) / 2));
      current = (current - 1) / 2;
    }
  }
  else fprintf(stderr, "ERROR: Heap does not conform to known type\n"), exit(1);
}

int popHeap(Heap *heap) {
  if(heap->length == 0) fprintf(stderr, "ERROR: PopHeap called on empty heap\n"), exit(1);
  int *a, *b;
  int b_flag = 0, a_flag = 0;
  int node = 0;
  int current = heap->length - 1;
  int output = *(heap->array);


  swap((heap->array), (heap->array + current));
  heap->length--;
  current--;
  realloc(heap->array, heap->length * sizeof(int));


  if (heap->type == 0) {
    while (1) {
      if (heap->length <= 2) {
        if (heap->length <= 1) break;

        else if ((heap->length <= 2) && (*heap->array > *(heap->array + 1))) break;

        else {
          swap(heap->array, (heap->array + 1));
          break;
        }
      }

      if ((2 * node + 1) < heap->length) a = heap->array + (2 * node + 1); else a_flag = 1;
      if ((2 * node + 2) < heap->length) b = heap->array + (2 * node + 2); else  b_flag = 1;
      if ((*(heap->array + node) < *a || *(heap->array + node) < *b) && (!a_flag || !b_flag)) {

        if ((*a > *b) && !(a_flag)) swap((heap->array + node), a), node = (2 * node + 1);
        else if (!b_flag) swap((heap->array + node), b), node = (2 * node + 2);
		else break;
      }
      else break;
    }

  }


  else if (heap->type == 1) {
    while (1) {
      if (heap->length <= 2) {
        if (heap->length <= 1) break;

        else if ((heap->length <= 2) && (*heap->array < *(heap->array + 1))) break;

        else {
          swap(heap->array, (heap->array + 1));
          break;
        }

      }
      if ((2 * node + 1) < heap->length) a = heap->array + (2 * node + 1); else a_flag = 1;
      if ((2 * node + 2) < heap->length) b = heap->array + (2 * node + 2); else  b_flag = 1;
      if ((*(heap->array + node) > *a || *(heap->array + node) > *b) && (!a_flag || !b_flag)) {

        if ((*a < *b) && !(a_flag)) swap((heap->array + node), a), node = (2 * node + 1);
        else if (!b_flag) swap((heap->array + node), b), node = (2 * node + 2);
		else break;
      }
      else break;
    }

  }


  else fprintf(stderr, "ERROR: Unknown type heap specified to popHeap\n"), exit(1);

  return output;
}


void printHeap(Heap *heap) {
  int i;
  for (i = 0; i < heap->length; i++) {
    fprintf(stdout, "Heap index [%d] : %d\n", i, *(heap->array + i));
  }

}
