%{

/* Declarations section */
#include <stdio.h>
#include "tokens.hpp"

char handleAscii(char first, char sec);


%}

%option yylineno
%option noyywrap
digit   		([0-9])
letter  		([a-zA-Z])
whitespace		([\t\n ])
string          (["])
relop           ((==)|(!=)|(\<=)|(\>=)|(\<)|(\>))
binop           ((\+)|(\-)|(\*)|(\/))
backslash       \x5C
printableascii  ([\x20-\x21\x23-\x5B\x5D-\x7E])
comment \/\/[^\n\r]*

%x STR
%x ESCAPESEQ
%x ASCII
%%

\"                          BEGIN(STR);
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
{comment}                   return COMMENT;
{letter}+({letter}|{digit})*		return ID;
{digit}+          			return NUM;
{whitespace}				;


<STR>{backslash}            BEGIN(ESCAPESEQ);

<ESCAPESEQ>"n"              *textbuffptr = '\n'; textbuffptr++; BEGIN(STR);
<ESCAPESEQ>"r"              *textbuffptr = '\r'; textbuffptr++; BEGIN(STR);
<ESCAPESEQ>"t"              *textbuffptr = '\t'; textbuffptr++; BEGIN(STR);
<ESCAPESEQ>{backslash}      *textbuffptr = '\\'; textbuffptr++; BEGIN(STR);
<ESCAPESEQ>"0"              *textbuffptr = '\0'; textbuffptr++; BEGIN(STR);
<ESCAPESEQ>x[0-9A-Za-z][0-9A-Za-z]   *textbuffptr = handleAscii(yytext[1], yytext[2]) ; textbuffptr++; BEGIN(STR);
<ESCAPESEQ>.                printf("Error undefined escape sequence %s\n", yytext); exit(0);

<STR>(\x0A|\x0D)            printf("Error unclosed string\n"); exit(0);

<STR>{printableascii}       *textbuffptr =  *yytext ; textbuffptr++;
<STR>["]                    {
                                BEGIN(INITIAL);
                                *textbuffptr ='\0';
                                return STRING;
                            }
.		                    printf("Error %s\n", yytext);

%%

char handleAscii(char first, char sec)
{
    //printf("the chars are: %c%c%c\n", yytext[0], yytext[1], yytext[2]);
    //printf("%c %c \n",first,sec);

    char hex_str[3]; // array to store the hex string
    char *end_ptr;   // pointer to track the end of the parsed string
    
    // Form the hex string from the characters
    hex_str[0] = first;
    hex_str[1] = sec;
    hex_str[2] = '\0';

    // Convert hex string to integer
    int current_hex = strtol(hex_str, &end_ptr, 16);

    // Check for valid hex conversion and ASCII range
    if (*end_ptr == '\0' && current_hex >= 0x00 && current_hex <= 0x7F) {
        return (char)current_hex;
    } else {
        if (first == '\0')
            printf("Error undefined escape sequence x\n");
        else if (sec == '\0')
            printf("Error undefined escape sequence x%c\n", first);
        else
            printf("Error undefined escape sequence x%c%c\n", first, sec);
        
        exit(EXIT_FAILURE);
    }
}