%{

/* Declarations section */
#include <stdio.h>
#include "source.hpp"
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
relational      ((\<=)|(\>=)|(\<)|(\>))
equal           ((==)|(!=))
binop           ((\+)|(\-))
mult            ((\*)|(\/))
backslash       \x5C
printableascii  ([\x20-\x21\x23-\x5B\x5D-\x7E])
comment \/\/[^\n\r]*

%x STRINGS
%x ESCAPESEQ
%x ASCII
%%

{whitespace}				;
"int"                       {yylval = new TypeNode("int"); return INT;}
"byte"                      {yylval = new TypeNode("byte");return BYTE;}
"b"                         return B;
"bool"                      {yylval = new TypeNode("bool");return BOOL;}
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
{relational}                return RELATIONAL;
{equal}                     return EQUAL;
{binop}                     return BINOP;
{mult}                      return MULT;
{letter}+({letter}|{digit})*		{yylval = new IdNode(yytext); return ID;}
"0"                         {yylval = new NumNode(yytext); return NUM;}
{nonzero}{digit}*          	{yylval = new NumNode(yytext); return NUM;}
{comment}                   ;
(\"([^\n\r\"\\]|\\[rnt"\\])+\")     return STRING;
.                                   {output::errorLex(yylineno); exit(0);};

%%