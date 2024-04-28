/* Authors: Nathan Hoffman (nh662219@ohio.edu)              */
/*          Kara Nemet     (kn485120@ohio.edu)              */
/* Brief: This yacc program parses the grammar and performs  */
/*        syntactic analysis                                 */
/* Date: 4-20-2024                                         */


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

    nodeMap node_map;
    intMap int_map;
    strMap str_map;
%}

%union {
  char* s_val;
  int num;
  int_expr *int_ptr;
  str_expr *str_ptr;
  str_list *str_list_ptr;
  statement *s_ptr;
  compound_statement *c_ptr;
}

%type <s_val> TK_STRING TK_IDENTIFIER 
%type <num> TK_INT
%type <int_ptr> int_expr
%type <str_ptr> str_expr 
%type <str_list_ptr> str_list
%type <s_ptr> statement for_statement buildnode_statement
%type <c_ptr> prog start_var multi_bn_statement

%%
start_var : prog { 
                    
                    $$= $1;
                    $1->evaluate_statement(node_map, int_map, str_map);
                    TreeNode* root = find_map(node_map, "root");
                    print_map(root);
                    cout << endl;
}

prog: statement  prog {$$ = new compound_statement($1,$2);}
    | {$$ = NULL;}
    ;
statement: buildnode_statement {$$ = $1;}
         | for_statement {$$ = $1;}
         ;

buildnode_statement: TK_BLDNODE '{' TK_NAME '=' str_expr ';' TK_WEIGHT '=' int_expr ';' '}' ';' 
{
                    $$ = new buildnode_statement($5, $9, NULL);
}
                    | TK_BLDNODE '{' TK_NAME '=' str_expr ';' TK_WEIGHT '=' int_expr ';' TK_IsAChildOf '=' str_expr ';' '}' ';' 
{
                    $$ = new buildnode_statement($5, $9, $13);
}
;

str_expr: TK_STRING { $$ = new str_constant($1); }
        | str_expr '+' TK_IDENTIFIER { $$ = new add_str_expr($1,new int_var($3)); }
        | TK_IDENTIFIER { $$ = new str_var($1); }
        ;

int_expr: TK_INT { $$ = new int_constant($1);}
        | TK_IDENTIFIER { $$ = new int_var($1); }
        | int_expr '+' int_expr { $$ = new add_int_expr($1, $3); }
        ;

for_statement: TK_FOR TK_IDENTIFIER TK_IN '[' TK_INT ':' TK_INT ']' '{' multi_bn_statement '}'
{
            $$ = new for_statement(new int_var($2), $5, $7, $10);
}
            | TK_FOR TK_IDENTIFIER TK_IN '[' str_list ']' '{' multi_bn_statement '}' 
{
            $$ = new str_for_statement(new str_var($2), $5, $8);
}
;
multi_bn_statement: buildnode_statement multi_bn_statement { $$ = new compound_statement($1, $2); }
            | { $$ = NULL; }
            ;

            /* I use TK_IDENTIFIER because TK_STRING has been used for string literals */
str_list: TK_IDENTIFIER { $$ = new str_list($1, NULL); }
            | TK_IDENTIFIER ',' str_list { $$ = new str_list($1, $3); }
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