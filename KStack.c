#include "KStack.h"

Stack *createStack(int length)
{
    int *array = malloc(sizeof(int)*length);
    Stack *stack = malloc(sizeof(stack));
    *stack = (Stack)
    {
        .length = length, .current = array, .reference = array
    };
    return stack;
}


int isEmptyStack(Stack stack)
{
    if (stack.current == stack.reference) return 1;
    else return 0;
}

int isFullStack(Stack stack)
{
    if(stack.current == stack.reference+stack.length-1) return 1;
    else return 0;
}


void push(Stack *stack, int value)
{
    if(!isFullStack(*stack)) *stack->current++ = value;
}

int pop(Stack *stack)
{
    if(!isEmptyStack(*stack)) return *stack->current--;
}

void printStack(Stack stack)
{
    int length = (stack.current - stack.reference);
    printf("Input stack of size %d, max size: %d\n", length, stack.length);
    int i;
    for(i = 0; i<length; i++) printf("Value at index [%d]: %d\n", i, *(stack.reference+i));
}


void stackTest()
{
    Stack dummy;
    Stack* ptrtoStack = createStack(10);
    dummy = *ptrtoStack;
    push(&dummy, 20);
    push(&dummy, 10);
    push(&dummy, 20);
    push(&dummy, 15);
    push(&dummy, 20);
    push(&dummy, 13);
    push(&dummy, 20);
    pop(&dummy);
    printStack(dummy);
}
