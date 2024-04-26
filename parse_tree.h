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

#include <string>
#include <set>
#include <algorithm>
#include <iostream>
#include "tree_node.h"

using namespace std;

class int_expr {
 public:
  virtual int evaluate_expression(Map* sym_tab) =0;
};

class str_expr {
 public:
  virtual char* evaluate_expression(Map* sym_tab)=0;
};

class int_constant:public int_expr {
 public:
  int_constant(int val) {saved_val = val;}

  virtual int evaluate_expression(Map* sym_tab) {
    return saved_val;
  }
 private:
  int saved_val;
};

class variable: public int_expr {
 public:
  variable(char *in_val) {saved_val =in_val;}

  virtual int evaluate_expression(Map* sym_tab) {
    TreeNode* p = find_map(sym_tab, saved_val);
    if (p != NULL) {
      return atoi(p->weight);
    } else {
      return -1;
    }
  }
 private:
  char* saved_val;
};

class add_expr: public int_expr {
 public:
  add_expr(int_expr *left, int_expr *right) {
    l = left;
    r = right;
  }

  virtual int evaluate_expression(Map* sym_tab) {
    return l->evaluate_expression(sym_tab) + r->evaluate_expression(sym_tab);
  }
  
  private:
    int_expr *l;
    int_expr *r;
};

class statement {
 public:
  virtual void evaluate_statement(Map* sym_tab) =0;
};

class compound_statement: public statement {
 public:
  compound_statement(statement *first, compound_statement *rest) {
    f = first;
    r = rest;
  }
  
  virtual void evaluate_statement(Map* sym_tab) {
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

class buildnode_statement: public statement {
 public:
  buildnode_statement(char *id, char *weight, char* parent) {
    ident = id;
    node_weight = weight;
    parent_id = parent;
  }
  virtual void evaluate_statement(Map* sym_tab) {
    TreeNode* parent_node = find_map(sym_tab, parent_id);
    if (parent_node != NULL) {
      TreeNode* new_node = create_node(ident, node_weight, parent_id);
    } else {
        std::cerr << "Error: Parent node " << parent_id << " not found." << std::endl;
        exit(EXIT_FAILURE);
    }
  }

  private: 
    char* ident;
    char *node_weight;
    char* parent_id;
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

