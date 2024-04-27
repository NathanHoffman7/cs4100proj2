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
    int idx = 0;

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
%type <s_val> str_expr str_list 
%type <num>   int_expr TK_INT
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
    TreeNode* new_node = create_node($5, $9, $13);
    TreeNode* parent = find_map(my_node_map, $13);
    if (parent == NULL)
    {
        cout << "Parent not found\n";
        exit(-1);
    }
    parent->children.push_back(new_node);

    insert_map(my_node_map, $5, new_node);
    $$ = new buildnode_statement($5, $9, $13);
}
| TK_BLDNODE '{' TK_NAME '=' str_expr ';' TK_WEIGHT '=' int_expr ';' '}' ';'
{ 
    cout << "Name: " << $5 << "\nWeight: " << $9 << endl;
    TreeNode* new_node = create_node($5, $9, NULL);

    insert_map(my_node_map, $5, new_node);
    $$ = new buildnode_statement($5, $9, NULL);
}
;

for_statement : TK_FOR TK_IDENTIFIER TK_IN '[' TK_INT ':' TK_INT ']' '{' prog '}'
{
    idx = 0;
    my_int_list.clear();
    for (int i = $5; i <= $7; i++) {
        my_int_list.push_back(i);
    }
    if (!my_int_list.empty()) {
        cout << "\nFor Statement:\n" << "Identifier: " << $2 << "\nValues: ";
    for (int i = 0; i < my_int_list.size(); i++) {
        cout << my_int_list[i] << " ";
    }
        cout << "\nList: " << $5 << endl;
    } else {
        // yyerror("Undefined identifier");
    }
    $$ = new for_statement($2, $5, $7, $10);
}

int_expr: TK_INT { $$ = $1; }
    | TK_IDENTIFIER 
{ 
    if (my_int_list.size() > 0) {
        $$ = my_int_list[idx]; // Use the current idx in the vector
    } else {
        // yyerror("Undefined identifier");
        $$ = 0; // Use a default value
    }
}
    | TK_INT '+' int_expr { $$ = $1 + $3; }
    | TK_IDENTIFIER '+' int_expr 
{ 
    if (my_int_list.size() > 0) {
        $$ = my_int_list[idx] + $3; // Use the current idx in the vector
    } else {
        // yyerror("Undefined identifier");
        $$ = $3; // Use the other operand as the result
    }
}
    | int_expr '+' TK_IDENTIFIER
{
    if (my_int_list.size() > 0) {
        $$ = $1 + my_int_list[idx]; // Use the current idx in the vector
    } else {
        // yyerror("Undefined identifier");
        $$ = $1; // Use the other operand as the result
    }
}
;

str_expr: TK_STRING { 
    string str = std::string($1);
    if (str[0] == '\"' && str[str.length()-1] == '\"')
        str = str.substr(1, str.length()-2);
    $$ = strdup(str.c_str());
}
    | TK_IDENTIFIER 
{ 
    if (my_str_list.size() > 0){
        $$ = strdup(my_str_list[idx].c_str()); // Use the current idx in the vector
    } else {
        // yyerror("Undefined identifier");
        $$ = strdup(""); // Use a default value
    }
}
    | str_expr '+' TK_IDENTIFIER
{ 
    string left = $1;
    string right = $3;
    if (my_str_list.size() > 0) {
        right = my_str_list[idx]; // Use the current idx in the vector
    }
    $$ = strdup((left + right).c_str()); 
};

str_list: str_expr { $$ = $1;}
    | str_list ',' str_expr{}
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