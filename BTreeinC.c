#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#define DEBUG 0
#define ifdb if(DEBUG)

typedef struct _kval
{
    int value;
    void *data;
} key;

//leaf prop, children, parent, keys, size, max_size
typedef struct _node
{
    int leaf;
    struct _kval *keys;
    struct _node **children;
    struct _node *parent;
    int size;
    int max_size;
} node, *nodePtr;

//returns an empty node of a set max_size
nodePtr nodeConstructor(int max_size)
{
    nodePtr newNode = malloc(sizeof(node));
    newNode->leaf = 1;
    newNode->keys = 0;
    newNode->children = 0;
    newNode->parent = 0;
    newNode->size = 0;
    newNode->max_size = (max_size%2) ? max_size : max_size+1;
    return newNode;
}


//splits a node into 2 and returns the newChild
nodePtr splitNode(nodePtr left)
{
    ifdb fprintf(stdout, "Splitting node: "), printLeafNode(left);
    if(left->size < left->max_size) fprintf(stderr, "Attempted to split a node which is not full(%d < %d).\n", left->size, left->max_size), exit(1);
    int i = 0;

    nodePtr right = nodeConstructor(left->max_size);
    right->leaf = left->leaf;
    right->size = left->size/2;
    right->parent = left->parent;

    right->keys = realloc(right->keys, sizeof(key)*left->size/2);




    if(!left->leaf)
    {
        right->children = realloc(right->children, sizeof(key)*(right->size+1));
        for(i = 0; i<(left->size+1)/2; i++) right->children[i] = left->children[i + ((left->size+1)/2)], right->children[i]->parent=right;
        left->children = realloc(left->children, sizeof(nodePtr)*(right->size+1));
    }

    for(i = 0; i<left->size/2; i++) right->keys[i] = left->keys[i + (left->size/2)+1];
    left->keys = realloc(left->keys, sizeof(key)*left->size/2);

    left->size = left->size/2;

    ifdb fprintf(stdout, " to get "), printLeafNode(left), fprintf(stdout, " and "), printLeafNode(right), fprintf(stdout, "\n");

    return right;

}

//extends the node
nodePtr updateRoot(nodePtr old_root)
{
    if(old_root->size < old_root->max_size) fprintf(stderr, "Attempted to split a root which is not full(%d < %d).\n", old_root->size, old_root->max_size), exit(1);
    nodePtr new_root = nodeConstructor(old_root->max_size);
    new_root->leaf = 0;
    new_root->keys = realloc(new_root->keys, sizeof(key)*1);
    new_root->children = realloc(new_root->children, sizeof(nodePtr)*2);
    new_root->parent = 0;
    new_root->size = 1;

    new_root->keys[0] = old_root->keys[old_root->size/2];
    new_root->children[0] = old_root;
    new_root->children[1] = splitNode(old_root);
    old_root->parent = new_root;
    new_root->children[1]->parent = new_root;
    ifdb fprintf(stdout, "NewRoot:\n");
    ifdb printNonLeafNode(new_root, 0);
    return new_root;
}



//inserts into a leaf and returns a val if splitting is needed
int insertLeafNode(nodePtr node, key val)
{
    ifdb fprintf(stdout, "Inserting %d into ", val.value), printLeafNode(node), fprintf(stdout, "\n");
    if(!node->leaf) fprintf(stderr, "Attempted calling insertLeafNode on non-leaf node\n"), exit(1);
    node->keys = realloc(node->keys, sizeof(key)*(node->size+1));
    node->keys[node->size++] = val;
    int i,j;
    for(i = 1; i<node->size; i++)
    {
        key temp = node->keys[i];
        for(j = i-1; j>=0; j--)
        {
            if(node->keys[j].value<=temp.value) break;
            node->keys[j+1] = node->keys[j];
        }
        node->keys[j+1] = temp;
    }
    if(node->size >= node->max_size) return 1;
    else return 0;
}

void printNonLeafNode(nodePtr node, int indent)
{
    int i;
    for(i = 0; i<indent; i++) fprintf(stdout, " ");
    fprintf(stdout,(node->parent == 0) ? "Root:[" : ((node->leaf) ? "Leaf:[":"["));
    for(i=0; i<node->size; i++)
    {
        fprintf(stdout, "(%d: %s),",node->keys[i].value, (char *)node->keys[i].data);
    }
    fprintf(stdout, "\b]\n");
    if(!node->leaf)
        for(i=0; i<=node->size; i++)
        {
            fprintf(stdout, "(%d)", i);
            printNonLeafNode(node->children[i], indent+3);
        }

    return;
}

void rearrangeSubNodes(nodePtr parent, int index)
{
    ifdb fprintf(stdout, "Changing "), printLeafNode(parent), fprintf(stdout,"[%d]:", index), printLeafNode(parent->children[index]),fprintf(stdout, "\n");
    if(parent->leaf || parent->size >= parent->max_size) fprintf(stderr, "Attempted calling rearrange on invalid node\n"), printLeafNode(parent), exit(1);

    parent->size++;
    parent->keys = realloc(parent->keys, sizeof(key)*parent->size);
    parent->children = realloc(parent->children, sizeof(nodePtr)*(parent->size+1));

    int i;
    //move the keys
    ifdb fprintf(stdout, "ParentSize: %d\n", parent->size);
    for(i = parent->size-1; i >= index+1; i--)
    {
        ifdb fprintf(stdout, "Moving Keys[%d] to [%d]\n", i, i-1);
        parent->keys[i] = parent->keys[i-1];
    }
    parent->keys[index] = parent->children[index]->keys[parent->children[index]->size/2];
    nodePtr newNode = splitNode(parent->children[index]);

    //move the children
    for(i = parent->size; i>index+1; i--)
    {
        parent->children[i] = parent->children[i-1];
    }

    parent->children[index+1] = newNode;
    return;
}

