%{
/* Declartions Section */
#include <stdio.h>
#include <iostream>
#include "tokens.hpp"
using std::string;
void showToken(string);

#define UNCLOSED 29
#define BAD_ESCAPE 30
#define INVALID_HEX 31
#define UNKNOWN 32
%}

%option yylineno
%option noyywrap
digit ([0-9])
letter ([a-zA-Z])
whitespace ([ \t\n\r])
afterBacklash([ntr0\"\\])
validHex1 (0[9ADad])
validHex2 ([2-6][0-9A-Fa-f])
validHex3 (7[0-9A-Ea-e])
strChars ([\x9\x20-\x21\x23-\x5B\x5D-\x7E])
%%
"void" {showToken("VOID"); return VOID;}
"int" {showToken("INT"); return INT;}
"byte" {showToken("BYTE"); return BYTE;}
"b" {showToken("B"); return B;}
"bool" {showToken("BOOL"); return BOOL;}
"and" {showToken("AND"); return AND;}
"or" {showToken("OR"); return OR;}
"not" {showToken("NOT"); return NOT;}
"true" {showToken("TRUE"); return TRUE;}
"false" {showToken("FALSE"); return FALSE;}
"return" {showToken("RETURN"); return RETURN;}
"if" {showToken("IF"); return IF;}
"else" {showToken("ELSE"); return ELSE;}
"while" {showToken("WHILE"); return WHILE;}
"break" {showToken("BREAK"); return BREAK;}
"continue" {showToken("CONTINUE"); return CONTINUE;}
";" {showToken("SC"); return SC;}
"(" {showToken("LPAREN"); return LPAREN;}
")" {showToken("RPAREN"); return RPAREN;}
"{" {showToken("LBRACE"); return LBRACE;}
"}" {showToken("RBRACE"); return RBRACE;}
"=" {showToken("ASSIGN"); return ASSIGN;}
"=="|"!="|"<"|">"|"<="|">=" {showToken("RELOP"); return RELOP;}
"+"|"-"|"*"|"/" {showToken("BINOP"); return BINOP;}
"//"[^\n\r]*  { return COMMENT;}
[a-zA-Z]({letter}|{digit})* {showToken("ID"); return ID;}
0|[1-9]{digit}* {showToken("NUM"); return NUM;}
\"({strChars}|\\{afterBacklash}|\\x({validHex1}|{validHex2}|{validHex3}))*\"   {return STRING;}  
\"({strChars}|\\{afterBacklash}|\\x({validHex1}|{validHex2}|{validHex3}))*  {return UNCLOSED;}
\"({strChars}|\\{afterBacklash}|\\x({validHex1}|{validHex2}|{validHex3}))*\\[^ntr0\"\\]     {return BAD_ESCAPE;}
\"({strChars}|\\{afterBacklash}|\\x({validHex1}|{validHex2}|{validHex3}))*\\x0[^9ADad] {return INVALID_HEX;}
\"({strChars}|\\{afterBacklash}|\\x({validHex1}|{validHex2}|{validHex3}))*\\x[2-6][^0-9A-Fa-f] {return INVALID_HEX;}
\"({strChars}|\\{afterBacklash}|\\x({validHex1}|{validHex2}|{validHex3}))*\\x7[^0-9A-Ea-e] {return INVALID_HEX;}
\"({strChars}|\\{afterBacklash}|\\x({validHex1}|{validHex2}|{validHex3}))*\\x[^02-67]. {return INVALID_HEX;}
\"({strChars}|\\{afterBacklash}|\\x({validHex1}|{validHex2}|{validHex3}))*\\x[^02-67] {return INVALID_HEX;}
{whitespace}+     ;  // ignore whitespace

. {return UNKNOWN;};
%%
//validhex ((\x0[9AD])| \x[2-6][0-9A-Fa-f] | x[7][0-9A-Ea-e])
//validhex (x[0-7][0-9A-Fa-f])
//\\x(0[^9ADad]| [2-6][^0-9A-Fa-f]| 7[^0-9A-Ea-e]| [^02-67]) 
// showToken should print in the following foramt <line number> <token name> <value>
// note that line number refers to the line number where the token ENDS not where it starts
// value refers to the lexeme excluding comments and strings
void showToken(string tokenName)
{
    printf("%d %s %s\n", yylineno, tokenName.c_str(), yytext);
}