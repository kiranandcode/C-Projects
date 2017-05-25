#include <stdio.h>

// Defines the size of a word on the system - hopefully
#define WORDSIZE (sizeof(long) * 8)

// Defines the maximum number of bits used in calculations
#define NUMBITS    158

// Defines the maximum index in an array of WORDS used to represent NUMBITS
#define NUMWORD    (NUMBITS/WORDSIZE)

// Defines the unused bits in an array
#define UPRSHIFT   (NUMBITS%WORDSIZE)

// Defines the number of words used to represent NUMBITS
#define MAXLONG    (NUMWORD+1)


#define HALFSIZE   (WORDSIZE/2)

#define HIMASK     (-1L<<HALFSIZE)

#define LOMASK     (~HIMASK)

#define CARRY      (1L<<HALFSIZE)

#define MSB_HW     (CARRY>>1)

#define INTMAX     (4*MAXLONG-1)

// Largest string needed to convert a integer from binary to decimal (y = log(2) * MAXLONG * WORDSIZE
#define MAXSTRING  (MAXLONG*WORDSIZE/3)


#define INTLOOP(i)      for (i=INTMAX; i>= 0; i--)

typedef unsigned long ELEMENT;

typedef short int INDEX;

typedef struct {
    ELEMENT          hw[4*MAXLONG];
} BIGINT;



void debug_print_binary(unsigned long long int value) {
    unsigned long long int val = value;
    int size = 0;
    while(value != 0) {
        //printf("Size:%d,value: %lld\n", size, value);
        value /= 2;
        size++;
    }
    int i;
    char vals[size+1];
    for(i = size-1; i>=0; i--) {
        //printf("val(%lld) %% 2 == (%lld)\n", val, (val%2));
        if(val %2 == 0) vals[i] = '0';
        else vals[i] = '1';
        val /= 2;
    }
   vals[size] = '\0';

    printf("%s\n",vals);


}



void int_null( BIGINT *a){
    INDEX i;
    INTLOOP(i) a->hw[i] = 0;
}

void int_copy(BIGINT *a, BIGINT *b) {
    INDEX i;

    INTLOOP(i) b->hw[i] = a->hw[i];
}

void int_add(BIGINT *a, BIGINT *b, BIGINT *c) {
    INDEX i;
    ELEMENT ec;

    ec = 0;
    INTLOOP(i) {
        // Add the carry bit to each term
        // How does this work?
        // -------------------
        // It works under the assumption that the second half of the memory for
        // the variable is used to store the carry over from addition.
        // Thus after each loop, add the carry bit from the prior iteration.
        ec = a->hw[i] + b->hw[i] + (ec >> HALFSIZE);


        // Here, once added, keep only the low bits, as the high bits will be used as the carry in the next iteration
        c->hw[i] = ec & LOMASK;
    }
}

// Function negates a BIGINT in place - twos complement and then add 1.
void int_neg(BIGINT *a) {
    register INDEX i;

    // First take the inversion of each element of the value, removing any bits that exceed the LOMASK as necassary - i.e ~000011111 = 111100000, but the high bits are not needed
    INTLOOP(i) a->hw[i] = ~a->hw[i] & LOMASK;

    // Then add 1 to the value, to do this, simply add 1 to each element, if causes no carry stop.
    // I.e  0001   11111   1110 + 1  == 0001 1111  1111
    INTLOOP(i) {
        a->hw[i]++;
        if(a->hw[i] & LOMASK) break;
        a->hw[i] &= LOMASK;
    }
}

void int_sub(BIGINT *a,BIGINT *b,BIGINT *c) {
    BIGINT negb;
    int_copy(b, &negb);
    int_neg(&negb);
    int_add(a, &negb, c);
}

void int_mul(BIGINT *a, BIGINT *b, BIGINT *c) {
    ELEMENT ea, eb, mul;
    INDEX   i , j , k  ;
    BIGINT  sum;

    int_null(c);
    for(i = INTMAX; i>INTMAX/2; i--) {
        ea = a->hw[i];
        int_null(&sum);
        for(j=INTMAX; j>INTMAX/2; j--) {
            eb = b->hw[j];
            k = i + j - INTMAX;
            mul = ea * eb + sum.hw[k];
            sum.hw[k] = mul & LOMASK;
            sum.hw[k-1] = mul >> HALFSIZE;
        }
        int_add(&sum, c, c);
    }
}


int main() {
    debug_print_binary(HIMASK);
    debug_print_binary(CARRY);
    printf("%d\n", sizeof(unsigned long) * 8);
    return 0;
}

