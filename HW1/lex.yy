
%{
#include "HW1/tokens.hpp"
#include <stdio.h>

void showToken(char*);
%}

%option yylineno
%option noyywrap
digit       ([0-9])
letter      ([a-zA-Z])
whitespace  ([\t\n ])

%%
([0-9])+            showToken((char*)"number");
(digit)+            showToken((char*)"number");
(whitespace)        ;
.                   printf("nothing\n");

%%

void showToken(char* name){
    printf("hello\n");
}