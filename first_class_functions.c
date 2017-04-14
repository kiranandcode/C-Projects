#include <stdio.h>
#include <malloc.h>

typedef enum type {
    VOID,
    INT,
    INTPTR,
    FLOAT,
    FLOATPTR,
} type;

typedef struct object {
    type t;
    void *param;
} object;

typedef struct objects {
    int count;
    object *objects;
} collection;

typedef struct thunk *(function)(struct thunk*);

typedef enum optionality {
    NOTHING,
    VALUE,
    APPLICATION
} optionality;

typedef struct thunk {
    optionality status;
    typedef union {
        struct {
            function *f;
            struct thunk* params;
        } application;
        struct {
            collection *col;
        } value;
    } value;
    value val;

} thunk;

/**
 * @brief Given a collection of items, encapsulates them so that they may be used by first class functions.
 * @param items A collection of items, could be 1 or even empty
 * @return A thunk value representing the object.
 */
thunk *thunkItems(collection *items) {
    thunk *result = malloc(sizeof(thunk));
    result->status = VALUE;
    result->val.value.col = items;
    return result;
}

thunk *getNothing() {
    thunk *nothing = malloc(sizeof(thunk));
    nothing->status = NOTHING;
    return nothing;
}

/**
 * @brief Given a function and a thunk, returns an unevaluated value;
 * @param firstclassf A first class function.
 * @param params A thunk or value to apply.
 * @return An thunk representing the application of the function to the thunk.
 */
thunk *createPromise(function *firstclassf, thunk *params) {

    // This allows for propogation of errors by just continuing nothing.
    if(params->status == NOTHING) return getNothing();

    thunk *result = malloc(sizeof(thunk));
    result->status = APPLICATION;
    result->val.application.f = firstclassf;
    result->val.application.params =  params;

    return result;
}

/**
 * @brief Recursively defined function which evaluates a promise and returns the result.
 * @param promise A thunk representing an unevaluated function.
 * @return a thunk representing the evaluated result
 */
thunk *evaluate(thunk *promise) {
    // Base case.
    if(promise->status == VALUE || promise->status == NOTHING) return promise;

    // Updates the thunked item to
    return promise->val.application.f(evaluate(promise->val.application.params));

}

thunk* firstclass_add(thunk * params) {
    if(params->status != VALUE) return getNothing();

    collection *items = params->val.value.col;

    double sum = 0;
    int i;
    for(i = 0; i<items->count; i++) {
        object *o = &items->objects[i];
        switch(o->t) {
            case VOID:
                break;
            case INT:
                sum += *((int *)o->param);
                break;
            case FLOAT:
                sum += *((float*)o->param);
                break;
            default:
                break;
        }

    }

    collection *obj = malloc(sizeof(collection));
    object *obje = malloc(sizeof(object));
    obj->count = 1;
    float *val = malloc(sizeof(float));
    *val = (float) sum;
    obje->param = val;
    obje->t = FLOAT;
    obj->objects = obje;

    thunk *result = malloc(sizeof(thunk));
    result->status = VALUE;
    result->val.value.col = obj;

    return result;
}




int main()
{
    printf("Hello, World!\n");
    return 0;
}