#include "global.h"
#include "symbol.h"
#include "error.h"

char lexbuf[BSIZE];
int lineno = 1;
int tokenval = NONE;


// each time lexan is called, it will
// retrieve a set of lexemes and then
// return a token
int lexan() {
	int t;
	while(1){ 
		t = getchar();
		if(t = ' ' || t == '\t')
			;	/* strip out blanks and tabs */ 
		else if(t == '\n')
			lineno = lineno + 1;
			/* if a newline is encountered, increment the lineno */
		else if(isdigit(t)) {
			ungetc(t, stdin);
			scanf("%d", &tokenval);
			/* if a digit is seen, read it and return digit TOKEN */
			return NUM;
		} else if(isalpha(t)) {
			int p,b = 0;
			while(isalnum(t)) {
				lexbuf[b] = t;
				t = getchar();
				b = b + 1;
				if(b > BSIZE)
					error("compiler error");
				/* keep on grabbing letters or numbers while available */
			}
			lexbuf[b] = EOS;
			if(t != EOF)
				ungetc(t, stdin);
			
			p = lookup(lexbuf);
			if(p == 0)
				p = insert(lexbuf, ID);

			/* insert it into the symtable */
			tokenval = p;

			// return ID
			return symtable[p].token;
			/* if a variable name is seen, read it into the symbol table and return */
		} else if(t==EOF){
			return DONE;
		} else {
			tokenval = NONE;
			return t;
		}
	}
}

