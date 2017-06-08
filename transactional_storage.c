/**
 *         Gopiandcode - C Code
 * ------------------------------------
 *  This code was created by gopia on 08/06/2017.
 *
 *  Gopiandcode's attempt at a fluent interfaced storage system.
 *
 *  Was doing a lot of background reading, so hadn't the chance to do any
 *  original programming. So, I thought I'd relax kick back and implement
 *  a cheeky quicksort and insertion sort alongside a basic fluent-like
 *  interface system.
 *
 *  Enjoyed? Check out my github for more: https://github.com/Gopiandcode
**/

#include <elf.h>
#include <stdlib.h>
#include <stdio.h>
#include "transactional_storage.h"

void insert_list_int(void *list_int, void *a) {
    struct list_int *list = list_int;
    int value = a;

    list->length++;
    list->items = realloc(list->items, sizeof(int) * list->length);

    list->items[list->length-1] = value;
}

void remove_list_int(void *list_int, int index) {
    struct list_int *list = list_int;
    int i;

    for(i = index; i<list->length-1; i++) {
        list->items[i] = list->items[i+1];
    }

    list->length--;
    list->items = realloc(list->items, sizeof(int) * list->length);
}

void print_list_int(void *list_int) {
    struct list_int *list = list_int;
    int i;
    printf("[");
    for(i = 0; i<list->length; i++) {
        printf("%d, ", list->items[i]);
    }
    printf("\b\b]\n");
}


void sort_quick_list_int(void *list_int) {
    struct list_int *list = list_int;

    int i,j;

    for(i = 1; i<list->length; i++) {
        int save = list->items[i];

        for( j = i-1; j >= 0; j--) {
            if(list->items[j] > save) {
                int temp = list->items[j];
                list->items[j] = list->items[j+1];
                list->items[j+1] = temp;
            } else {
                break;
            }
        }

        list->items[j+1] = save;

    }

}

void sort_full_list_int(void *int_list) {
    struct list_int *list = int_list;
    r_qsort_int_list(list->items, list->length);
}

void r_qsort_int_list(int *items, int length) {
    if(length <= 1) return;

    int pivot = items[length-1];
    int j = 0, i;
    // Pivot
    for(i = 0; i<length-1; i++) {
        if(items[i] < pivot) {
            int temp = items[j];
            items[j] = items[i];
            items[i] = temp;
            j++;
        }
    }
    items[length-1] = items[j];
    items[j] = pivot;

    r_qsort_int_list(items, j);
    r_qsort_int_list(items+j-1, length-j);
}

void *begin_modification(void *database) {
    struct sorted_database* sorted_db = database;

    if(database == NULL || sorted_db->transaction) return NULL;
    sorted_db->transaction = 1;
    return database;
}

void *insert_database(void *database, void *item) {
    struct sorted_database *sorted_db = database;

    if(database == NULL || !sorted_db->transaction) return NULL;
    sorted_db->insert(sorted_db->database, item);
    sorted_db->transaction_count++;

    return database;
}

void *remove_database(void *database, int item) {
    struct sorted_database *sorted_db = database;

    if(database == NULL || !sorted_db->transaction) return NULL;
    sorted_db->remove(sorted_db->database, item);
    sorted_db->transaction_count++;

    return database;
}


void *end_modification(void *database) {
    struct sorted_database *sorted_db = database;

    if(database == NULL || !sorted_db->transaction) return NULL;

    if(sorted_db->transaction_count > sorted_db->transaction_threshhold) {
        sorted_db->sort_full(sorted_db->database);
    } else {
        sorted_db->sort_quick(sorted_db->database);
    }

    sorted_db->transaction_count = 0;
    sorted_db->transaction = 0;

    return database;
}

void *print_database(void *database) {
    struct sorted_database *sorted_db = database;
    sorted_db->print(sorted_db->database);
    return database;
}

struct list_int *list_int_cons() {
    struct list_int* item = malloc(sizeof(struct list_int));
    item->items = NULL;
    item->length = 0;
    return item;
}

struct sorted_database *empty_sorted_database_cons(void *list) {
    struct sorted_database *database = malloc(sizeof(struct sorted_database));
    database->database = list;
    database->transaction_count =  0;
    database->transaction = 0;
    return database;

}

struct sorted_database *list_int_database_cons(int threshhold) {
    struct list_int *list = list_int_cons();
    struct sorted_database *database = empty_sorted_database_cons(list);
    database->transaction_threshhold = threshhold;
    database->insert = insert_list_int;
    database->remove = remove_list_int;
    database->print = print_list_int;
    database->sort_full = sort_full_list_int;
    database->sort_quick = sort_quick_list_int;
    return database;
}

int main() {
    struct sorted_database *database = list_int_database_cons(10);
    database = begin_modification(database);
    database = insert_database(database, 10);
    database = print_database(database);
    database = insert_database(database, 20);
    database = insert_database(database, 4);
    database = print_database(database);
    database = end_modification(database);
    database = print_database(database);
    database = begin_modification(database);
    database = remove_database(database, 2);
    database = end_modification(database);
    database = print_database(database);


}
