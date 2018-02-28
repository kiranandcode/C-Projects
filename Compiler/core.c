#include "core.h"

#include "base.h"

#include <string.h>



void Expression() {
	// LL(1) parser
	// using 1 token lookahead here to check whether to parse ('+') or parse terms
	if(IsAddop(Look)){
		EmitLn("CLR D0");
	}
	else {
		Term();
	}
	while(Look == '+' || Look == '-') {
		EmitLn("MOVE D0,-(SP)");
		switch(Look) {
			case '+':
				Add();
				break;
			case '-':
				Subtract();
				break;
			default:
				Expected("Addop");
		}
	}
}


void Add() {
	Match('+');
	Term();
	EmitLn("ADD (SP)+,D0");
}

void Subtract() {
	Match('-');
	Term();
	EmitLn("SUB (SP)+,D0");
	EmitLn("NEG D0");
}



void Factor() {
	if(Look == '(') {
		Match('(');
		Expression();
		Match(')');
	}
	else if(IsAlpha(Look)) {
		Ident();
	}
	else {
		char output[11];
		strcpy(output,"MOVE #");
		char temp[2];
		temp[0] = GetNum();
		temp[1] = '\0';
		strcat(output, temp);
		strcat(output, ",D0");	
		EmitLn(output);
	}
}

void Ident() {
	char Name = GetName();
	if(Look == '(') {
		Match('(');
		Match(')');
		char resp[6];
		strcpy(resp, "BSR ");
		char temp[2];
		temp[0] = Name;
		temp[1] = '\0';
		strcat(resp,temp);
		EmitLn(resp);

	} else {
		char str[14];
		strcpy(str, "MOVE ");
		char temp[2];
		temp[0] = Name; 
		temp[1] = '\0';
		strcat(str, temp);
		strcat(str,"(PC),D0");
	}
}

void Multiply() {
	Match('*');
	Factor();
	EmitLn("MULS (SP)+,D0");
}

void Divide() {
	Match('/');
	Factor();
	EmitLn("MOVE (SP)+,D1");
	EmitLn("DIVS D1,D0");
}



void Term() {

	Factor();
	while(Look == '*' || Look == '/') {
		EmitLn("MOVE D0,-(SP)");
		switch(Look) {
			case '*':
				Multiply();
				break;
			case '/':
				Divide();
				break;
			default:
				Expected("Mulop");
		}
	}
}
