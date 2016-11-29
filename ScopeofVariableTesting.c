#include <stdio.h>

int globaledited;
int localedited;
globaledited = 333;

main() {
    localedited = 333;

    printf("Global edited: %d \n", globaledited);
    printf("Local edited: %d \n", localedited);

    globaleditor();
    localeditor();

    printf("Global edited: %d \n", globaledited);
    printf("Local edited: %d \n", localedited);


}

globaleditor(){
    globaledited = 666;
}

localeditor(){
    auto int localedited;
    localedited = 555;
    printf("Localeditor says localedited is %d \n", localedited);
    {extern localedited;
    localedited = 666;
    printf("Global scope indicates that it is %d\n", localedited);}
    printf("But still Localeditor says localedited is %d \n", localedited);
}

//When a variable is referenced, first the compiler checks the local scope, then the global scope - compiler definitions just mean that you cant have two same name variables in the same namespace - just separate uses by brackets