int checkChildren(nodePtr node)
{
    if(node->leaf) return -1;
    int i;
    for(i = 0; i<node->size+1; i++)
    {
        nodePtr current = node->children[i];
        if(current->size >= current->max_size) return i;
    }
    return -1;
}

int getIndex(nodePtr node)
{
    if(node->parent == 0) fprintf(stderr, "Node is root, but attempted to be simplified by the wrong function.\n"), exit(1);
    nodePtr parent = node->parent;
    int i;
    ifdb fprintf(stdout, "Parent size %d\n", parent->size+1);
    ifdb printNonLeafNode(parent, 0);
    for(i=0; i<parent->size+1; i++)
    {
        if(parent->children[i] == node) return i;
        ifdb fprintf(stdout, "EVAL:"), printLeafNode(parent->children[i]), fprintf(stdout, "<-NEQAL->"), printLeafNode(node),fprintf(stdout, "\n");
    }
    fprintf(stderr, "node has pointer to parent node, who lacks knowledge of the child.\n"), exit(1);
}

void insertKeyHelper(nodePtr node, key K)
{
    if(node->leaf)
    {
        if(!insertLeafNode(node, K))return;
        else
        {
            if(node->parent != 0)
            {
                rearrangeSubNodes(node->parent, getIndex(node));
                return;
            }
            else return;
        }
    }
    int index;
    while((index = checkChildren(node)) != -1)
    {
        rearrangeSubNodes(node, index);
    }
    int i;
    for(i = 0; i<node->size && node->keys[i].value <= K.value; i++);
    insertKeyHelper(node->children[i], K);
    return;
}

nodePtr insertKey(nodePtr root, key K)
{
    if(root->size >= root->max_size)
    {
        root = updateRoot(root);
        ifdb fprintf(stdout, "Updated root\n");
    }
    insertKeyHelper(root, K);
    return root;
}

key getData(nodePtr node, int value) {
    int i;
    for(i = 0; i<node->size && node->keys[i].value < value; i++);
    if(node->keys[i].value == value) return node->keys[i];
    if(!node->leaf) return getData(node->children[i], value);
    else {fprintf(stdout, "Key not found\n"); key a; a.data = 0; a.value = 0; return a;}
}

nodePtr getNode(nodePtr node, int value) {
    int i;
    for(i = 0; i<node->size && node->keys[i].value < value; i++);
    if(node->keys[i].value == value) return node->children[i];
    if(!node->leaf) return getNode(node->children[i], value);
    else {fprintf(stdout, "Key not found\n"); return 0;}
}

void printLeafNode(nodePtr node)
{
    //if(!node->leaf) fprintf(stderr, "Attempted calling insertLeafNode on non-leaf node\n"), exit(1);
    int i;
    fprintf(stdout, (node->parent == 0) ? "Root:[" : ((node->leaf) ? "Leaf:[":"["));
    for(i=0; i<node->size; i++)
    {
        fprintf(stdout, "(%d: %s),",node->keys[i].value, (char *)node->keys[i].data);
    }
    fprintf(stdout, "\b]");
}


key encodeData(int val, char *string)
{
    key newKey;
    newKey.value = val;
    newKey.data = string;
    return newKey;
}

nodePtr minNode(nodePtr root) {
    if(root->leaf) return root;
    else return minNode(root->children[0]);
}
nodePtr maxNode(nodePtr root) {
    if(root->leaf) return root;
    else return maxNode(root->children[root->size]);
}

nodePtr successor(nodePtr root, int val) {
   nodePtr node = getNode(root, val);
   if(node == 0) fprintf(stdout, "Attempted to find successor of non-present node");
   if(!node->leaf) {
     int index = getIndex(node);
     node = node->parent->children[index+1];
     return minNode(node);
   }


}

key delMin(nodePtr node) {
    if(!node->leaf) fprintf(stdout, "Attempted to delete a key from a non leaf node.\n"), exit(1);
    key result = node->keys[0];
    int i;
    for(i = 1; i<node->size; i++) {
        node->keys[i-1] = node->keys[i];
    }
    node->size--;
    node->keys = realloc(node->keys, node->size*sizeof(key));
    return result;
}

key delMax(nodePtr node) {
    if(!node->leaf) fprintf(stdout, "Attempted to delete a key from a non leaf node.\n"), exit(1);
    key result = node->keys[node->size-1];
    node->size--;
    node->keys = realloc(node->keys, node->size*sizeof(key));
    return result;
}

void printData(key K) {
    if(K.data != 0)
    fprintf(stdout, "(%d:%s)\n", K.value, K.data);
}

void T_insertKey(nodePtr *root, int val) {
    char *store = malloc(sizeof(char)*5);
    snprintf(store, 20, "%d", val);
    key newKey = encodeData(val, store);
    *root = insertKey(*root, newKey);
}

int main()
{
    int i = 0;
    nodePtr root = nodeConstructor(5);
    T_insertKey(&root, 10);
    printNonLeafNode(root, 0);
    T_insertKey(&root, 5);
    printNonLeafNode(root, 0);
    T_insertKey(&root, 2);
    printNonLeafNode(root, 0);
    T_insertKey(&root, 7);
    printNonLeafNode(root, 0);
    T_insertKey(&root, 7);
    printNonLeafNode(root, 0);
    T_insertKey(&root, 7);
    printNonLeafNode(root, 0);
    T_insertKey(&root, 7);
    printNonLeafNode(root, 0);
    T_insertKey(&root, 5);


    printNonLeafNode(root, 0);

}
