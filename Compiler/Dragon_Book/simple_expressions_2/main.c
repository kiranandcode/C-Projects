#include "input.h"
#include "global.h"
#include "lex.h"

#include <stdio.h>


int main() {
	FILE *fp;

	fp = fopen("test.txt", "r");

	if(fp != NULL ){
		filebuffer_F buffer = filebuffer_new(stdin);

		int t;
		while((t = gettoken(buffer)) != EOI) {
			printf("%s{%d} ", lexer_token_names[t], t);
		}

	/*	do {
			printf("[%d]: ", filebuffer_lineno(buffer));
			char *cp = filebuffer_cp(buffer);
			while(*cp != '\n' && cp < filebuffer_limit(buffer)) {
				printf("%c", *cp);
				cp++;
				filebuffer_incrementcp(buffer);
			}
			printf("\n");
		} while(filebuffer_nextline(buffer) != filebuffer_limit(buffer)); */
	}
}
