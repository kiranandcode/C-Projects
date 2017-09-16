#ifndef LEX_H
#define LEX_H

#include "input.h"

#define MAXTOKEN 32

int gettoken(filebuffer_F buffer);

// keywords
#define ABSTRACT 	0
#define ASSERT 		1

#define BOOLEAN 	2
#define BREAK 		3
#define BYTE 		4

#define CASE 		5
#define CATCH 		6
#define CHAR 		7
#define CLASS 		8
#define CONST 		9
#define CONTINUE 	10

#define DEFAULT 	11
#define DO 		12
#define DOUBLE 		13

#define ELSE 		14
#define ENUM 		15
#define EXTENDS 	16

//#define FALSE           17
#define FINAL 		18
#define FINALLY 	19
#define FLOAT 		20
#define FOR 		21

#define IF 		22

#define GOTO 		23

#define IMPLEMENTS 	24
#define IMPORT 		25
#define INSTANCEOF 	26
#define INT 		27
#define INTERFACE 	28

#define LONG 		29

#define NATIVE 		30
#define NEW 		31
//#define NULL_TOK        32

#define PACKAGE 	33
#define PRIVATE 	34
#define PROTECTED 	35
#define PUBLIC 		36

#define RETURN 		37

#define SHORT 		38
#define STATIC 		39
#define STRICTFP 	40
#define SUPER 		41
#define SWITCH 		42
#define SYNCHRONIZED 	43

#define THIS 		44
#define THROW 		45
#define THROWS 		46
#define TRANSIENT 	47
//#define TRUE            48
#define TRY 		49

#define VOID 		50
#define VOLATILE 	51

#define WHILE 		52

// literals
#define INTEGER_LITERAL 	53
#define FLOATINGPOINT_LITERAL 	54
#define BOOLEAN_LITERAL  	55
#define CHARACTER_LITERAL 	56
#define NULL_LITERAL      	57

// separators
#define L_PARENS	58
#define R_PARENS	59
#define L_BRACE		60
#define R_BRACE 	61
#define L_SQRBRACKET	62
#define R_SQRBRACKET	63
#define SEMICOLON	64
#define DOT		65
#define COMMA		66

// operators
#define ASSIGN_OP 		67
#define GT_OP 			68
#define LT_OP 			69
#define NOT_OP 			70
#define TILDE_OP 		71
#define TERNARY_OP 		72
#define COLON_OP 		73
#define EQ_OP 			74
#define LTEQ_OP 		75
#define GTEQ_OP 		76
#define NEQ_OP 			77
#define AND_OP 			78
#define OR_OP 			79
#define INC_OP 			80
#define DECR_OP 		81
#define ADD_OP 			82
#define SUB_OP 			83
#define MULT_OP 		84
#define DIV_OP 			85
#define BITAND_OP 		86
#define BITOR_OP 		87
#define BITXOR_OP 		88
#define MOD_OP 			89
#define LSHIFT_OP 		90
#define RSHIFT_OP 		91
#define RRSHIFT_OP 		92
#define ADDEQ_OP 		93
#define SUBEQ_OP 		94
#define MULTEQ_OP 		95
#define DIVEQ_OP 		96
#define BITANDEQ_OP 		97
#define BITOREQ_OP 		98
#define BITXOREQ_OP 		99
#define MODEQ_OP 		100
#define LSHIFTEQ_OP 		101
#define RSHIFTEQ_OP 		102
#define RRSHIFTEQ_OP 		103


#define EOI 104
#define IDENTIFIER 105
#endif 		//LEX_H
