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
  virtual int evaluate_expression(Map* sym_tab, vector<int>& my_int_list) =0;
};

class str_expr {
 public:
  virtual char* evaluate_expression(Map* sym_tab, vector<string>& my_str_list) =0;
};

class int_constant:public int_expr {
 public:
  int_constant(int val) {saved_val = val;}

  virtual int evaluate_expression(Map* sym_tab, vector<int>& my_int_list) {
    return saved_val;
  }
 private:
  int saved_val;
};

class variable: public int_expr {
 public:
  variable(char *in_val) {saved_val =in_val;}

  virtual int evaluate_expression(Map* sym_tab, vector<int>& my_int_list) {
    TreeNode* p = find_map(sym_tab, saved_val);
    if (p != NULL) {
      return p->weight;
    } else {
      return -1;
    }
  }
 private:
  char* saved_val;
};

class add_int_expr: public int_expr {
 public:
  add_int_expr(int_expr *left, int_expr *right) {
    l = left;
    r = right;
  }

  virtual int evaluate_expression(Map* sym_tab, vector<int>& my_int_list) {
    return l->evaluate_expression(sym_tab, my_int_list) + r->evaluate_expression(sym_tab, my_int_list);
  }
  
  private:
    int_expr *l;
    int_expr *r;
};

class statement {
 public:
  virtual void evaluate_statement(Map* sym_tab, vector<int>& my_int_list, vector<string>& my_str_list) =0;
};

class compound_statement: public statement {
 public:
  compound_statement(statement *first, compound_statement *rest) {
    f = first;
    r = rest;
  }
  
  virtual void evaluate_statement(Map* sym_tab, vector<int>& my_int_list, vector<string>& my_str_list) {
    if (f!=NULL) {
      f->evaluate_statement(sym_tab, my_int_list, my_str_list);
    }
    if (r!=NULL) {
      r->evaluate_statement(sym_tab, my_int_list, my_str_list);
    }
  }
 private:
  compound_statement *r;
  statement *f;
};

class buildnode_statement: public statement {
 public:
  buildnode_statement(char *id, int weight, char* parent) {
    ident = id;
    node_weight = weight;
    parent_id = parent;
  }
virtual void evaluate_statement(Map* sym_tab, vector<int>& my_int_list, vector<string>& my_str_list) {
    TreeNode* parent_node = find_map(sym_tab, parent_id);
    if (parent_node != NULL) {
        // Check if the identifier exists in the map
        TreeNode* new_node = create_node(ident, node_weight, parent_id);
        add_child(parent_node, new_node);  // Add the new node to the parent's children vector
    } else {
        std::cerr << "Error: Parent node " << parent_id << " not found." << std::endl;
        exit(EXIT_FAILURE);
    }
}
  private: 
    char* ident;
    int node_weight;
    char* parent_id;
};

class for_statement: public statement {
 public:
  for_statement(char *id, int start, int end, compound_statement *body) {
    ident = id;
    s = start;
    e = end;
    b = body;
  }
virtual void evaluate_statement(Map* sym_tab, vector<int>& my_int_list, vector<string>& my_str_list) {
    int start = s;
    int end = e;
    my_int_list.clear(); // Clear the vector
    for (int i = start; i <= end; i++) {
      // my_int_list.push_back(i); // Store all values in the vector
      b->evaluate_statement(sym_tab, my_int_list, my_str_list);
    }
}
 private:
  char* ident;
  int s;
  int e;
  compound_statement *b;
};

