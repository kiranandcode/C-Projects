#ifndef LEX_H
#define LEX_H

#include "input.h"

#define MAXTOKEN 32


#define RBRACE 1
#define LBRACE 2
#define RBRACKET 3
#define LBRACKET 4
#define SQUARELBRACKET 5
#define SQUARERBRACKET 6
#define SEMICOLON 7
#define COMMA 8
#define COLON 9

#define NULL_TOK 10
#define TRUE 11
#define FALSE 12
#define ID 13
#define NUMERAL 14
#define BREAK 15
#define CASE 16
#define CATCH 17
#define CLASS 18
#define CONST 19
#define CONTINUE 20
#define DEBUGGER 21
#define DEFAULT 22
#define DELETE 23
#define DO 24
#define ELSE 25
#define EXPORT 26
#define EXTENDS 27
#define FINALLY 28
#define FOR 29
#define FUNCTION 30
#define IF 31
#define IMPORT 32
#define IN 33
#define INSTANCEOF 34
#define NEW 35
#define RETURN 36
#define SUPER 37
#define SWITCH 38
#define THIS 39
#define THROW 40
#define TRY 41
#define TYPEOF 42
#define VAR 43
#define VOID 44
#define WHILE 45
#define WITH 46
#define YIELD 47
#define EOI 48

int gettoken(filebuffer_F buffer);

#endif //LEX_H
