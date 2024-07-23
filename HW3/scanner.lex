%{

/* Declarations section */
#include <stdio.h>
#include "source.hpp"
#include "parser.tab.hpp"
#include "output.hpp"
#include "hw3_output.hpp"

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
"int"                       {yylval = new TypeNode(yylineno, "INT"); return INT;}
"byte"                      {yylval = new TypeNode(yylineno, "BYTE");return BYTE;}
"b"                         {yylval = new Node(yylineno); return B;}
"bool"                      {yylval = new TypeNode(yylineno, "BOOL");return BOOL;}
"and"                       {yylval = new TypeNode(yylineno, "BOOLEXP"); return AND;}
"or"                        {yylval = new TypeNode(yylineno, "BOOLEXP"); return OR;}
"not"                       {yylval = new TypeNode(yylineno, "BOOLEXP"); return NOT;}
"true"                      {yylval = new TypeNode(yylineno, "BOOLEXP"); return TRUE;}
"false"                     {yylval = new TypeNode(yylineno, "BOOLEXP"); return FALSE;}
"return"                    {yylval = new Node(yylineno); return RETURN;}
"if"                        {yylval = new Node(yylineno); return IF;}
"else"                      {yylval = new Node(yylineno); return ELSE;}
"while"                     {yylval = new Node(yylineno); return WHILE;}
"break"                     {yylval = new Node(yylineno); return BREAK;}
"continue"                  {yylval = new Node(yylineno); return CONTINUE;}
";"                         {yylval = new Node(yylineno); return SC;}
"("                         {yylval = new Node(yylineno); return LPAREN;}
")"                         {yylval = new Node(yylineno); return RPAREN;}
"{"                         {yylval = new Node(yylineno); return LBRACE;}
"}"                         {yylval = new Node(yylineno); return RBRACE;}
"="                         {yylval = new Node(yylineno); return ASSIGN;}
{relational}                {yylval = new Node(yylineno); return RELATIONAL;}
{equal}                     {yylval = new Node(yylineno); return EQUAL;}
{binop}                     {yylval = new Node(yylineno); return BINOP;}
{mult}                      {yylval = new Node(yylineno); return MULT;}
{letter}+({letter}|{digit})*		{yylval = new IdNode(yylineno, yytext); return ID;}
"0"                         {yylval = new NumNode(yylineno, yytext); return NUM;}
{nonzero}{digit}*          	{yylval = new NumNode(yylineno, yytext); return NUM;}
{comment}                   ;
(\"([^\n\r\"\\]|\\[rnt"\\])+\")     return STRING;
.                                   {output::errorLex(yylineno); exit(0);};

%%