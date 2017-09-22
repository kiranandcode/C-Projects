#ifndef LEX_H
#define LEX_H

#include "filebuffer.h"
#define L lexer_L
struct L;
typedef struct L *L;

#define MAXTOKEN 32
L lexer_new(filebuffer_F buffer);
void lexer_delete(L lexer);

int lexer_gettoken(L lexer);
int lexer_getlasttoken(L lexer);
size_t lexer_lexemelen(L lexer);
void lexer_lexemecopy(lexer_L lexer, char *buf, size_t size);

// LEXER TOKENS 
#define TAGBEGIN       1
#define TAGEND         2
#define TAGCLOSE       3
#define TAGENDANDCLOSE 4
#define ATTRIBUTENAME  5
#define EQUALSIGN      6 
#define ATTRIBUTEVALUE 7
#define CONTENT        8
#define EOI            10


#undef L
#endif 		//LEX_H
