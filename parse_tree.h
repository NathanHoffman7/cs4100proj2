/**
 * @file parse_tree.h
 * @authors Nathan Hoffman (nh662219@ohio.edu)
 *          Kara Nemet     (kn485120@ohio.edu)
 * @brief 
 * @version 0.1
 * @date 04-18-2024
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <map>
#include <string>
#include <set>
#include <algorithm>
#include <iostream>


using namespace std;

class int_expr {
 public:
  virtual int evaluate_expression(map<string, int> &sym_tab) =0;
};


class str_expr {
 public:
     virtual char* evaluate_expression(map<string, int> &sym_tab)=0;
};


class int_constant:public int_expr {
 public:
  int_constant(int val) {saved_val = val;}

  virtual int evaluate_expression(map<string, int> &sym_tab) {
    return saved_val;
  }
 private:
  int saved_val;
};

class variable: public int_expr {
 public:
  variable(char *in_val) {//cout << "Found variable = " << in_val << endl; 
                          saved_val =in_val;}

  virtual int evaluate_expression(map<string, int> &sym_tab) {

    map<string,int>::iterator p;
    p =sym_tab.find(saved_val);
    //cout << "Looking up variable " << saved_val << endl;
    if (p!=sym_tab.end()) {
      //cout << "Returning value of variable " << saved_val << endl;
      //cout << "= " << p->second << endl;

      return p->second;
    } else {
      // ERROR .... for now return -1;
      // Should throw error!
      return -1;
    }

  }
 private:
  string saved_val;
  
};

class add_expr: public int_expr {
 public:
  add_expr(int_expr *left, int_expr *right) {
    l = left;
    r = right;
  }

  virtual int evaluate_expression(map<string, int> &sym_tab) {
    return l->evaluate_expression(sym_tab) + r->evaluate_expression(sym_tab);
  }
  
  private:
    int_expr *l;
    int_expr *r;
};

class statement {
 public:
  virtual void evaluate_statement(map<string, int> &sym_tab) =0;
};

class compound_statement: public statement {
 public:
  compound_statement(statement *first, compound_statement *rest) {
    f = first;
    r = rest;
  }
  
  virtual void evaluate_statement(map<string, int> &sym_tab) {
    if (f!=NULL) {
      f->evaluate_statement(sym_tab);
    }
    if (r!=NULL) {
      r->evaluate_statement(sym_tab);
    }
  }
 private:
  compound_statement *r;
  statement *f;
};
  

// class while_statement: public statement {
//  public:
//   while_statement(boolean_expression *cond, compound_statement *body) {
//     c=cond;
//     b=body;
//   }

//   virtual void evaluate_statement(map<string, int> &sym_tab) {
//     while (c->evaluate_expression(sym_tab)) {
//       b->evaluate_statement(sym_tab);
//     }
//   }
    

    
//   private:
//     boolean_expression *c;
//     compound_statement *b;
//   };

class assignment_statement: public statement {

 public:
  assignment_statement(char *id, int_expr *rhs) {
    ident = id;
    r_side = rhs;
  }
  virtual void evaluate_statement(map<string, int> &sym_tab) {
    
    int temp = r_side->evaluate_expression(sym_tab);

    //cout << "Assigning" << ident << " to " << temp << endl;

    sym_tab[ident]=temp;
  }


  private: 
    string ident;
    int_expr *r_side;
  };

