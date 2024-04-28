/**
 * @file parse_tree.h
 * @authors Nathan Hoffman (nh662219@ohio.edu)
 *          Kara Nemet     (kn485120@ohio.edu)
 * @brief   This file contains the definitions for the classes 
 *         that will be used to parse the input file and build
 *        the tree structure.
 * @version 0.1
 * @date 04-18-2024
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include <iostream>
#include "tree_node.h"

using namespace std;

typedef map<string, TreeNode*> nodeMap;
typedef map<string, vector<int>> intMap;
typedef map<string, vector<string>> strMap;
int idx = 0;
vector<string> str_vec;


class int_expr {
 public:
  virtual int evaluate_expression(nodeMap& node_tbl, intMap& int_tbl, strMap& str_tbl) {
    // cout << "Evaluating int_expr" << endl;
    return 0;
  }
};

class str_expr {
 public:
  virtual string evaluate_expression(nodeMap& node_tbl, intMap& int_tbl, strMap& str_tbl) {
    // cout << "Evaluating str_expr" << endl;
    return "";
  }
};

class int_constant:public int_expr {
 public:
  int_constant(int val) {saved_val = val;}

  virtual int evaluate_expression(nodeMap& node_tbl, intMap& int_tbl, strMap& str_tbl) {
    // cout << "Evaluating int_constant" << endl;
    return saved_val;
  }
 private:
  int saved_val;
};

class str_constant:public str_expr {
 public:
  str_constant(string val) 
  {
    saved_val = val;
    saved_val.erase(remove(saved_val.begin(), saved_val.end(), '\"'), saved_val.end());
  }

  virtual string evaluate_expression(nodeMap& node_tbl, intMap& int_tbl, strMap& str_tbl) {
    // cout << "Evaluating str_constant" << endl;
    return saved_val;
  }
 private:
  string saved_val;
};

class int_var: public int_expr {
 public:
  int_var(string in_val) {saved_val =in_val;}

  virtual int evaluate_expression(nodeMap& node_tbl, intMap& int_tbl, strMap& str_tbl) {
    // cout << "Evaluating int_var" << endl;
    // cout << int_tbl[saved_val].at(idx) << endl;
    return int_tbl[saved_val].at(idx);
  }
  string get_name() {return saved_val;}
 private:
  string saved_val;
};

class str_var: public str_expr {
 public:
  str_var(string in_val) {saved_val =in_val;}

  virtual string evaluate_expression(nodeMap& node_tbl, intMap& int_tbl, strMap& str_tbl) {
    // cout << "Evaluating str_var" << endl;
    // cout << saved_val << endl;
    return str_tbl[saved_val].at(idx);
  }
  string get_name() {return saved_val;}
 private:
  string saved_val;
};

class add_int_expr: public int_expr {
 public:
  add_int_expr(int_expr *left, int_expr *right) {
    l = left;
    r = right;
  }

  virtual int evaluate_expression(nodeMap& node_tbl, intMap& int_tbl, strMap& str_tbl) {
    // cout << "Evaluating add_int_expr" << endl;
    return l->evaluate_expression(node_tbl, int_tbl, str_tbl) + r->evaluate_expression(node_tbl, int_tbl, str_tbl);
  }
  
  private:
    int_expr *l;
    int_expr *r;
};

class add_str_expr: public str_expr {
 public:
  add_str_expr(str_expr *left, int_expr *right) {
    l = left;
    r = right;
  }

  virtual string evaluate_expression(nodeMap& node_tbl, intMap& int_tbl, strMap& str_tbl) {
    // cout << "Evaluating add_str_expr" << endl;
    string left = l->evaluate_expression(node_tbl, int_tbl, str_tbl);
    string right = to_string(r->evaluate_expression(node_tbl, int_tbl, str_tbl));
    // cout << left << " " << right << endl;
    return left + right;
  }
  
  private:
    str_expr *l;
    int_expr *r;
};

class str_list {
 public:
  str_list(string first, str_list* rest) {
    f = first;
    r = rest;
  }

  virtual void evaluate_expression(nodeMap& node_tbl, intMap& int_tbl, strMap& str_tbl) {
    // cout << "Evaluating str_list" << endl;

    str_vec.insert(str_vec.begin(), f);
    if (r!=NULL)
    {
      r->evaluate_expression(node_tbl, int_tbl, str_tbl);
    }
  
  }
  
  private:
    string f;
    str_list *r;
};

class statement {
 public:
  virtual void evaluate_statement(nodeMap& node_tbl, intMap& int_tbl, strMap& str_tbl)=0;
};

class compound_statement: public statement {
 public:
  compound_statement(statement *first, compound_statement *rest) {
    f = first;
    r = rest;
  }
  
  virtual void evaluate_statement(nodeMap& node_tbl, intMap& int_tbl, strMap& str_tbl) {
    // cout << "Evaluating compound_statement" << endl;
    if (f!=NULL) {
      f->evaluate_statement(node_tbl, int_tbl, str_tbl);
    }
    if (r!=NULL) {
      r->evaluate_statement(node_tbl, int_tbl, str_tbl);
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
virtual void evaluate_statement(nodeMap& node_tbl, intMap& int_tbl, strMap& str_tbl) {
    // cout << "Evaluating buildnode_statement" << endl;
    string id = ident->evaluate_expression(node_tbl, int_tbl, str_tbl);
    int weight = node_weight->evaluate_expression(node_tbl, int_tbl, str_tbl);
    string parent;
    if (parent_id != NULL)
    {
        parent = parent_id->evaluate_expression(node_tbl, int_tbl, str_tbl);

        TreeNode* parent_node = find_map(node_tbl, parent);
        TreeNode* new_node = create_node(id, weight, parent);
        insert_map(node_tbl, id, new_node);
        add_child(parent_node, new_node);  // Add the new node to the parent's children vector
    }
    else
    {
        parent = "NULL";
        TreeNode* new_node = create_node(id, weight, parent);
        insert_map(node_tbl, id, new_node);
    }
    
    // cout << "Node: " << id << " " << weight << " " << parent << endl;
}
  private: 
    str_expr* ident;
    int_expr* node_weight;
    str_expr* parent_id;
};

class for_statement: public statement {
 public:
  for_statement(int_var* id, int start, int end, compound_statement *body) {
    ident = id;
    s = start;
    e = end;
    b = body;
  }
virtual void evaluate_statement(nodeMap& node_tbl, intMap& int_tbl, strMap& str_tbl) {
    // cout << "Evaluating for_statement" << endl;
    string int_id = ident->get_name();
    // cout << "For loop: " << int_id << " " << s << " " << e << endl;
    int start = s;
    int end = e;
    int_tbl.clear(); // Clear the vector
    for (idx = start - 1; idx < end; idx++) {
      int_tbl[int_id].push_back(idx + 1);
      b->evaluate_statement(node_tbl, int_tbl, str_tbl);
    }
    int_tbl[int_id].clear(); // Clear the vector
}
 private:
  int_var* ident;
  int s;
  int e;
  compound_statement *b;
};

class str_for_statement: public statement {
 public:
  str_for_statement(str_var* id, str_list* lst, compound_statement *body) {
    ident = id;
    l = lst;
    b = body;
  }
virtual void evaluate_statement(nodeMap& node_tbl, intMap& int_tbl, strMap& str_tbl) {
    // cout << "Evaluating str_for_statement" << endl;
    string str_id = ident->get_name();
    // cout << "For loop: " << str_id << " " << endl;
    l->evaluate_expression(node_tbl, int_tbl, str_tbl);
    str_tbl.clear(); // Clear the vector
    for (idx = 0; idx < str_vec.size(); idx++) {
      str_tbl[str_id].push_back(str_vec.at(idx));
      b->evaluate_statement(node_tbl, int_tbl, str_tbl);
    }
    str_tbl[str_id].clear(); // Clear the vector
    str_vec.clear();
}
  private:
    str_var* ident;
    str_list* l;
    compound_statement *b;
  };
