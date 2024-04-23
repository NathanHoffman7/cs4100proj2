/* Authors: Nathan Hoffman (nh662219@ohio.edu)              */
/*          Kara Nemet     (kn485120@ohio.edu)              */
/* Brief: This lex program parses a file and tokenizes it.  */
/*        It then outputs the tokens to a file.             */
/* Date: 4-20-2024                                         */
%{
#include <iostream>
#include <fstream>
using namespace std;

// Define the token types (need some revision)
#define TK_BLDNODE 1
#define TK_FOR 2
#define TK_IN 3
#define TK_NAME 4
#define TK_WEIGHT 5
#define TK_IsAChildOf 6
#define TK_ASSIGN 7
#define TK_STRING 8
#define TK_SYMBOL 9



/* Count the number of lines */
int lineCount = 1;
int n;
char *return_string;
char return_char;
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
{L}          {return_string = yytext; return TK_ASSIGN;}
{NUM}        {return_string = yytext; return TK_ASSIGN;}
[\{|\}|\(|\)|\,|\;|=|:|\[|\]|+]  { return_char = yytext[0]; return TK_SYMBOL; }

%%
/*------------AUX-CODE------------*/
int yywrap()
{
    return 1;
}

int main(int argc, char *argv[])
{
    int token;
    ofstream out;
    out.open("test_out.txt");
    while (token = yylex())
    {
        switch (token)
        {
        case TK_BLDNODE:
            out << "TK_BLDNODE" << endl;
            break;
        case TK_FOR:
            out << "TK_FOR" << endl;
            break;
        case TK_IN:
            out << "TK_IN" << endl;
            break;
        case TK_NAME:
            out << "TK_NAME" << endl;
            break;
        case TK_WEIGHT:
            out << "TK_WEIGHT" << endl;
            break;
        case TK_IsAChildOf:
            out << "TK_IsAChildOf" << endl;
            break;
        case TK_ASSIGN:
            out << return_string << endl;
            break;
        case TK_STRING:
            out << return_string << endl;
            break;
        case TK_SYMBOL:
            out << return_char << endl;
            break;
        default:
            break;
        }
    }
}