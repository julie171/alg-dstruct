#pragma once

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct node_t {
    int key;
    struct node_t* left;
    struct node_t* right;
} node_t;

node_t* RotateLeft(node_t* chosen_node);

node_t* RotateRight(node_t* node);

node_t* FindAndSplay(node_t* root, int key);

node_t* SplayTreeInit(int key);

void FreeTree(node_t* root);

node_t* AddNode(node_t* root, int key);

node_t* DeleteNode(node_t* root, int key);

int Interface();

#ifdef __cplusplus
}
#endif