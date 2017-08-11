#include "global.h"
#include "lexer.h"
#include "symbol.h"
#include "error.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>

char lexbuf[LEXER_BUFFER_SIZE];
int linenum = 1;
int columnnum = 1;

int token;
union tokenval tokenval;

int isoperator(int t);

int lexan() {
	int t;

	while(1) {
		t = getchar();
		if(t == ' ' || t == '\t')
			columnnum++; /* strip out whitespace */
		else if(t == '\n') {
			// if newline, reset column, update lines
			linenum++;
			columnnum = 1;
		} else if(isalpha(t) || isoperator(t)) {

			struct entry *p;
		        int b = 0;

			while(isalnum(t) || isoperator(t)) {
				lexbuf[b++] = t;
				columnnum++;
				if(isoperator(t)) {
					t = getchar();
					break;
				}
				t = getchar();
				if(b >= LEXER_BUFFER_SIZE)
					error(linenum, columnnum, "Lexer buffer exceeded");
			}

			lexbuf[b] = '\0';
			if(t != EOF)
				ungetc(t,stdin);
			p = lookup(lexbuf);
			tokenval.ptr_value = p;
			token = get_token(p);
			return token;

		} else if(isdigit(t)) {

			int sum =0;

			while(isdigit(t)) {
				columnnum++;
				sum *= 10;
				sum += (t - '0');
				t = getchar();
			}

			if(t != EOF)
				ungetc(t, stdin);
			tokenval.value = sum;
			token = NUM;
			return NUM;
		} else if(strchr("%[]",t) != NULL){
			columnnum++;
			tokenval.value = t;
			token = OP;
			return OP;
		
		} else if(t == EOF) {
			return DONE;
		} else {
			columnnum++;
			tokenval.value = NONE;
			token = NONE;
			return t;
		}

	}

	return 0;

}

int isoperator(int t) {
	return (t == '+' || t == '-' || t == '(' || t == ')' || t == '*' || t == '/');
}
