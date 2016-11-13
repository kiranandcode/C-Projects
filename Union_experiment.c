#include <stdio.h>
//This file is a little excursion into how unions work, just trying to work it all out

//declare variables
union myStruct {
    int number;
    char value;
    double longer;
} case1;

union myStruct *pointer;


//declare functions
void printUnion(void);



//main function
int main() {
    pointer = &case1;
    //case1.longer = 45.1;
    case1.number = 24;
    case1.value = 'a';
    printUnion();


}




//define functions
void printUnion(){
    printf("%c\n", pointer->number);
    printf("%d",pointer->number);
    //printf("%lf", case1.longer);


}
