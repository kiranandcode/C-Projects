#ifndef G_HEAP
#define G_HEAP

typedef struct Heap {
  int length;
  int type;                              // 1 for min    0 for max
  int* array;
} Heap;


Heap *createHeap(int value, int type);
void addHeap(int value, Heap *heap);
int popHeap(Heap *heap);
void printHeap(Heap *heap);

#endif
