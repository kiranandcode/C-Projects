//
// Created by gopia on 01/05/2017.
//

#include "list.h"

/*
 * Example Use
 * ----------------------------------
 *  list *ls = getNewList(INT);
    append(ls, &value);
    append(ls, &val);
    printList(ls);
    pop(ls);
    append(ls, &val);
    append(ls, &val);
    append(ls, &val);
    printList(ls);
    printf("\n%d\n", length(ls, LIST));
    append(ls, &value);
    append(ls, &val);
    append(ls, &value);
    append(ls, &value);
    append(ls, &val);
    append(ls, &value);

    printList(ls);


    list *lss = getNewList(LIST);
    append(lss, ls);

    append(lss, ls);

    append(lss, ls);

    append(lss, ls);
    printList(lss);
    printList(ls);
 */


void (*print[COUNT]) (void *) = {_print_INT, _print_LIST, NULL};


void _print_INT(void * element) {
    int item = *((int*) element);
    printf("%d", item);
}








list *getNewList(types t) {
    list *ls = malloc(sizeof(list));
    ls->head = NULL;
    ls->type = t;
}




void _print_LIST(void *element) {
    list *ls = (list *) element;
    printList(ls);
}







void append(list *ls, void *obj) {

    // Get the pointer to the head of the list.
    elem *head = ls->head;

    // Iterate through the list until the element pointing to NULL is reached.
    for(;head != NULL && head->next != NULL; head = head->next);

    // Construct the next element.
    elem *next = malloc(sizeof(elem));

    // Set it's next element to null
    next->next = NULL;

    // Update the last element of the list to point to this - only if non empty
    if(head != NULL)
        head->next = next;


    // Set/Update the head pointer of the list and of the element
    if(ls->head != NULL) {
    }
    else {
        // This case is mainly if the list is empty
        ls->head = next;
    }


    next->ls = ls;
    // Set the element to append - no need to worry about it's type.
    next->element = obj;


    return;
}


int length(void *obj, enum types t) {


    switch(t) {

        case INT:
            return *((int *)obj);
        case LIST:
        {
            list *ls = (list *) obj;
            elem *head;
            int i = 0;
            for(head = ls->head; head != NULL; head = head->next) ++i;
            return i;
        }
        default:
            return 0;
    }
}

void *get(int index, list *ls) {
    // Get the head element
    elem *head = ls->head;

    // Quick check to see if input parameters make sense.
    if(head == NULL || index < 0) return NULL;

    // Iterate over the items
    for(; head->next != NULL && index > 0; head = head->next);

    // Check success end conditions.
    if(index == 0) return head->element;
    else return NULL;

}


void del(int index, list *ls) {
    // Get the head element
    elem *head = ls->head;
    elem *prev = NULL;
    // Quick check to see if input parameters make sense.
    if(head == NULL || index < 0) return;

    // Iterate over the items
    for(; head->next != NULL && index > 0; prev = head, head = head->next, index--);

    // Check success end conditions.
    if(index == 0) {
        // Accounting for 0 index removal
        if(prev == NULL) {

            // Set head of list to null.
            ls->head = head->next;

            // Delete head and it's element
            //if(head->element != NULL) free(head->element);
            free(head);
        }
        else {
            // Set prev->next to head->next
            prev->next = head->next;

            // Delete head and it's element
            //if(head->element != NULL) free((int *)head->element);

            free(head);
        }
    }

}

void *pop(list *ls) {
    // Get the head element
    elem *head = ls->head;

    // Quick check to see if input parameters make sense.
    if(head == NULL) return NULL;

    // Accounting for 1 element list
    if(head->next == NULL) {
        void *obj = head->element;
        free(head);
        ls->head = NULL;
        return obj;
    }

    // Iterate over the items
    for(; head->next->next != NULL; head = head->next);

    // Get the item
    void *obj = head->next->element;

    // Free the list
    if(head->next != NULL) free(head->next);

    // Remove the pointer
    head->next = NULL;

    // Return the value.
    return obj;

}

void printList(list *ls) {
    elem *head;
    int i = 0;
    printf("[");
    for(head = ls->head; head != NULL; head = head->next) print[ls->type](head->element), printf(", "), ++i;
    if(i != 0) printf("\b\b");
    printf("]");
}