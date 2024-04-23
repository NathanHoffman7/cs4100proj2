/**
 * @file tree_node.h
 * @authors Nathan Hoffman (nh662219@ohio.edu)
 *          Kara Nemet     (kn485120@ohio.edu)
 * @brief Class definition for a tree node
 * @version 0.1
 * @date 04-18-2024
 * 
 * @copyright Copyright (c) 2024
 * 
 */

// tree_node.h
#ifndef TREE_NODE_H
#define TREE_NODE_H

#include <stdlib.h>
#include <string.h>

typedef struct TreeNode {
    char* name;
    int weight;
    struct TreeNode* parent;
} TreeNode;

TreeNode* create_node(char* name, int weight) {
    TreeNode* node = (TreeNode*) malloc(sizeof(TreeNode));
    node->name = strdup(name);
    node->weight = weight;
    node->parent = NULL;

    return node;
}


#endif // TREE_NODE_H