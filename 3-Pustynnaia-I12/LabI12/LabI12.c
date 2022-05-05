#include <stdio.h>
#include <stdlib.h>

#define T 3
#define SIZE 16

typedef struct node_t {
    int numKeys;
    struct node_t** children;
    int* keys;
    int isLeaf;
} node_t;

typedef struct bTree_t {
    int t;
    node_t* root;
} bTree_t;

int FindKeyInArr(const int* arr, const int size, const int key) {
    for (int i = 0; i < size; ++i)
        if (arr[i] == key)
            return i;
    return -1;
}

void FreeNode(node_t* node) {
    free(node->children);
    free(node->keys);
    free(node);
}

void FreeAllNodes(node_t* node) {
    if (node->isLeaf == 0)
        for (int i = 0; i < node->numKeys + 1; ++i)
            FreeAllNodes(node->children[i]);
    FreeNode(node);
}

void FreeTree(bTree_t* tree) {
    FreeAllNodes(tree->root);
    tree->root = NULL;
}

node_t* FindKey(const bTree_t* tree, const int key, int* id) {
    node_t* foundNode = tree->root;
    while (foundNode->isLeaf == 0) {
        *id = FindKeyInArr(foundNode->keys, foundNode->numKeys, key);
        if (*id != -1)
            return foundNode;
        if (key >= foundNode->keys[foundNode->numKeys - 1]) {
            foundNode = foundNode->children[foundNode->numKeys];
            continue;
        }
        for (int i = 0; i < foundNode->numKeys; ++i)
            if (key < foundNode->keys[i]) {
                foundNode = foundNode->children[i];
                break;
            }
    }
    *id = FindKeyInArr(foundNode->keys, foundNode->numKeys, key);
    return *id != -1 ? foundNode : NULL;
}

int FindNode(const bTree_t* tree, const int key) {
    int idx = -1;
    node_t* found = FindKey(tree, key, &idx);
    if (found == NULL)
        return 0;
    return 1;
}

node_t* CreateNode(const int t) {
    node_t* root = (node_t*)malloc(sizeof(node_t));
    if (root == NULL)
        return NULL;
    root->children = (node_t**)malloc(2 * t * sizeof(node_t*));
    if (root->children == NULL) {
        free(root);
        return NULL;
    }
    for (int i = 0; i < 2 * t; ++i)
        root->children[i] = NULL;
    root->keys = (int*)malloc((2 * t - 1) * sizeof(int));
    if (root->keys == NULL) {
        free(root->children);
        free(root);
        return NULL;
    }
    root->numKeys = 0;
    root->isLeaf = 0;
    return root;
}

int TreeInitialization(bTree_t* tree) {
    node_t* root = CreateNode(tree->t);
    if (root == NULL)
        return 0;
    tree->root = root;
    root->isLeaf = 1;
    return 1;
}

int SplitNode(node_t* node, const int t, const int numSplit) {
    node_t* newNode = CreateNode(t);
    if (newNode == NULL)
        return 0;
    node_t* child = node->children[numSplit];
    newNode->isLeaf = child->isLeaf;
    newNode->numKeys = t - 1;
    for (int i = 0; i < t - 1; ++i)
        newNode->keys[i] = child->keys[i + t];
    if (child->isLeaf == 0)
        for (int i = 0; i < t; ++i)
            newNode->children[i] = child->children[i + t];
    child->numKeys = t - 1;
    for (int i = node->numKeys + 1; i > numSplit + 1; --i)
        node->children[i] = node->children[i - 1];
    node->children[numSplit + 1] = newNode;
    for (int i = node->numKeys; i > numSplit; --i)
        node->keys[i] = node->keys[i - 1];
    node->keys[numSplit] = child->keys[t - 1];
    ++node->numKeys;
    node->isLeaf = 0;
    return 1;
}

int AddInNonfull(node_t* node, const int key, const int t) {
    int i = node->numKeys - 1;
    if (node->isLeaf != 0) {
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            --i;
        }
        node->keys[i + 1] = key;
        ++node->numKeys;
        return 1;
    }
    while (i >= 0 && key < node->keys[i])
        --i;
    ++i;
    if (node->children[i]->numKeys == 2 * t - 1) {
        if (SplitNode(node, t, i) == 0)
            return 0;
        if (key > node->keys[i])
            ++i;
    }
    return AddInNonfull(node->children[i], key, t);
}

