#ifndef ERROR_H
#define ERROR_H
#include "c.h"

extern void expect ARGS((int tok));
extern void skipto ARGS((int tok, char set[]));

extern char kind[];

#endif // ERROR_H
