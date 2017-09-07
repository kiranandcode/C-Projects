#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>


#define PREBUFSIZE 100
#define BUFSIZE 100


#ifndef PREBUFSIZE
#define PREBUFSIZE 512
#endif
#ifndef BUFSIZE
#define BUFSIZE 4096
#endif

#define F filebuffer_F
struct F;
typedef struct F *F;

F filebuffer_new(FILE *fp);
void filebuffer_delete(F buffer);
char *filebuffer_nextline(F buffer);
char *filebuffer_fillbuf(F buffer);
char *filebuffer_fillbuf(F buffer);
char *filebuffer_incrementcp(F buffer);
size_t filebuffer_size(F buffer);
char *filebuffer_cp(F buffer);
char *filebuffer_limit(F buffer);
int filebuffer_lineno(F buffer);
int filebuffer_columnno(F buffer);
FILE *filebuffer_fp(F buffer);


#undef F
#endif