int AddNode(bTree_t* tree, const int key) {
    if (FindNode(tree, key) != 0)
        return 0;
    node_t* oldRoot = tree->root;
    if (oldRoot->numKeys == 2 * (tree->t) - 1) {
        node_t* newRoot = CreateNode(tree->t);
        if (newRoot == NULL)
            return 0;
        tree->root = newRoot;
        newRoot->isLeaf = 0;
        newRoot->numKeys = 0;
        newRoot->children[0] = oldRoot;
        if (SplitNode(newRoot, tree->t, 0) == 0) {
            FreeNode(newRoot);
            tree->root = oldRoot;
            return 0;
        }
        return AddInNonfull(newRoot, key, tree->t);
    }
    return AddInNonfull(oldRoot, key, tree->t);
}

void RemoveFromArr(int* arr, const int size, const int id) {
    for (int i = id; i < size - 1; ++i)
        arr[i] = arr[i + 1];
}

void RemoveFromArrNP(node_t** arr, const int size, const int idx) {
    for (int i = idx; i < size - 1; ++i)
        arr[i] = arr[i + 1];
}

void MergeChild(node_t* node, const int keyId) {
    node_t* lChild = node->children[keyId];
    node_t* rChild = node->children[keyId + 1];
    lChild->keys[lChild->numKeys] = node->keys[keyId];
    for (int i = 0; i < rChild->numKeys; ++i)
        lChild->keys[lChild->numKeys + 1 + i] = rChild->keys[i];
    for (int i = 0; i < rChild->numKeys + 1; ++i)
        lChild->children[lChild->numKeys + 1 + i] = rChild->children[i];
    lChild->numKeys += 1 + rChild->numKeys;
    RemoveFromArr(node->keys, node->numKeys, keyId);
    RemoveFromArrNP(node->children, node->numKeys + 1, keyId + 1);
    --node->numKeys;
    FreeNode(rChild);
}

void LeftRotate(node_t* node, const int keyId) {
    node_t* lNeighbour = node->children[keyId];
    node_t* rNeighbour = node->children[keyId + 1];
    ++lNeighbour->numKeys;
    lNeighbour->keys[lNeighbour->numKeys - 1] = node->keys[keyId];
    lNeighbour->children[lNeighbour->numKeys] = rNeighbour->children[0];
    node->keys[keyId] = rNeighbour->keys[0];
    for (int i = 0; i < rNeighbour->numKeys - 1; ++i)
        rNeighbour->keys[i] = rNeighbour->keys[i + 1];
    for (int i = 0; i < rNeighbour->numKeys; ++i)
        rNeighbour->children[i] = rNeighbour->children[i + 1];
    --rNeighbour->numKeys;
}

void RightRotate(node_t* node, const int keyId) {
    node_t* lNeighbour = node->children[keyId];
    node_t* rNeighbour = node->children[keyId + 1];
    ++rNeighbour->numKeys;
    for (int i = rNeighbour->numKeys - 1; i > 0; --i)
        rNeighbour->keys[i] = rNeighbour->keys[i - 1];
    rNeighbour->keys[0] = node->keys[keyId];
    for (int i = rNeighbour->numKeys; i > 0; --i)
        rNeighbour->children[i] = rNeighbour->children[i - 1];
    rNeighbour->children[0] = lNeighbour->children[lNeighbour->numKeys];
    node->keys[keyId] = lNeighbour->keys[lNeighbour->numKeys - 1];
    --lNeighbour->numKeys;
}

