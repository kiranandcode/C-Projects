#include "KQueue.h"


Queue *CreateQueue(int length)
{
    int *array = malloc(sizeof(int)*length);
    Queue *output = malloc(sizeof(Queue));
    *output = (Queue)
    {
        .reference = array, .start = array, .end = array, .length = length
    };
    return output;
}

int isEmpty(Queue queue)
{
    if (queue.start == queue.end) return 1;
    else return 0;
}
//  full when [###,end,start,###, ###] or [start, ###, ###, end]
int isFull(Queue queue)
{
    int start = queue.reference - queue.start;
    int end = queue.reference - queue.end;
    int length = queue.length;

    if ((end+1)%length == start) return 1;
    else return 0;
}

int dequeue(Queue *queue)
{
    if(!isEmpty(*queue))
    {
        if(queue->end != queue->reference) return *(queue->end)--;
        else
        {
            queue->end = (queue->end)+queue->length-1;
            return *(queue->end);
        }
    }
}

void enqueue(Queue *queue, int value)
{
    if(!isFull(*queue))
    {
        if(queue->start != queue->reference)
        {
            queue->start--;
            *(queue->start) = value;

        }
        else
        {
            queue->start += queue->length-1;
            *(queue->start) = value;
        }
    }
    return;
}


void printQueue(Queue *queue)
{
    printf("Input queue of max length: %d\n", queue->length);
    int length;
    int start = (queue->reference - queue->start)%queue->length;
    if(start<0) start = -start;
    int end = (queue->reference - queue->end)%queue->length;
    if(end<0) end = -end;
    printf("Start: %d, End: %d\n", start, end);
    if(!isEmpty(*queue))
    {

        if(start < end)
        {
            length = end-start;
        }
        else
        {
            length = (queue->length - start) + end;
        }
        printf("Length of Queue is: %d\n", length);
        int i;
        for(i = start; i<length+start; i++)
        {
            printf("value at index %d: ", i%(queue->length));
            printf("%d\n", *(queue->reference +i%queue->length));
        }
    }
    return;
}







void queue_test()
{
    Queue* basic = CreateQueue(10);
    Queue normie = *basic;
    printQueue(basic);
    printf("%d\n", isEmpty(normie));
    enqueue(basic, 10);
    enqueue(basic, 12);
    enqueue(basic, 13);
    enqueue(basic, 14);
    dequeue(basic);
    dequeue(basic);
    enqueue(basic, 23);
    dequeue(basic);
    printQueue(basic);
}
