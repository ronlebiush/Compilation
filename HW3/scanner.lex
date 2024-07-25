%{

/* Declarations section */
#include <stdio.h>
#include "source.hpp"
#include "parser.tab.hpp"
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
"b"                         {yylval = new Node(yylineno, "B"); return B;}
"bool"                      {yylval = new TypeNode(yylineno, "BOOL");return BOOL;}
"and"                       {yylval = new TypeNode(yylineno, "BOOL"); return AND;}
"or"                        {yylval = new TypeNode(yylineno, "BOOL"); return OR;}
"not"                       {yylval = new TypeNode(yylineno, "BOOL"); return NOT;}
"true"                      {yylval = new TypeNode(yylineno, "BOOL"); return TRUE;}
"false"                     {yylval = new TypeNode(yylineno, "BOOL"); return FALSE;}
"return"                    {yylval = new Node(yylineno, "RETURN"); return RETURN;}
"if"                        {yylval = new Node(yylineno, "IF"); return IF;}
"else"                      {yylval = new Node(yylineno, "ELSE"); return ELSE;}
"while"                     {yylval = new Node(yylineno, "WHILE"); return WHILE;}
"break"                     {yylval = new Node(yylineno, "BREAK"); return BREAK;}
"continue"                  {yylval = new Node(yylineno, "CONTINUE"); return CONTINUE;}
";"                         {yylval = new Node(yylineno, "SC"); return SC;}
"("                         {yylval = new Node(yylineno, "LPAREN"); return LPAREN;}
")"                         {yylval = new Node(yylineno, "RPAREN"); return RPAREN;}
"{"                         {yylval = new Node(yylineno, "LBRACE"); return LBRACE;}
"}"                         {yylval = new Node(yylineno, "RBRACE"); return RBRACE;}
"="                         {yylval = new Node(yylineno, "ASSIGN"); return ASSIGN;}
{relational}                {yylval = new Node(yylineno, "RELATIONAL"); return RELATIONAL;}
{equal}                     {yylval = new Node(yylineno, "EQUAL"); return EQUAL;}
{binop}                     {yylval = new Node(yylineno, "BINOP"); return BINOP;}
{mult}                      {yylval = new Node(yylineno, "MULT"); return MULT;}
{letter}+({letter}|{digit})*		{yylval = new IdNode(yylineno, yytext, ""); return ID;}
"0"                         {yylval = new NumNode(yylineno, stoi(yytext), "INT"); return NUM;}
{nonzero}{digit}*          	{yylval = new NumNode(yylineno, stoi(yytext), "INT"); return NUM;}
{comment}                   ;
(\"([^\n\r\"\\]|\\[rnt"\\])+\")     {yylval = new TypeNode(yylineno, "STRING"); return STRING;}
.                                   {output::errorLex(yylineno); exit(0);};

%%