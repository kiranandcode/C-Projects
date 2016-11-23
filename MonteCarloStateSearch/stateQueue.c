#include <stdio.h>
#include <stdlib.h>


#include "stateQueue.h"

head *startQueue(void) {
    head *output;
    output = malloc(sizeof(head));
    output->length = 0;
    return output;
}


void insertState(head *queue, int state, int weight) {
    queue->length++;
    node *newNode = malloc(sizeof(node));

    if (queue->length <= 1) {
        newNode->state = state;
        newNode->weight = weight;
        queue->start = newNode;
        newNode->next = 0;
        return;
    }

    node *currentNode = queue->start;

    while(1) {
        if (currentNode->weight > weight) {
            newNode->state = currentNode->state;
            newNode->weight = currentNode->weight;
            newNode->next = currentNode->next;

            currentNode->state = state;
            currentNode->weight = weight;
            currentNode->next = newNode;

            return;
        }


        if(currentNode->next == 0) {
            currentNode->next = newNode;
            newNode->state = state;
            newNode->weight = weight;
            newNode->next = 0;
            return;
        }

        currentNode = currentNode->next;
    }
}

int retrieveState(head *queue) {
    if(queue->length <= 0 || queue->start == 0) fprintf(stderr, "ERROR: attempted to retrieve from empty queue\n"), exit(1);
    node *ref = queue->start;
    queue->start = ref->next;
    queue->length--;

    int output = ref->state;
    free(ref);
    return output;
}

// Unused function, checks whether a state exists in the queue
int checkQueue(head* queue, int state) {
    if (queue->start == 0) return 0;
    node *currentNode = queue->start;
    while(1) {
        if (currentNode->state == state) return 1;

        if(currentNode->next == 0) return 0;
        else currentNode = currentNode->next;
    }
}

// Unused function, prints the raw values stored in a queue
void printQueue(head* queue) {
    int counter = 0;
    if (queue->start == 0) {return;}
    node *currentNode = queue->start;
    while(1) {
        fprintf(stdout, "[%d] : %d\n",counter++, currentNode->state);

        if(currentNode->next == 0) return;
        else currentNode = currentNode->next;
    }
}

// Used to print queues storing positions, converts to format required by question
void altPrintQueue(head* queue) {
    int counter = 0;
    if (queue->start == 0) {return;}
    node *currentNode = queue->start;
    while(1) {
        fprintf(stdout, "[%d] : %d\n",counter++, convToGood(currentNode->state));
        if(currentNode->next == 0) return;
        else currentNode = currentNode->next;
    }
}

//Unused function to change the weight of a value while still maintaining the sorted property
void setWeight(head *queue, int state, int weight) {
    if ((queue->start == 0) || !checkQueue(queue, state)) {insertState(queue, state, weight); return;}
    int refState = state;
    int refWeight = weight;
    int temp;
    node *currentNode = queue->start;
    while(1) {
        if (currentNode->state == state){
            currentNode->state = refState;
            currentNode->weight = refWeight;
            return;
        }

        if(currentNode->weight > refWeight) {
            temp = currentNode->weight;
            currentNode->weight = refWeight;
            refWeight = temp;

            temp = currentNode->state;
            currentNode->state = refState;
            refState = temp;
        }

        if(currentNode->next == 0) fprintf(stderr, "ERROR: In setWeight, despite checking whether state in queue, state still not found.\n"), exit(1);
        currentNode = currentNode->next;
    }


}


