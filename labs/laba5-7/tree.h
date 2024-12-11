#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>

typedef struct TNode {
    int data;
    struct TNode *firstChild;
    struct TNode *nextBrother;
    bool exist;
} TNode;

TNode *createNode(int data) {
    TNode *newNode = (TNode *)malloc(sizeof(TNode));
    if (newNode != NULL) {
        newNode->data = data;
        newNode->firstChild = NULL;
        newNode->nextBrother = NULL;
        newNode->exist = true;
    }
    return newNode;
}

void addChild(TNode *parent, int data) { // добавление дочернего узла
    TNode *newChild = createNode(data);
    if (newChild == NULL) {
        fprintf(stderr, "Failed to create a new child node.\n");
        return;
    }
    if (parent->firstChild == NULL) {
        parent->firstChild = newChild;
    } else {
        TNode *brother = parent->firstChild;
        while (brother->nextBrother != NULL) {
            brother = brother->nextBrother;
        }
        brother->nextBrother = newChild;
    }
}


TNode* find_node(TNode *root, int data) { // обход дерева
    if (root == NULL) {
        return NULL;
    }

    if (root->data == data && root->exist == true) {
        return root;
    }

    TNode *found_in_child = find_node(root->firstChild, data);
    if (found_in_child != NULL) {
        return found_in_child;
    }

    return find_node(root->nextBrother, data);
}

void Heartbit(TNode* root, int time) {
    if (root != NULL) {
        if (root->exist) {
            sleep(time);
            printf("Heartbit: node %d is fine\n", root->data);
        }
        else {
            sleep(4*time);
            printf("Heartbit: node %d is unavailable now\n", root->data);
        }
        Heartbit(root->firstChild, time);
        Heartbit(root->nextBrother, time);
    }
}

void disableNode(TNode *node) { // рекурсивное убийство дочерних узлов
    if (node == NULL) {
        return;
    }
    node->exist = false;
    disableNode(node->firstChild);
    disableNode(node->nextBrother);
}

void disableOneNode(TNode *node) { // убийство родительского узла
    node->exist = false;
}
