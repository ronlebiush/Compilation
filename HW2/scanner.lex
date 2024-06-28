%{

/* Declarations section */
#include <stdio.h>
#include "parser.tab.hpp"
#include "output.hpp"

char handleAscii(char first, char sec);

extern int yylineno;
extern char* yytext;
extern int yyleng;
extern int yylex();
extern char textbuff[1024];
extern char* textbuffptr;


%}

%option yylineno
%option noyywrap
digit   		([0-9])
nonzero         ([1-9])
letter  		([a-zA-Z])
whitespace		([\t\n\r ])
string          (["])
relop           ((==)|(!=)|(\<=)|(\>=)|(\<)|(\>))
binop           ((\+)|(\-)|(\*)|(\/))
backslash       \x5C
printableascii  ([\x20-\x21\x23-\x5B\x5D-\x7E])
comment \/\/[^\n\r]*

%x STRINGS
%x ESCAPESEQ
%x ASCII
%%

"int"                       return INT;
"byte"                      return BYTE;
"b"                         return B;
"bool"                      return BOOL;
"and"                       return AND;
"or"                        return OR;
"not"                       return NOT;
"true"                      return TRUE;
"false"                     return FALSE;
"return"                    return RETURN;
"if"                        return IF;
"else"                      return ELSE;
"while"                     return WHILE;
"break"                     return BREAK;
"continue"                  return CONTINUE;
";"                         return SC;
"("                         return LPAREN;
")"                         return RPAREN;
"{"                         return LBRACE;
"}"                         return RBRACE;
"="                         return ASSIGN;
{relop}                     return RELOP;
{binop}                     return BINOP;
{letter}+({letter}|{digit})*		return ID;
"0"                         return NUM;
{nonzero}{digit}*          	return NUM;
(\")                        BEGIN(STRINGS);
<STRINGS><<EOF>>            {output::errorLex(yylineno); exit(0);};
<STRINGS>([\x00-\x09\x0b-\x0c\x0e-\x21\x23-\x5b\x5d-\x7f]|((\\)(\\))|((\\)(\"))|((\\)(n))|((\\)(r)))*(\") {BEGIN(INITIAL);return STRING;}
<STRINGS>([^(\")])*((\")?)  {output::errorLex(yylineno); exit(0);};
{whitespace}				;
.                           {output::errorLex(yylineno); exit(0);};


%%