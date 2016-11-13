#include "LinkedLists.h"
Node *createHead(value)
{
    Node *head = malloc(sizeof(Node));
    head->data = value;
    head->pointer = 0;
    return head;
}

//returns the location of the value - implicitly returning the length
int appendValue(Node *head, int value)
{
    Node *current = head;
    int counter = 0;
    while(current->pointer != 0)
    {
        current = current->pointer;
        counter++;
    }

    Node *newNode = malloc(sizeof(Node));

    current->pointer = newNode;

    newNode->data = value;
    newNode->pointer = 0;

    return counter;
}

int removeValueAtIndex(Node *head, int index)
{
    Node *current = head;
    int counter = 0;
    int endOfList = 0;
    while(counter != index-1)
    {
        if (current->pointer == 0)
        {
            return 0;
        }
        current = current->pointer;
        counter++;
    }
    Node *toBeRemoved = current->pointer;
    current->pointer = toBeRemoved->pointer;
    int removed = toBeRemoved->data;
    free(toBeRemoved);
    return removed;
}



void insertValueAtIndex(Node *head, int index, int value)
{
    Node *current = head;
    int counter = 0;
    int endOfList = 0;
    while(counter != index-1)
    {
        if (current->pointer == 0)
        {
            return;
        }
        current = current->pointer;
        counter++;
    }
    Node* newNode = malloc(sizeof(Node));

    Node *afterNode = current->pointer;

    current->pointer = newNode;

    newNode->data = value;
    newNode->pointer = afterNode;
    return;
}




int returnValueAtIndex(Node *head, int index)
{
    Node *current = head;
    int counter = 0;
    int endOfList = 0;
    while(counter != index)
    {
        if (current->pointer == 0) return 0;
        current = current->pointer;
        counter++;
    }
    return current->data;
}

void printList(Node *head)
{
    Node *current = head;
    int counter = 0;
    printf("Value at index [%d]: %d\n", counter, current->data);
    while(current->pointer != 0)
    {
        current = current->pointer;
        counter++;
        printf("Value at index [%d]: %d\n", counter, current->data);
    }
    return;
}
