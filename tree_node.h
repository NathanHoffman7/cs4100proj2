/**
 * @file tree_node.h
 * @authors Nathan Hoffman (nh662219@ohio.edu)
 *          Kara Nemet     (kn485120@ohio.edu)
 * @brief  This file contains the definitions and functions for the tree node.
 * @version 0.1
 * @date 04-18-2024
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef TREE_NODE_H
#define TREE_NODE_H

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

typedef struct TreeNode {
    string name;
    int weight;
    string parent;
    vector<TreeNode*> children;
} TreeNode;

TreeNode* create_node(string name, int weight, string parent) {
    TreeNode* node = new TreeNode;
    node->name = name;
    node->weight = weight;
    node->parent = parent;
    return node;
}

void add_child(TreeNode* parent, TreeNode* child) {
    parent->children.push_back(child);
}

map<string, TreeNode*> create_map() {
    map<string, TreeNode*> new_map;
    return new_map;
}

void insert_map(map<string, TreeNode*>& node_map, string key, TreeNode* tree_node) {
    node_map[key] = tree_node;
}

TreeNode* find_map(map<string, TreeNode*>& node_map, string key) {
    if (node_map.find(key) != node_map.end()) {
        return node_map[key];
    }
    return NULL;
}

void print_map(TreeNode* node) {
    if (node == NULL) {
        return;
    }

    cout << "(<" << node->name << "," << node->weight << ">";
    
    // Then print all children from right to left
    for (int i = node->children.size() - 1; i >= 0; i--) {
        cout << ',';  // Separate children with a comma
        print_map(node->children[i]);
    }
    cout << ")";
}

#endif // TREE_NODE_H