void RemoveFromLeaf(node_t* node, const int key, const int t) {
    int i = node->numKeys - 1;
    while (i >= 0 && key < node->keys[i])
        --i;
    ++i;
    node_t* child = node->children[i];
    int id = FindKeyInArr(child->keys, child->numKeys, key);
    if (id != -1) {
        RemoveFromArr(child->keys, child->numKeys, id);
        --child->numKeys;
    }
    else
        RemoveFromLeaf(child, key, t);
    if (child->numKeys < t - 1) {
        if (i < node->numKeys) {
            node_t* rNeighbour = node->children[i + 1];
            if (child->numKeys + rNeighbour->numKeys < 2 * t - 1)
                MergeChild(node, i);
            else
                LeftRotate(node, i);
        }
        else {
            node_t* lNeighbour = node->children[i - 1];
            if (child->numKeys + lNeighbour->numKeys < 2 * t - 1)
                MergeChild(node, i - 1);
            else
                RightRotate(node, i - 1);
        }
    }
}

int GetPredecessor(node_t* node, const int id) {
    node = node->children[id];
    while (node->isLeaf == 0)
        node = node->children[node->numKeys];
    return node->keys[node->numKeys - 1];
}

int GetSuccessor(node_t* node, const int id) {
    node = node->children[id + 1];
    while (node->isLeaf == 0)
        node = node->children[0];
    return node->keys[0];
}

void Remove(node_t* node, const int t, const int key, node_t* nodeWithKey, const int id) {
    if (nodeWithKey == NULL)
        return;
    if (nodeWithKey->isLeaf != 0)
        RemoveFromLeaf(node, key, t);
    else {
        if (nodeWithKey->children[id]->numKeys >= t) {
            int pred = GetPredecessor(nodeWithKey, id);
            RemoveFromLeaf(node, pred, t);
            nodeWithKey->keys[id] = pred;
        }
        else if (nodeWithKey->children[id + 1]->numKeys >= t) {
            int succ = GetSuccessor(nodeWithKey, id);
            RemoveFromLeaf(node, succ, t);
            nodeWithKey->keys[id] = succ;
        }
        else {
            MergeChild(nodeWithKey, id);
            Remove(node, t, key, nodeWithKey->children[id], (2 * t - 1) / 2);
        }
    }
}

void RemoveNode(bTree_t* tree, const int key) {
    if (tree->root->isLeaf != 0) {
        int id = FindKeyInArr(tree->root->keys, tree->root->numKeys, key);
        if (id != -1) {
            RemoveFromArr(tree->root->keys, tree->root->numKeys, id);
            --tree->root->numKeys;
        }
    }
    else {
        int id = -1;
        node_t* neededNode = FindKey(tree, key, &id);
        Remove(tree->root, tree->t, key, neededNode, id);
        if (tree->root->numKeys == 0) {
            node_t* root = tree->root;
            tree->root = tree->root->children[0];
            FreeNode(root);
        }
    }
}

int Interface() {
    bTree_t tree = { T, NULL };
    int check = TreeInitialization(&tree);
    if (check == 0)
        return 0;
    int key = 0;
    node_t* node = NULL;
    int id = -1;
    char buf[SIZE] = { 0 };
    char action = 0;
    while (fgets(buf, SIZE, stdin) != NULL) {
        sscanf(buf, "%c %d", &action, &key);
        switch (action) {
        case 'a':
            AddNode(&tree, key);
            break;
        case 'r':
            RemoveNode(&tree, key);
            break;
        case 'f':
            fprintf(stdout, "%s\n", FindNode(&tree, key) ? "yes" : "no");
            break;
        default:
            FreeTree(&tree);
            return 0;
        }
    }
    FreeTree(&tree);
    return 1;
}

void Print(const node_t* node, const int t, int margin) {
    if (node->isLeaf == 0)
        for (int i = 0; i < (node->numKeys + 1) / 2; i++)
            Print(node->children[i], t, margin + 1);
    for (int i = 0; i < margin * (11 * (2 * t - 1) + 1); i++)
        printf(" ");
    printf("[");
    for (int i = 0; i < node->numKeys - 1; ++i)
        printf("%i,", node->keys[i]);
    printf("%i]\n", node->keys[node->numKeys - 1]);
    if (node->isLeaf == 0)
        for (int i = (node->numKeys + 1) / 2; i < node->numKeys + 1; i++)
            Print(node->children[i], t, margin + 1);
}

int main() {
    if (Interface() == 0)
        return -1;
    return 0;
}