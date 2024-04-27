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

class int_const:public int_expr {
 public:
  int_const(int val) {saved_val = val;}

  virtual int evaluate_expression(Map* sym_tab, vector<int>& my_int_list) {
    return saved_val;
  }
 private:
  int saved_val;
};

class int_var: public int_expr {
 public:
  int_var(int in_val) {saved_val = in_val;}

virtual int evaluate_expression(Map* sym_tab, vector<int>& my_int_list) {
  char str_val[12];  // Buffer to hold the string representation of the integer
  sprintf(str_val, "%d", saved_val);  // Convert the integer to a string
  TreeNode* p = find_map(sym_tab, str_val);  // Now pass the string to find_map
  if (p != NULL) {
    return p->weight;
  } else {
    return -1;
  }
}
 private:
  int saved_val;
};

class int_add_expr: public int_expr {
 public:
  int_add_expr(int_expr *left, int_expr *right) {
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

class str_expr {
 public:
  virtual char* evaluate_expression(Map* sym_tab, vector<string>& my_str_list) =0;
};

class str_const:public str_expr {
 public:
  str_const(char *val) {
    saved_val = new char[strlen(val) + 1];
    strcpy(saved_val, val);
  }

  virtual char* evaluate_expression(Map* sym_tab, vector<string>& my_str_list) {
    return saved_val;
  }
 private:
  char* saved_val;
};

class str_var: public str_expr {
 public:
  str_var(char *in_val) {
    saved_val = new char[strlen(in_val) + 1];
    strcpy(saved_val, in_val);
  }

  virtual char* evaluate_expression(Map* sym_tab, vector<string>& my_str_list) {
    TreeNode* p = find_map(sym_tab, saved_val);
    if (p != NULL) {
      return p->name;
    } else {
      return NULL;
    }
  }
 private:
  char* saved_val;
};

class str_add_expr: public str_expr {
 public:
  str_add_expr(str_expr *left, str_expr *right) {
    l = left;
    r = right;
  }

  virtual char* evaluate_expression(Map* sym_tab, vector<string>& my_str_list) {
    char* left = l->evaluate_expression(sym_tab, my_str_list);
    char* right = r->evaluate_expression(sym_tab, my_str_list);
    char* result = (char*) malloc(strlen(left) + strlen(right) + 1);
    strcpy(result, left);
    strcat(result, right);
    return result;
  }
  
  private:
    str_expr *l;
    str_expr *r;
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
  buildnode_statement(str_expr* id, int_expr* weight, str_expr* parent) {
    ident = id;
    node_weight = weight;
    parent_id = parent;
  }
virtual void evaluate_statement(Map* sym_tab, vector<int>& my_int_list, vector<string>& my_str_list) {
    TreeNode* parent_node = find_map(sym_tab, parent_id->evaluate_expression(sym_tab, my_str_list));
    if (parent_node != NULL) {
        // Check if the identifier exists in the map
        TreeNode* new_node = create_node(ident->evaluate_expression(sym_tab, my_str_list),
         node_weight->evaluate_expression(sym_tab, my_int_list), parent_id->evaluate_expression(sym_tab, my_str_list));
        insert_map(sym_tab, ident->evaluate_expression(sym_tab, my_str_list), new_node);  // Insert the new node into the map
        add_child(parent_node, new_node);  // Add the new node to the parent's children vector
    } else {
        std::cerr << "Error: Parent node " << parent_id << " not found." << std::endl;
        exit(EXIT_FAILURE);
    }
}
  private: 
    str_expr* ident;
    int_expr* node_weight;
    str_expr* parent_id;
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

