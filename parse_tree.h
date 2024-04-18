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

class integer_expression {
 public:
  virtual int evaluate_expression(map<string, int> &sym_tab) =0;
};


class boolean_expression {
 public:
     virtual bool evaluate_expression(map<string, int> &sym_tab)=0;
};


class int_constant:public integer_expression {
 public:
  int_constant(int val) {saved_val = val;}

  virtual int evaluate_expression(map<string, int> &sym_tab) {
    return saved_val;
  }
 private:
  int saved_val;
};

class variable: public integer_expression {
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

class neg_constant: public integer_expression {
 public:
  neg_constant(integer_expression *ptr) {
    eval_ptr = ptr;
  }
  
  virtual int evaluate_expression(map<string, int> &sym_tab) {
    return -eval_ptr->evaluate_expression(sym_tab);
  }
  integer_expression *eval_ptr;
};



class plus_expr: public integer_expression {
 public:
  plus_expr(integer_expression *left, integer_expression *right) {
    l = left;
    r = right;
  }

  virtual int evaluate_expression(map<string, int> &sym_tab) {
    return l->evaluate_expression(sym_tab) + r->evaluate_expression(sym_tab);
  }
  
  private:
    integer_expression *l;
    integer_expression *r;
};

class minus_expr: public integer_expression {
 public:
  minus_expr(integer_expression *left, integer_expression *right) {
    l = left;
    r = right;
  }

  virtual int evaluate_expression(map<string, int> &sym_tab) {
    return l->evaluate_expression(sym_tab) - r->evaluate_expression(sym_tab);
  }
  
  private:
    integer_expression *l;
    integer_expression *r;
};

class mult_expr: public integer_expression {
 public:
  mult_expr(integer_expression *left, integer_expression *right) {
    l = left;
    r = right;
  }

  virtual int evaluate_expression(map<string, int> &sym_tab) {
    return l->evaluate_expression(sym_tab) * r->evaluate_expression(sym_tab);
  }
  
  private:
    integer_expression *l;
    integer_expression *r;
};


class div_expr: public integer_expression {
 public:
  div_expr(integer_expression *left, integer_expression *right) {
    l = left;
    r = right;
  }

  virtual int evaluate_expression(map<string, int> &sym_tab) {
    return l->evaluate_expression(sym_tab) / r->evaluate_expression(sym_tab);
  }
  
  private:
    integer_expression *l;
    integer_expression *r;
};

class mod_expr: public integer_expression {
 public:
  mod_expr(integer_expression *left, integer_expression *right) {
    l = left;
    r = right;
  }

  virtual int evaluate_expression(map<string, int> &sym_tab) {
    return l->evaluate_expression(sym_tab) % r->evaluate_expression(sym_tab);
  }
  
  private:
    integer_expression *l;
    integer_expression *r;
};


class less_expr: public boolean_expression {
 public:
  less_expr(integer_expression *left, integer_expression *right) {
    l=left; r=right;
  }
  virtual bool evaluate_expression(map<string, int> &sym_tab) {
    return l->evaluate_expression(sym_tab) < r->evaluate_expression(sym_tab);
  }
 private:
  integer_expression *l;
  integer_expression *r;
};
class greater_expr: public boolean_expression {
 public:
  greater_expr(integer_expression *left, integer_expression *right) {
    l=left; r=right;
  }
  virtual bool evaluate_expression(map<string, int> &sym_tab) {
    return l->evaluate_expression(sym_tab) > r->evaluate_expression(sym_tab);
  }
 private:
  integer_expression *l;
  integer_expression *r;
};
class ge_expr: public boolean_expression {
 public:
  ge_expr(integer_expression *left, integer_expression *right) {
    l=left; r=right;
  }
  virtual bool evaluate_expression(map<string, int> &sym_tab) {
    return l->evaluate_expression(sym_tab) >= r->evaluate_expression(sym_tab);
  }
 private:
  integer_expression *l;
  integer_expression *r;
};
class le_expr: public boolean_expression {
 public:
  le_expr(integer_expression *left, integer_expression *right) {
    l=left; r=right;
  }
  virtual bool evaluate_expression(map<string, int> &sym_tab) {
    return l->evaluate_expression(sym_tab) <= r->evaluate_expression(sym_tab);
  }
 private:
  integer_expression *l;
  integer_expression *r;
};
class ee_expr: public boolean_expression {
 public:
  ee_expr(integer_expression *left, integer_expression *right) {
    l=left; r=right;
  }
  virtual bool evaluate_expression(map<string, int> &sym_tab) {
    return l->evaluate_expression(sym_tab) == r->evaluate_expression(sym_tab);
  }
 private:
  integer_expression *l;
  integer_expression *r;
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
  

class while_statement: public statement {
 public:
  while_statement(boolean_expression *cond, compound_statement *body) {
    c=cond;
    b=body;
  }

  virtual void evaluate_statement(map<string, int> &sym_tab) {
    while (c->evaluate_expression(sym_tab)) {
      b->evaluate_statement(sym_tab);
    }
  }
    

    
  private:
    boolean_expression *c;
    compound_statement *b;
  };

class assignment_statement: public statement {

 public:
  assignment_statement(char *id, integer_expression *rhs) {
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
    integer_expression *r_side;
  };

class print_statement: public statement {
 public:
  print_statement(integer_expression *expr) {
    e=expr;
  }
  virtual void evaluate_statement(map<string, int> &sym_tab) {
    cout << e->evaluate_expression(sym_tab) << endl;
  }
    

  private:
    integer_expression *e;

};
