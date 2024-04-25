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
    char* parent;
} TreeNode;

typedef struct Node {
    char* key;
    TreeNode* value;
    struct Node* next;
} Node;

typedef struct Map {
    Node* head;
} Map;

TreeNode* create_node(char* name, int weight) {
    TreeNode* node = (TreeNode*) malloc(sizeof(TreeNode));
    node->name = strdup(name);
    node->weight = weight;
    node->parent = NULL;

    return node;
}

TreeNode* create_node_with_parent(char* name, int weight, char* parent) {
    TreeNode* node = create_node(name, weight);
    node->parent = parent;

    return node;
}

Map* create_map() {
    Map* map = (Map*) malloc(sizeof(Map));
    map->head = NULL;

    return map;
}

void insert_map(Map* map, char* key, TreeNode* value) {
    Node* node = (Node*) malloc(sizeof(Node));
    node->key = strdup(key);
    node->value = value;
    node->next = map->head;
    map->head = node;
}

TreeNode* find_map(Map* map, char* key) {
    Node* node = map->head;
    while (node != NULL) {
        if (strcmp(node->key, key) == 0) {
            return node->value;
        }
        node = node->next;
    }
    return NULL;
}

#endif // TREE_NODE_H