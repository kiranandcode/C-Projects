#ifndef SYM_H
#define SYM_H
#include "c.h"


#define HASHSIZE NELEMS(((Table)0)->buckets)
#define equalp(x) v.x == p->sym.u.c.v.x

// exported types
typedef struct symbol *Symbol;
typedef struct coord {
	char *file;
	unsigned x,y;
} Coordinate;
typedef struct table *Table;
typedef union value {
	char sc;
	short ss;
	int i;
	unsigned char uc;
	unsigned short us;
	unsigned int u;
	float f;
	double d;
	void *p;
} Value;

enum {CONSTANTS=1, LABELS, GLOBAL, PARAM, LOCAL };

// exported data
extern Table constants;
extern Table externals;
extern Table globals;
extern Table identifiers;
extern Table labels;
extern Table types;


extern int level;



// Functions 
Table table(Table, int);
void foreach(Table , int, void (*)ARGS((Symbol,void *)), void *);
void enterscope();
void exitscope();
Symbol install(char *, Table *, int, int);
Symbol lookup(char *, Table);
int genlabel(int);
Symbol findlabel(int);
Symbol constant(Type, Value);
Symbol intconst(int);
#endif //SYM_H
