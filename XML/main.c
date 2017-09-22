#include "lex.h"
#include <stdio.h>
#define CHRBUFSIZE 300

int main() {
	int tok;
	char buf[CHRBUFSIZE];
	lexer_L lexer = lexer_new(filebuffer_new(stdin));
	while((tok = lexer_gettoken(lexer)) != EOI) {
		switch(tok) {
			case TAGBEGIN:
				lexer_lexemecopy(lexer, buf, CHRBUFSIZE);
				printf("TAGBEGIN(%s)\n", buf);
				break;
			case TAGEND:         
				printf("TAGEND\n");
				break;
			case TAGCLOSE:       
				lexer_lexemecopy(lexer, buf, CHRBUFSIZE);
				printf("TAGCLOSE(%s)\n", buf);
				break;
			case TAGENDANDCLOSE: 
				printf("TAGENDANDCLOSE\n");
				break;
			case ATTRIBUTENAME:  
				lexer_lexemecopy(lexer, buf, CHRBUFSIZE);
				printf("ATTRIBUTENAME(%s)\n", buf);
				break;
			case EQUALSIGN:      
				printf("EQUALSIGN\n");
				break;
			case ATTRIBUTEVALUE: 
				lexer_lexemecopy(lexer, buf, CHRBUFSIZE);
				printf("ATTRIBUTEVALUE(%s)\n", buf);
				break;
			case CONTENT:        
				lexer_lexemecopy(lexer, buf, CHRBUFSIZE);
				printf("CONTENT(%s)\n", buf);
				break;
				break;
			case EOI:            
				printf("End of INPUT\n");
				break;
			default:
				fprintf(stderr, "ERROR: Unknown token %d\n", tok);

		}
	}

}
