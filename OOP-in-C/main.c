#include <stdio.h>
#include <stdlib.h>

#include "new.h"
#include "Object.h"
#include "Set.h"


int main()
{

const void *Set;
const void *Object;
    void* s = new(Set);
    void* a = add(s, new(Object));
    void* b = add(s, new(Object));
    void* c = new(Object);

    if(contains(s, a) && contains(s, b)) puts("Add-Contains Relationship Verified");

    if(contains(s,c)) puts("Issues with contains function - checking incorrectly");

    if(differ(a, add(s,a))) puts("Issues with add function - adding duplicates");

    if(contains(s, drop(s, a))) puts("Issues with Drop-Contains Relationship");

    delete(drop(s, b));
    delete(drop(s, c));

    return 0;
}
