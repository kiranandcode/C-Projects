#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define insertQueueTuple(start, value, weight) insertQueueTupleAux(&start, value, weight)
#define popQueueTuple(start) popQueueTupleAux(&start)
#define t_QueueDestructor(start) t_QueueDestructorAux(&start)
#define unionSetVar(a, b) unionSetAux(&a, &b)
#define minValIndex(array) minValIndexAux(array, size(array))
#define size(arr) sizeof(arr)/sizeof(arr[0])
#define DEBUG 1


typedef struct setContainer {
    struct setNode *head;
    struct setNode *tail;
} set, *setPtr;

typedef struct setNode {
    setPtr container;
    int value;
    struct setNode *next;

} element, *elementPtr;

typedef struct elementManager {
    int size;

    elementPtr *elements;

} global_elements, *global_elementsPtr;

typedef struct graph {
    int vertices;
    int **graph;
} graph, *graphPtr;

typedef struct tuple {
    int a,b;
} tuple, tuplePtr;

typedef struct tupleQueue {
    tuple value;
    int weight;
    struct tupleQueue *nextNode;
} t_Queue, *t_QueuePtr;



graphPtr graphConstructor(int vertices);
void graphPrint(graphPtr Adj_Mat);
graphPtr graphDestructor(graphPtr graph);
int getEdge(graphPtr graph, tuple edge);
void setEdge(graphPtr graph, tuple edge, int cost);
tuple graphMinEdge(graphPtr graph);
void testGraphs();



t_QueuePtr t_QueueConstructor(tuple value, int weight);
void t_QueueDestructorAux(t_QueuePtr *start);
tuple getQueueTuple(t_QueuePtr start, int index);
void insertQueueTupleAux(t_QueuePtr *strPtr, tuple value, int weight);
tuple popQueueTupleAux(t_QueuePtr *startptr);
void printTuple(tuple value);
void printT_Node(t_QueuePtr node);
void printT_Queue(t_QueuePtr start);
void testTupleLists();

t_QueuePtr graphToEdges(graphPtr graph);
graphPtr edgesToGraph(t_QueuePtr edges, int vertices);
void testTupleGraphConv(void);


void increaseStoreSize();
void registerElement(elementPtr S);
elementPtr elementFromValue(int value);
void clearStore();

setPtr makeSet(int val);
void unionSetAux(setPtr *a, setPtr *b);
void unionSet(setPtr setA, setPtr setB);
setPtr findSet(int val);
int lengthSet(setPtr set);
void printSet(setPtr set);
void testSet();

void printElement(elementPtr elem);
int minValIndexAux(int* array, int length);

void startScreen();
graphPtr inputGraph();

void testSuite();
#endif // GRAPH_H_INCLUDED
