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

start_var : prog { $$ = $1; 
                
                cout << "Trying to print map\n";
                Node* root = my_sym_tab->head;
                print_map(my_sym_tab, root);}
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
    TreeNode* new_node = create_node($5, $9, $13);
    if (new_node == NULL) {
        cout << "Error: Failed to create new node." << endl;
        exit(1);
    }
    if (my_sym_tab == NULL) {
        cout << "Error: Symbol table is not initialized." << endl;
        exit(1);
    }
    insert_map(my_sym_tab, $5, new_node);
    $$ = new buildnode_statement($5, $9, $13);
}
| TK_BLDNODE '{' TK_NAME '=' str_expr ';' TK_WEIGHT '=' int_expr ';' '}' ';'
{ 
    cout << "Name: " << $5 << "\nWeight: " << $9 << endl;
    TreeNode* new_node = create_node($5, $9, NULL);
    if (new_node == NULL) {
        cout << "Error: Failed to create new node." << endl;
        exit(1);
    }
    if (my_sym_tab == NULL) {
        cout << "Error: Symbol table is not initialized." << endl;
        exit(1);
    }
    insert_map(my_sym_tab, $5, new_node);
    $$ = new buildnode_statement($5, $9, NULL);
}
;

for_statement : TK_FOR TK_IDENTIFIER TK_IN '[' int_expr ':' int_expr ']' '{' prog '}'
{cout << "\nFor Statement:\n" << "Identifier: " << $2 << "\nList: " << $5 << endl;}
    | TK_FOR TK_IDENTIFIER TK_IN '[' str_list ']' '{' prog '}'
{cout << "\nFor Statement:\n" << "Identifier: " << $2 << "\nList: " << $5 << endl;}
    ;

str_expr: TK_STRING { 
    string str = std::string($1);
    if (str[0] == '\"' && str[str.length()-1] == '\"')
        str = str.substr(1, str.length()-2);
    $$ = strdup(str.c_str());
}
    | TK_IDENTIFIER { $$ = $1;}
    | str_expr '+' str_expr{ }
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