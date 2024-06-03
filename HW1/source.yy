
#include "tokens.hpp"
#include <stdio.h>

void showToken(char*);


%option noyywrap


%%
digit ([0-9])

%%
(digit)+            showToken("NUM");

%%

void showToken(char* name){
    print("hello\n");
}