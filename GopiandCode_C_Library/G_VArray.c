#include <stdio.h>
#include <stdlib.h>

#include "G_VArray.h"

//creates a Variable array, of a specified initial size;
VArray *createVArray(int length) {
  VArray *newVArray = malloc(sizeof(VArray)*1);
  newVArray->start = calloc(sizeof(int), length); //Calloc used to preinitialize to 0
  newVArray->length = length;
  return newVArray;
}

//sets a value into a Variable array
void setVArray(VArray *array, int index, int value) {
  if(index > array->length-1) {
    array->start = (int *)realloc(array->start, index+1);
    array->length = index+1;
    *(array->start + index) = value;
    printf("NOTE: Index greater than length, array size increased\n");
  }
  else {
    *(array->start + index) = value;
  }
  return;
}

//returns the value stored at a specified index in a variable array
int getVArray(VArray *array, int index) {
  if(index > array->length-1) fprintf(stderr, "ERROR: Attempted to retrieve index value greater than length of Variable Array\n"), exit(1);
  return *(array->start+index);
}


