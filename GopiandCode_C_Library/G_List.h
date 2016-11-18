#ifndef G_List
#define G_List

struct intListEntry {
  int value;
  struct intListEntry *next;
};
typedef struct intListEntry intListEntry;


intListEntry **createList(int value);
void printIntList(intListEntry *Meta);

void intListAppend(intListEntry *List, int value);
void intListInsertInOrder(intListEntry *List, int value);

int intListRemove(intListEntry *List, int removeIndex);

int *intListRange(intListEntry *List, int startIndex, int endIndex);
intListEntry **intListSubList(intListEntry *OriginalList, int startIndex, int endIndex);

int intListValue(intListEntry *List, int retrieveIndex);
int intListCheck(intListEntry *List, int value);
int intListGetIndex(intListEntry *List, int checkIndex);

#endif
//Add Append List function
//Add sort List function
//Add reverse list function
//Add Pop function
