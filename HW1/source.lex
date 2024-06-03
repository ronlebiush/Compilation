% {
#include "tokens.hpp"
#include <stdio.h>
#include <iostream>

void showToken(char*);
% }

%option noyywrap


%%
digit ([0-9])

%%
(digit)+            showToken("NUM");

%%

void showToken(char* name){
    std::cout << yylineno << name << yytext << std::endl;
}