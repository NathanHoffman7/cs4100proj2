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

// Define the token types (need some revision)
#define TK_BLDNODE 1
#define TK_FOR 2
#define TK_IN 3
#define TK_NAME 4
#define TK_WEIGHT 5
#define TK_IsAChildOf 6
#define TK_INT 7
#define TK_STRING 8
#define TK_IDENTIFIER 9



/* Count the number of lines */
int lineCount = 1;
int n;
char *return_string;
char *return_identifier;
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
STRING    \"[^"]*\"
IDENTIFIER ({NUM}|{L})({NUM}|{L}|_)*

/*---------TRANSLATION-RULES--------*/
%%
{WHITE}     { ACC(yytext[0]); 
                /* Remove tabs/whitespace/newlines */}
buildnode    { return TK_BLDNODE; }
for          { return TK_FOR; }
in           { return TK_IN; }
name         { return TK_NAME; }
weight       { return TK_WEIGHT; }
isachildof   { return TK_IsAChildOf; }
{STRING}     {return_string = yytext; return TK_STRING;}
{NUM}        { return_int = atoi(yytext); return TK_INT; }
{IDENTIFIER} {return_identifier = yytext; return TK_IDENTIFIER;}
[\{|\}|\(|\)|\,|\;|=|:|\[|\]|+] {return yytext[0];}

%%
