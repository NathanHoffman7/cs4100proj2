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
#include <vector>

typedef struct TreeNode {
    char* name;
    int weight;
    char* parent;
    std::vector<TreeNode*> children;
} TreeNode;

typedef struct Node {
    char* key;
    TreeNode* tree_node;
    struct Node* next;
} Node;

typedef struct Map {
    Node* head;
} Map;

TreeNode* create_node(char* name, int weight, char* parent) {
    TreeNode* node = (TreeNode*) malloc(sizeof(TreeNode));
    node->name = strdup(name);
    node->weight = weight;
    node->parent = parent ? strdup(parent) : NULL;
    node->children = std::vector<TreeNode*>();  // Initialize the children vector

    return node;
}

void add_child(TreeNode* parent, TreeNode* child) {
    parent->children.push_back(child);
}

Map* create_map() {
    Map* map = (Map*) malloc(sizeof(Map));
    map->head = NULL;

    return map;
}

void insert_map(Map* map, char* key, TreeNode* tree_node) {
    Node* node = (Node*) malloc(sizeof(Node));
    if (node == NULL) {
        fprintf(stderr, "Failed to allocate memory for Node.\n");
        exit(1);
    }
    node->key = strdup(key);
    node->tree_node = tree_node;
    node->next = map->head;
    map->head = node;
}

TreeNode* find_map(Map* map, char* key) {
    Node* node = map->head;
    while (node != NULL) {
        if (strcmp(node->key, key) == 0) {
            return node->tree_node;
        }
        node = node->next;
    }
    return NULL;
}

void print_map(TreeNode* node) {
    if (node == NULL) {
        return;
    }

    std::cout << "(<" << node->name << "," << node->weight << ">";
    
    // Then print all children from right to left
    for (int i = node->children.size() - 1; i >= 0; i--) {
        std::cout << ',';  // Separate children with a comma
        print_map(node->children[i]);
    }
    std::cout << ")";
}

#endif // TREE_NODE_H