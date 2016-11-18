#ifndef G_VArray
#define G_VArray
typedef struct VArray {
  int* start;
  int length;
} VArray;

VArray *createVArray(int length);
void setVArray(VArray *array, int index, int value);
int getVArray(VArray *array, int index);

#endif
