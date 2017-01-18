#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct _kval {
    int value;
    void *data;
} key;

//leaf prop, children, parent, keys, size, max_size
typedef struct _node {
    int leaf;
    struct _kval *keys;
    struct _node **children;
    struct _node *parent;
    int size;
    int max_size;
} node, *nodePtr;

//returns an empty node of a set max_size
nodePtr nodeConstructor(int max_size) {
    nodePtr newNode = malloc(sizeof(node));
    newNode->leaf = 1;
    newNode->keys = 0;
    newNode->children = 0;
    newNode->parent = 0;
    newNode->size = 0;
    newNode->max_size = max_size;
    return newNode;
}

//inserts into a leaf and returns a val if splitting is needed
int insertLeafNode(nodePtr node, key val) {
    if(!node->leaf) fprintf(stderr, "Attempted calling insertLeafNode on non-leaf node\n"), exit(1);
    node->keys = realloc(node->keys, sizeof(key)*(node->size+1));
    node->keys[node->size++] = val;
    int i,j;
    for(i = 1; i<node->size; i++) {
        key temp = node->keys[i];
        for(j = i-1; j>0; j--) {
            if(node->keys[j].value>temp.value) break;
            node->keys[j+1] = node->keys[j];
        }
        node->keys[j+1] = temp;
    }
    if(node->size >= node->max_size) return 1;
    else return 0;
}

void printLeafNode(nodePtr node) {
    if(!node->leaf) fprintf(stderr, "Attempted calling insertLeafNode on non-leaf node\n"), exit(1);
    int i;
    fprintf(stdout, "[");
    for(i=0; i<node->size; i++) {
        fprintf(stdout, "(%d: %s),", node->keys[i].value, (char *)node->keys[i].data);
    }
    fprintf(stdout, "\b]\n");
}


key encodeData(int val, char *string) {
    key newKey;
    newKey.value = val;
    newKey.data = string;
    return newKey;
}


int main() {
    nodePtr root = nodeConstructor(10);
    int i;
    for(i = 0; i<5; i++) {
        key newKey = encodeData((i+7)%5, "Hello");
        insertLeafNode(root, newKey);
        printLeafNode(root);
    }



}
