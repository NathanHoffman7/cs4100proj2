%start start_var
%token TK_BLDNODE TK_FOR TK_IN TK_NAME TK_WEIGHT TK_IsAChildOf TK_STRING TK_INT TK_IDENTIFIER
%left '+'

%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "parse_tree.h"
    #include "tree_node.h"

    #include <iostream>
    using namespace std;
%}
%{
    int yylex();
    void yyerror(char *string);

    Map* my_sym_tab = new Map();
%}

%union {
    char* s_val;
    int num;
    int_expr *int_ptr;
    str_expr *str_ptr;
    statement *s_ptr;
    compound_statement *c_ptr;
    
}

%type <s_val> TK_STRING TK_IDENTIFIER TK_INT
%type <s_val> str_expr str_list int_expr
%type <s_ptr> statement for_statement buildnode_statement
%type <c_ptr> prog start_var

%%

start_var : prog { cout << "Start Var\n";}

prog : statement prog { cout << "Statement\n"; }
    | {$$ = NULL;}
    ;

statement : for_statement { $$ = $1; cout << "For Statement\n"; }
          | buildnode_statement { $$ = $1; cout << "Building Node Statement\n";}
          ;

buildnode_statement : TK_BLDNODE '{' TK_NAME '=' str_expr ';' TK_WEIGHT '=' int_expr ';' TK_IsAChildOf '=' str_expr ';' '}' ';'
{ cout << "Name: " << $5 << "\nWeight: " << $9 << "\nIsAChildOf: " << $13 << endl;}
    | TK_BLDNODE '{' TK_NAME '=' str_expr ';' TK_WEIGHT '=' int_expr ';' '}' ';'
{ cout << "Name: " << $5 << "\nWeight: " << $9 << endl;}
    ;

for_statement : TK_FOR TK_IDENTIFIER TK_IN '[' str_list ']' '{' prog '}'
{cout << "\nFor Statement:\n" << "Identifier: " << $2 << "\nList: " << $5 << endl;}
    ;

str_expr: TK_STRING { $$ = $1;}
    | TK_IDENTIFIER { $$ = $1;}
    | str_expr '+' str_expr{}
    ;

str_list: str_expr { $$ = $1;}
    | str_list ',' str_expr{}
    | TK_INT ':' TK_INT{}
    ;

int_expr: TK_INT {$$ = $1;}
    | int_expr '+' int_expr{}
    | TK_IDENTIFIER '+' int_expr{}
    ;

%%
#include "lex.yy.c"

void yyerror(char *err_string)
{
    cout << "Error: " << err_string;
    cout << " at line: " << line_number();
    cout << ", unexpected token: " << yytext << endl;
    exit(-1);
}

main()
{
    yyparse();
}