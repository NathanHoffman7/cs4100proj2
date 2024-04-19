
%start start_var
%token TKPRINT TKWHILE TKID TKEE TKLE TKGE TKINT
%left '*' '/' '%'
%left '+' '-'

%{
#include <map>
#include <string>
#include "parse_tree.h"

using namespace std;

%}

%union {
  char* s_val;
  integer_expression *int_ptr;
  boolean_expression *bool_ptr;
  statement *s_ptr;
  compound_statement *c_ptr;
}

%{
extern int yylex();
extern void yyerror(char *String);  

#include <iostream>
 using namespace std;

%}

%type <s_val> TKID TKINT
%type <int_ptr> integer_expression
%type <bool_ptr> boolean_expr
%type <s_ptr> statement while_statement assignment_statement print_statement
%type <c_ptr> prog start_var
%%
start_var : prog { // At this point, the 
                   // the program is done --- let's evaluate the
                   // program
                   map<string,int> my_sym_tab;
                   $$= $1;
                   $1->evaluate_statement(my_sym_tab);
}

prog: statement  prog {$$ = new compound_statement($1,$2);}
    | {$$ = NULL;}
    ;
statement: while_statement {$$ = $1;}
         | assignment_statement {$$ = $1;}
         | print_statement {$$ = $1;}
         ;

print_statement: TKPRINT integer_expression ';' {$$ = new print_statement($2);}
               ;
assignment_statement: TKID '=' integer_expression ';' { //cout << "TKID = " << $1 << endl;
                                                        $$ = new assignment_statement($1, $3);}
                    ;  

while_statement: TKWHILE '(' boolean_expr ')' '{' prog '}' {$$ = new while_statement($3, $6);}
               ;

integer_expression: TKINT {//cout << "Integer: " << $1 << endl;
		      $$ = new int_constant(atoi($1));}
| TKID {//cout << "Identifier: " << $1 << endl; 
		      $$= new variable($1); }
                  | '-' integer_expression {$$=new neg_constant($2);}
                  | integer_expression '+' integer_expression {$$=new plus_expr($1,$3);}
                 
                  | integer_expression '-' integer_expression {$$=new minus_expr($1,$3);}
                  | integer_expression '*' integer_expression {$$=new mult_expr($1,$3);}
                  | integer_expression '/' integer_expression {$$=new div_expr($1,$3);}
                  | integer_expression '%' integer_expression {$$=new mod_expr($1,$3);}
                  | '(' integer_expression ')' {$$ = $2;} 
                  ;

boolean_expr:  integer_expression '<' integer_expression {$$=new less_expr($1,$3);}
|  integer_expression '>' integer_expression {$$ = new greater_expr($1,$3);}
|  integer_expression TKEE integer_expression {$$= new ee_expr($1,$3); }
|  integer_expression TKLE integer_expression {$$= new le_expr($1,$3); }
|  integer_expression TKGE integer_expression {$$= new ge_expr($1,$3); }
            ;
%%
#include "lex.yy.c"

void yyerror(char *error_string)
{
  cout << "Error : " << error_string << "  on line " << line_nmb() << endl;
  exit(-1);
}


main() {
  yyparse();
}
