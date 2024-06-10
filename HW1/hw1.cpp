#include <stdio.h>
#include "tokens.hpp"
extern int yylex();

void showToken(const char * name)
{
        printf("%d %s %s\n", yylineno, name, yytext);
}

int main(){
	int token;
	while(token = yylex()) {
		if (token == NUM) {
			showToken("NUM");
		} else if (token == ID) {
			showToken("ID");
		} else if (token == INT) {
			showToken("INT");
		} else if (token == BYTE) {
			showToken("BYTE");
		} else if (token == B) {
			showToken("B");
		} else if (token == BOOL) {
			showToken("BOOL");
		} else if (token == AND) {
			showToken("AND");
		} else if (token == OR) {
			showToken("OR");
		} else if (token == NOT) {
			showToken("NOT");
		} else if (token == TRUE) {
			showToken("TRUE");
		} else if (token == FALSE) {
			showToken("FALSE");
		} else if (token == RETURN) {
			showToken("RETURN");
		} else if (token == IF) {
			showToken("IF");
		} else if (token == ELSE) {
			showToken("ELSE");
		} else if (token == WHILE) {
			showToken("WHILE");
		} else if (token == BREAK) {
			showToken("BREAK");
		} else if (token == CONTINUE) {
			showToken("CONTINUE");
		} else if (token == SC) {
			showToken("SC");
		} else if (token == COMMA) {
			showToken("COMMA");
		} else if (token == LPAREN) {
			showToken("LPAREN");
		} else if (token == RPAREN) {
			showToken("RPAREN");
		} else if (token == LBRACE) {
			showToken("LBRACE");
		} else if (token == RBRACE) {
			showToken("RBRACE");
		} else if (token == ASSIGN) {
			showToken("ASSIGN");
		} else if (token == RELOP) {
			showToken("RELOP");
		} else if (token == BINOP) {
			showToken("BINOP");
		} else if (token == COMMENT) {
			showToken("COMMENT");
		} else if (token == NUM) {
			showToken("NUM");
		} else if (token == STRING) {
			showToken("STRING");
		} else if (token == OVERRIDE) {
			showToken("OVERRIDE");
		} else if (token == UNKNOWN) {
			showToken("UNKNOWN");
		} else if (token == UNCLOSED) {
			showToken("UNCLOSED");
		}
	}
	return 0;
}