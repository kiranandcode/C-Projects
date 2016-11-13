#include <stdio.h>
#define ARRAY_SIZE(array) sizeof(array)/sizeof(array[0])
#define TEST 90

int input[] = {1,3,5};
long solution = 0;
int preval;


main() {
    preval = TEST;
    funct(TEST);
    printf("%d",solution);
}

int funct(x) {
    if (x == 0) {
        //printf("solution found\n");
        solution++;
        return 0;
    }
    else if (x < 0) {
        //printf("reached 0 or less: %d\n", x);
        return 0;
    }
    else {
        int i;
        for(i = 0; i < ARRAY_SIZE(input); i++){
            if (input[i] >= (preval - x)) {
                //printf("iterating over %d with %d\n", x, input[i]);
                preval = x;
                funct((x - input[i]));
            };
        };
    };
}



