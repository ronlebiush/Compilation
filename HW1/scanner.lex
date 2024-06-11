%{

/* Declarations section */
#include <stdio.h>
#include "tokens.hpp"

%}

%option yylineno
%option noyywrap
digit   		([0-9])
letter  		([a-zA-Z])
whitespace		([\t\n ])
ignoredEscapeSeq       ([\r\\\"\n\t\0])
string          (["])
relop           ((==)|(!=)|(\<=)|(\>=)|(\<)|(\>))
binop           ((\+)|(\-)|(\*)|(\/))

%x STRINGSTART
%%

["]                         BEGIN(STRINGSTART);
";"                         return SC;
"if"                        return IF;
"return"                    return RETURN;
"false"                     return FALSE;
"true"                      return TRUE;
"not"                       return NOT;
"or"                        return OR;
"and"                       return AND;
"bool"                      return BOOL;
"b"                         return B;
"byte"                      return BYTE;
"int"                       return INT;
{digit}+          			return NUM;
{letter}+({letter}|{digit})*		return ID;
{whitespace}				;
<STRINGSTART>[a-zA-Z0-9\\]    return STRING;   
<STRINGSTART>["]            BEGIN(INITIAL);

.		printf("Lex doesn't know what that is!\n");

%%

