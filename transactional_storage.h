/**
 *         Gopiandcode - C Code
 * ------------------------------------
 *  This code was created by gopia on 08/06/2017.
 *  
 *  Enjoyed? Check out my github for more: https://github.com/Gopiandcode
**/

#ifndef CODEJAM_TRANSACTIONAL_STORAGE_H
#define CODEJAM_TRANSACTIONAL_STORAGE_H

struct sorted_database {
    void (*sort_quick)(void *);
    void (*sort_full )(void *);
    void (*insert )   (void *, void *);
    void (*remove )   (void *, int   );
    void (*print)     (void *);
    void *database;
    int transaction_count;
    int transaction;
    int transaction_threshhold;
};


struct list_int {
    size_t length;
    int *items;
};

void insert_list_int(void *list_int, void *a);

void remove_list_int(void *list_int, int index);

void print_list_int(void *list_int);

void sort_quick_list_int(void *list_int);

void sort_full_list_int(void *int_list);

void *begin_modification(void *database);

void *insert_database(void *database, void *item);

void *remove_database(void *database, int item);

void *end_modification(void *database);

void *print_database(void *database);

struct sorted_database *list_int_database_cons(int threshhold);


#endif //CODEJAM_TRANSACTIONAL_STORAGE_H
