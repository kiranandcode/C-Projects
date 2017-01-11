#include <stdio.h>
#include <stdlib.h>
typedef struct node {
    int single;
    union {
        int i;
    } key;
    struct node *left;
    struct node *right;
    struct node *parent;
} node;

node *cpNode(node *original) {
    node *new = malloc(sizeof(node));
    new->key.i = original->key.i;
    new->left = original->left;
    new->right = original->right;
    new->parent = original->parent;
    return new;
}

void inOrderTreeWalk(node * root) {
    if(root->left != NULL) {
        inOrderTreeWalk(root->left);
    }

    fprintf(stdout, "%d, ", root->key.i);

    if(root->right != NULL) {
        inOrderTreeWalk(root->right);
    }
    return;

}

int countNodes(node * root, int count) {
    if(root->left != NULL) {
        count = countNodes(root->left, count+1);
    }

    if(root->right != NULL) {
       count = countNodes(root->right, count+1);
    }
    return count;
}

node *searchTreeR(node* root, int key) {
    if(root == NULL) return NULL;
    if(root->key.i == key) return root;
    if(key <= root->key.i) {
        return searchTreeR(root->left, key);
    }
    else {
        return searchTreeR(root->right, key);
    }
}

node *searchTree(node *root, int key) {
    node * current = root;
    while(current != NULL && current->key.i != key) {
        if (key <= current->key.i) current = current->left;
        else current = current->right;
    }
    return current;
}

node *minTree(node *root) {
    while(root->left != NULL) {
        root = root->left;
    }
    return root;
}


node *maxTree(node *root) {
    while(root->right != NULL){
        root = root->right;
    }
    return root;
}

node *successor(node *start) {
    if(start->right != NULL) {
        return minTree(start->right);
    } else {
        node *parent = start->parent;
        while(parent != NULL && start == parent->right){
            start = start->parent;
            parent = start->parent;
        }
        return parent;
    }
}

void insertTree(node *root, int key) {
    int flag = 0;
    if(root->key.i > key){if(root->left != NULL){insertTree(root->left, key); return;} else flag = 1;}
    if(root->key.i <= key) {if(root->right != NULL){insertTree(root->right, key); return;} else flag = 2;}
    node *nw = malloc(sizeof(node));
    nw->key.i = key;
    nw->left = NULL;
    nw->right = NULL;
    nw->parent = root;
    if(flag == 1) root->left = nw;
    else if (flag == 2) root->right = nw;
    else fprintf(stderr, "ERROR, flag not raised, issue in implementation"), exit(1);
}

void printTree(node *root, int len) {
    int i = 0;
    for(;i<len;i++) printf("\t");
    printf("(Root:");
    printf("%d\n", root->key.i);
    if(root->left != NULL) printf("Left:"),printTree(root->left, len+1);
    if (root->right != NULL) printf("Right:"), printTree(root->right, len+1);
    else printf("\n");
    for(;i<len;i++) printf("\t"); printf(")");
    return;
}

void deleteNode(node *root, int key) {
    node *removalNode = searchTree(root, key);
    int pos = -1;
    if(removalNode->parent == NULL) pos = 0;
    else if(removalNode == removalNode->parent->right) pos = 2;
    else if (removalNode == removalNode->parent->left) pos = 1;

    if(removalNode->left == NULL && removalNode->right == NULL) {
        if(pos == 1) removalNode->parent->left = NULL;
        else if(2 == pos) removalNode->parent->right = NULL;
        else if(0 == pos) {removalNode->single = 1; return;}
        else printf("Error in tree structure\n"), exit(1);
        pos = -1;
    }
    if(removalNode->left == NULL && pos != -1) {

            if(pos == 1)
                removalNode->parent->left = removalNode->right;
            if(pos == 2)
                removalNode->parent->right = removalNode->right;
            if(pos == 0) {
                removalNode->key.i = removalNode->right->key.i;
                removalNode->right->left->parent = removalNode;
                removalNode->right->right->parent = removalNode;
                removalNode->left = removalNode->right->left;
                removalNode->parent = removalNode->right->right;
                free(removalNode->right);
                removalNode->right = removalNode->parent;
                removalNode->parent = NULL;
                return;
            }
            pos = -1;
    }
    if(removalNode->right == NULL && pos != -1) {
            if(pos == 1)
                removalNode->parent->left = removalNode->left;
            if(pos == 2)
                removalNode->parent->right = removalNode->left;
            if(pos == 0) {
                removalNode->key.i = removalNode->left->key.i;
                removalNode->left->left->parent = removalNode;
                removalNode->left->right->parent = removalNode;
                removalNode->right = removalNode->left->right;
                removalNode->parent = removalNode->left->left;
                free(removalNode->left);
                removalNode->left = removalNode->parent;
                removalNode->parent = NULL;
                return;
            }
            pos = -1;
    }
    if(pos != -1) {
        if(pos != 0) {
            node *child = cpNode(successor(removalNode));
            deleteNode(root, child->key.i);
            if(pos == 1)
                removalNode->parent->left = child;
            if(pos == 2)
                removalNode->parent->right = child;
            if(removalNode->left != NULL) removalNode->left->parent = child;
            if(removalNode->right != NULL) removalNode->right->parent = child;
            child->left = removalNode->left;
            child->right = removalNode->right;
            child->parent = removalNode->parent;
        }
        else {
            node *child = cpNode(successor(removalNode));
            deleteNode(root, child->key.i);
            removalNode->key.i = child->key.i;
            free(child);
            return;
        }
        pos = -1;
    }
    free(removalNode);
}


int main()
{
    node *root = malloc(sizeof(node));
    root->left = root->right = root->parent = NULL;
    int cases;
    printf("Enter the number of elements to be stored:\n");
    fscanf(stdin, "%d", &cases);
    printf("Please enter %d elements below:\n", cases);
    int i, val;
    for(i = 0; i<cases; i++) {
        fscanf(stdin, "%d", &val);
        if(i == 0) {
            root->key.i = val;
        }
        else {
            insertTree(root, val);
        }
    }
    printTree(root, 0);
    printf("The minimum is: %d, %d\n", minTree(root)->key.i, maxTree(root)->key.i);
    inOrderTreeWalk(root);
    printf("\n Enter a key to get it's successor:\n");
    fscanf(stdin, "%d", &val);
    printf("Key is %d\n", searchTree(root, val)->key.i);
    printf("Successor of %d is %d", val, successor(searchTree(root, val))->key.i);
    printf("\n Enter a key to remove it:\n");
    fscanf(stdin, "%d", &val);
    deleteNode(root, val);
    printTree(root, 0);
    inOrderTreeWalk(root);
    printf("\n Enter a key to remove it:\n");
    fscanf(stdin, "%d", &val);
    deleteNode(root, val);
    inOrderTreeWalk(root);
    return 0;
}
