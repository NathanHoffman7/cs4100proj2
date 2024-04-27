%start start_var
%token TK_BLDNODE TK_FOR TK_IN TK_NAME TK_WEIGHT TK_IsAChildOf TK_STRING TK_INT TK_IDENTIFIER
%left '+'

%{
    #include <stdio.h>
    #include <map>
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

    Map* my_node_map = create_map();
    vector<string> my_str_list;
    vector<int> my_int_list;

%}

%union {
    char* s_val;
    int num;
    int_expr *int_ptr;
    str_expr *str_ptr;
    statement *s_ptr;
    compound_statement *c_ptr;
    
}

%type <s_val> TK_STRING TK_IDENTIFIER 
%type <str_ptr> str_expr str_list 
%type <num>   TK_INT
%type <int_ptr> int_expr
%type <s_ptr> statement for_statement buildnode_statement
%type <c_ptr> prog start_var

%%

start_var : prog { $$ = $1; 
                
                cout << "Trying to print map\n";
                TreeNode* root = find_map(my_node_map, "root");
                print_map(root);
                cout << endl;
}
    ;

prog : statement prog { $$ = new compound_statement($1, $2); }
    | {$$ = NULL;}
    ;

statement : for_statement { $$ = $1; cout << "For Statement\n"; }
          | buildnode_statement { $$ = $1; cout << "Building Node Statement\n";
          }
          ;

buildnode_statement : TK_BLDNODE '{' TK_NAME '=' str_expr ';' TK_WEIGHT '=' int_expr ';' TK_IsAChildOf '=' str_expr ';' '}' ';'
{ 
    cout << "Name: " << $5 << "\nWeight: " << $9 << "\nIsAChildOf: " << $13 << endl;
    $$ = new buildnode_statement($5, $9, $13);
}
| TK_BLDNODE '{' TK_NAME '=' str_expr ';' TK_WEIGHT '=' int_expr ';' '}' ';'
{ 
    cout << "Name: " << $5 << "\nWeight: " << $9 << endl;
    $$ = new buildnode_statement($5, $9, NULL);
}
;

for_statement : TK_FOR TK_IDENTIFIER TK_IN '[' TK_INT ':' TK_INT ']' '{' prog '}'
{
    $$ = new for_statement($2, $5, $7, $10);
}

int_expr: TK_INT { $$ = new int_const($1); }
    | TK_IDENTIFIER { $$ = new int_var(atoi($1)); }
    | int_expr '+' int_expr { $$ = new int_add_expr($1, $3); }
;

str_expr: TK_STRING { $$ = new str_const($1); }
    | TK_IDENTIFIER { $$ = new str_var($1); }
    | str_expr '+' str_expr { $$ = new str_add_expr($1, $3); }
;

str_list: str_expr { $$ = $1;}
    | str_list ',' str_expr { $$ = new str_list($1, $3);}
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