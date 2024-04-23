%start start_var
%token TK_STRING TK_INT TK_IDENTIFIER
%token TK_BLDNODE TK_FOR TK_IN TK_NAME TK_WEIGHT TK_IsAChildOf
%left '+'

%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <map>
    #include "parse_tree.h"
    #include "tree_node.h"

    #include <iostream>
    using namespace std;
%}
%{
    extern int yylex();
    extern void yyerror(char *string);
%}

%union {
    char* str;
    int num;
    int_expr *int_ptr;
    str_expr *str_ptr;
    statement *s_ptr;
    compound_statement *c_ptr;
    
}

%type <str> TK_STRING TK_IDENTIFIER
%type <num> TK_INT
%type <str_ptr> str_expr str_list
%type <int_ptr> int_expr
%type <s_ptr> statement for_statement buildnode_statement
%type <c_ptr> prog start_var

%%

start_var : prog {  map<string, TreeNode> my_sym_tab;
                $$ = $1;
                $1->evaluate_statement(my_sym_tab);}

prog : statement prog{ $$ = new compound_statement($1, $2);}
    | {$$ = NULL;}
    ;

statement : for_statement { $$ = $1;}
          | buildnode_statement { $$ = $1;}
          ;


buildnode_statement : TK_BLDNODE '{' TK_NAME '=' str_expr ';' TK_WEIGHT '=' int_expr ';' TK_IsAChildOf '=' str_expr '}'
{$$ = new buildnode_statement(my_sym_tab, $5, $9, $13);}
    ;

for_statement : TK_FOR TK_IDENTIFIER TK_IN '[' str_list ']' '{' prog '}' {$$ = new for_statement(my_sym_tab, $2, $5, $8);}
    | TK_FOR TK_IDENTIFIER TK_IN '[' TK_INT ':' TK_INT ']' '{' prog '}' {$$ = new for_statement(my_sym_tab, $2, $5, $7, $10);}
    ;

str_expr: TK_STRING { $$ = new str_expr($1); }
    ;

str_list: str_expr { $$ = new str_list($1);}
    | str_list ',' str_expr { $$ = new str_list($1, $3);}
    ;

int_expr: TK_INT { $$ = new int_constant(atoi($1));}
    | int_expr '+' int_expr { $$ = new add_expr($1, $3);}
    ;
    



%%
#include "lex.yy.c"

void yyerror(char *err_string)
{
    cout << "Error : " << err_string << " on line: " << line_number() << endl;
    exit(-1);
}

main()
{
    yyparse();
}