#include "emitter.h"
#include "error.h"
#include "global.h"

#include <stdio.h>

int lookahead;

void expr();
void match(int token);
void term();
void factor();


void parse() {
	lookahead = lexan();
	while(lookahead != DONE) {
		expr(); match(';');
	}

}

void expr() {
	int t;
	term();
	while(1)
		switch(lookahead) {
			case PLUSOP: case SUBOP:
					t = lookahead;
					match(lookahead);
					term();
					emit(lookahead, tokenval);
					continue;

			default:
				return;
		}
}


void term() {
	int t;
	factor();
	while(1)
		switch(lookahead){
			case DIV: case MOD: case DIVOP: case MULTOP:
				t = lookahead;
				match(lookahead);
				term();
				emit(lookahead, tokenval);
				continue;
			default:
				return;

		} 

}

void factor() {
	switch(lookahead) {
		case L_PAREN:
			match(L_PAREN); expr(); match(R_PAREN); break;
		case NUM:
			emit(lookahead, tokenval); match(NUM); break;
		case ID:
			emit(lookahead, tokenval); match(ID); break;
		default:
			error(linenum, columnnum, "syntax error");
	}

}


void match(int t) {
	if(lookahead == t)
		lookahead = lexan();
	else {
		char buf[40];
		sprintf(buf, "expecting token %d(%c) got token %d(%c)", t,t, lookahead, lookahead);
		error(linenum, columnnum, buf);
				
				
	}

}
