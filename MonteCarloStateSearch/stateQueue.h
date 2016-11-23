#ifndef STATEQUEUE_H
#define STATEQUEUE_H

typedef struct stateQueueNode {
    int state;
    int weight;
    struct stateQueueNode *next;
} node;

typedef struct stateQueueHead {
    int length;
    node *start;
} head;

head *start;
head *startQueue(void);
void insertState(head *queue, int state, int weight);
int retrieveState(head *queue);
int checkQueue(head *queue, int state);
void setWeight(head *queue, int state, int weight);
void printQueue(head *queue);
void altPrintQueue(head *queue);

#endif // STATEQUEUE_H
