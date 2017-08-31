#ifndef INPUT_H
#define INPUT_H
#include "c.h"

#define MAXLINE 512
#define BUFSIZE 4096

extern unsigned char *cp;
extern unsigned char *limit;
extern int infd;
extern char *firstfile;
extern char *file;
extern char *line;
extern int lineno;

void inputInit();
void nextline();
void fillbuf();

#endif //INPUT_H
