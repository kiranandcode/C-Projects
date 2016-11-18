#include <stdlib.h>
#include <stdio.h>

#include "G_List.h"

intListEntry **createList(int value) {
  //function returns a pointer to the head node in the list

  intListEntry *head = malloc(sizeof(intListEntry));
  head->value = value;
  head->next = 0;
  
  intListEntry **output = malloc(sizeof(intListEntry *));
  *output = head;
  return output;
}

void intListAppend(intListEntry *List, int value) {
  intListEntry *current = List;
  while(current->next != 0) current = current->next;
  intListEntry *newEntry = malloc(sizeof(intListEntry));
  newEntry->value = value, newEntry->next = 0;
  current->next = newEntry;
  
  return;
}

int intListRemove(intListEntry *List, int removeIndex) {
  intListEntry *current = List;
  int index = 0;
  int value;
  while(current->next != 0 && index < removeIndex-1){
    current = current->next, index++;
  }
  if(current->next == 0) fprintf(stderr, "ERROR: intListRemove attempted on out of bounds index\n"), exit(1);
  intListEntry *remove = current->next;

  
  if(removeIndex == 0) {
    value = current->value;
    current->value = remove->value;
    current->next = remove->next;
  }
  else {
    current->next = remove->next;
    value = remove->value;
  }
  free(remove);
  return value;
}


int *intListRange(intListEntry *List, int startIndex, int endIndex) {
  int length = endIndex - startIndex;
  if(length<0 || startIndex < 0) fprintf(stderr, "ERROR: intListRange attempted on invalid indexes\n"), exit(1);
  int *output = malloc(sizeof(int)*length);
  intListEntry *current = List;
  int currentIndex = 0;
  
  while(currentIndex<=endIndex) {
    if(currentIndex >= startIndex) output[currentIndex-startIndex] = current->value;
    if(current->next == 0) break;
    currentIndex++;
    current = current->next;
  }
  if(currentIndex < endIndex) fprintf(stderr, "WARNING: intListRange attempted on range outside list length - program will continue, but may not function properly (output array not filled)\n");
  return output;
}

intListEntry **intListSubList(intListEntry *OriginalList, int startIndex, int endIndex){
  intListEntry *current = OriginalList;
  int index = 0;
  intListEntry **output;

  while(index < startIndex) {
    if(current->next == 0) fprintf(stderr, "ERROR: intListSubList attempted on range outside list\n"), exit(1);
    current = current->next;
    index++;
  }
  output = createList(current->value);
  while(index <endIndex) {
    current = current->next;
    intListAppend(*output, current->value);
    index++;
    if(current->next == 0) {
      fprintf(stderr, "WARNING: intListSubList given end value outside range\n");
      break;
    }
  }
  return output;
}

void intListInsertInOrder(intListEntry *List, int value) {
  intListEntry *current = List;
  intListEntry *prev;
  int index = 0;
  
  while(current->value < value) {
    if(current->next == 0) break;

    prev = current;
    current = current->next;
    index++;
  }
  intListEntry *newEntry = malloc(sizeof(intListEntry));
  
  if(index == 0) {
    if(current->value > value) {
      newEntry->value = current->value;
      current->value = value;
    }
    else newEntry->value = value;
    newEntry->next = current->next;
    current->next = newEntry;
    return;
  }
  
  newEntry->next = prev->next;
  newEntry->value = value;
  prev->next = newEntry;
  return;
}


int intListValue(intListEntry *List, int retrieveIndex) {
  intListEntry *current = List;
  int index = 0;
  
  while(1) {
    if(current->next == 0) fprintf(stderr, "ERROR: intListValue attempted to retrieve value at index greater than the list length\n"), exit(1);
    if(index == retrieveIndex) return current->value;
    current = current->next;
    index++;
  }
}

int intListCheck(intListEntry *List, int value) {
  intListEntry *current = List;
  
  while(1) {
    if(current->value == value) return 1;
    if(current->next == 0) return 0;
    current = current->next;
  }
}


int intListGetIndex(intListEntry *List, int value) {
  intListEntry *current = List;
  int index = 0;
  
  while(1) {
    if(current->value == value) return index;
    if(current->next == 0) fprintf(stderr, "ERROR: intListGetIndex attempted to find the index of a value not in the list\n");
    current = current->next;
    index++;
  }
}


void printIntList(intListEntry *List) {
  intListEntry *current = List;
  int index = 0;
  printf("Value at index[%d]: %d\n", index, current->value);
  while(current->next != 0) {
    current = current->next;
    index++;
    printf("Value at index[%d]: %d\n", index, current->value);
  }
}
