#include "global.h"
#include "emitter.h"
#include "symbol.h"

#include <stdio.h>

void emit(int t, union tokenval val) {

	switch(t) {
		case OP:
			printf("%c\n", val.value); break;
		case NUM:
			printf("%d\n", val.value); break;
		case ID:
			printtoken("%s\n", val.ptr_value); break;
		case MOD: 
			printf("keyword mod\n"); break;
		case DIV: 
			printf("keyword div\n");break;
		case IF: 
			printf("keyword if\n");break;
		case ELSE: 
			printf("keyword else\n");break;
		case INT: 
			printf("keyword int\n");break;
		case RETURN: 
			printf("keyword return\n");break;
		case L_PAREN: 
			printf("operator l_paren\n");break;
		case R_PAREN: 
			printf("operator r_paren\n");break;
		case PLUSOP:  
			printf("operator plusop\n");break;
		case SUBOP:   
			printf("operator subop\n");break;
		case MULTOP:  
			printf("operator multop\n");break;
		case DIVOP:   
			printf("operator divop\n");break;
		default:
			printf("token %d(%c), tokenval %d(%c)\n", t, t, val.value, val.value); break;

	}
}
