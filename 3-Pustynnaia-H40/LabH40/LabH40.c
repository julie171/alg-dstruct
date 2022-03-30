#include <stdio.h>
#include <stdlib.h>
#include "LabH40.h"

node_t* RotateLeft(node_t* chosen_node) {
    if (chosen_node == NULL)
        return NULL;
    node_t* buffer = chosen_node->right;
    chosen_node->right = buffer->left;
    buffer->left = chosen_node;
    return buffer;
}

node_t* RotateRight(node_t* node) {
    if (node == NULL)
        return NULL;
    node_t* buffer = node->left;
    node->left = buffer->right;
    buffer->right = node;
    return buffer;
}

node_t* FindAndSplay(node_t* root, int key) {
    if (root == NULL || root->key == key)
        return root;
    if (root->key > key) {
        if (root->left == NULL)
            return root;
        if (root->left->key > key) {
            root->left->left = FindAndSplay(root->left->left, key);
            root = RotateRight(root);
        }
        else if (root->left->key < key) {
            root->left->right = FindAndSplay(root->left->right, key);
            if (root->left->right != NULL)
                root->left = RotateLeft(root->left);
        }
        return (root->left != NULL) ? RotateRight(root) : root;
    }
    if (root->right == NULL)
        return root;
    if (root->right->key > key) {
        root->right->left = FindAndSplay(root->right->left, key);
        if (root->right->left != NULL)
            root->right = RotateRight(root->right);
    }
    else if (root->right->key < key) {
        root->right->right = FindAndSplay(root->right->right, key);
        root = RotateLeft(root);
    }
    return (root->right != NULL) ? RotateLeft(root) : root;
}

node_t* SplayTreeInit(int key) {
    node_t* newNode = (node_t*)malloc(sizeof(node_t));
    if (newNode == NULL)
        return NULL;
    newNode->key = key;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

void FreeTree(node_t* root) {
    if (root != NULL) {
        FreeTree(root->left);
        FreeTree(root->right);
        root->left = NULL;
        root->right = NULL;
        free(root);
    }
}

node_t* AddNode(node_t* root, int key) {
    node_t* addedNode = NULL;
    if (root == NULL)
        return SplayTreeInit(key);
    root = FindAndSplay(root, key);
    if (root->key == key)
        return root;
    addedNode = SplayTreeInit(key);
    if (addedNode == NULL) {
        FreeTree(root);
        return NULL;
    }
    if (root->key > key) {
        addedNode->right = root;
        addedNode->left = root->left;
        root->left = NULL;
    }
    else {
        addedNode->left = root;
        addedNode->right = root->right;
        root->right = NULL;
    }
    return addedNode;
}

node_t* DeleteNode(node_t* root, int key) {
    if (root == NULL)
        return NULL;
    node_t* cur = NULL;
    root = FindAndSplay(root, key);
    if (root->key != key)
        return root;
    cur = root;
    if (root->left == NULL)
        root = root->right;
    else {
        root = FindAndSplay(root->left, key);
        root->right = cur->right;
    }
    free(cur);
    return root;
}

int Interface() {
    node_t* root = NULL;
    char action = 0;
    int key = 0;
    gets(&action);
    while (action != '\n') {
        scanf_s(" %d", & key);
        switch (action) {
        case 'a':
            root = AddNode(root, key);
            if (root == NULL)
                return 0;
            break;
        case 'r':
            root = DeleteNode(root, key);
            break;
        case 'f':
            root = FindAndSplay(root, key);
            if (root != NULL) {
                if (root->key == key)
                    printf("yes\n");
                else
                    printf("no\n");
            }
            else
                printf("no\n");
            break;
        }
        gets(&action);
    }
    FreeTree(root);
    return 1;
}

int main() {
    if (Interface() == 0)
        return -1;
    return 0;
}