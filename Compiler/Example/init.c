#include "symbol.h"
#include "global.h"

void init_keywords();

void init() {

	init_symbol_table();
	init_keywords();

}


void init_keywords() {

	insert("mod", MOD);
	insert("div", DIV);
	insert("if", IF);
	insert("else", ELSE);
	insert("int", INT);
	insert("return", RETURN);
	insert("(", L_PAREN);
	insert(")", R_PAREN);
	insert("+", PLUSOP);
	insert("-", SUBOP);
	insert("*", MULTOP);
	insert("/", DIVOP);



}
