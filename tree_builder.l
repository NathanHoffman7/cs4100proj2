/* Authors: Nathan Hoffman (nh662219@ohio.edu)              */
/*          Kara Nemet     (kn485120@ohio.edu)              */
/* Brief: This lex program parses a file and tokenizes it.  */
/*        It then outputs the tokens to a file.             */
/* Date: 4-20-2024                                         */
%{
#include <iostream>
#include <fstream>
#include "tree_node.h"
using namespace std;

/* Count the number of lines */
int lineCount = 1;
int n;
int return_int;
void ACC(char c)
{
    if (c == '\n')
    {
        lineCount++;
    }
}
int line_number()
{
    return lineCount;
}

%}
/*------------MACROS------------*/
WHITE     [ \t\n\f\r]
NUM       [0-9]+
L         [a-zA-Z]+
IDENTIFIER ({NUM}|{L})({NUM}|{L}|_)*

/*---------TRANSLATION-RULES--------*/
%%
buildnode    { return TK_BLDNODE; }
for          { return TK_FOR; }
in           { return TK_IN; }
name         { return TK_NAME; }
weight       { return TK_WEIGHT; }
isachildof   { return TK_IsAChildOf; }
\"[^"]*\"     {yylval.s_val = strdup(yytext); return TK_STRING;}
[\{|\}|\(|\)|\,|\;|=|:|\[|\]|+] {return yytext[0];}
{NUM}        { yylval.num = atoi(yytext); return TK_INT; }
{IDENTIFIER} {yylval.s_val = strdup(yytext); return TK_IDENTIFIER;}
{WHITE}     { ACC(yytext[0]); 
                /* Remove tabs/whitespace/newlines */}

%%
