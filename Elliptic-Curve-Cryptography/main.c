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

        // For each HALFSIZE Digit
        ea = a->hw[i];
        int_null(&sum);


        for(j=INTMAX; j>INTMAX/2; j--) {
            // Multiply by the all other digits
            eb = b->hw[j];

            // As i iterates from IntMax to IntMax/2
            // As j iterates from IntMax to IntMax/2
            // k iterates from IntMax to Intmax/2, then Intmax/2 to 0

            k = i + j - INTMAX;
            mul = ea * eb + sum.hw[k];


            // Add the lower half to it's location
            sum.hw[k] = mul & LOMASK;

            // Add the upper half to the next digit over
            sum.hw[k-1] = mul >> HALFSIZE;
        }

        // Finally add the resulting sum to the accumulator
        int_add(&sum, c, c);
    }
}

void int_div(BIGINT *top, BIGINT *bottom, BIGINT *quotient, BIGINT *remainder) {
    BIGINT d,e;
    ELEMENT mask;
    INDEX l,m,n,i,j;
    int_copy(bottom, &e);
    int_copy(top, &d);
    l = (INTMAX +1) * HALFSIZE;

    // These two loops find the MSB location (in bits) and set L equal to it
    for(i = 0; i<=INTMAX; i++) {
        // First, find the chunk where numbers start
        if(!d.hw[i]) l -= HALFSIZE;
        else break;
    }

    mask = MSB_HW;
    for(j=0; j<HALFSIZE; j++) {
        // Then loop back until the msb is found
        if(!(d.hw[i] & mask)){
            l--;
            mask >>= 1;
        }
        else break;
    }

    m = (INTMAX +1) * HALFSIZE;

    for(i = 0; i<=INTMAX; i++) {
        if(!e.hw[i]) m-= HALFSIZE;
        else break;
    }
    mask = MSB_HW;
    for(j =0; j<HALFSIZE; j++) {
        if(!(e.hw[i] & mask)) {
            m--;
            mask >>= 1;
        }
        else break;
    }

    // Not complete division - does not check for division by 0 error, this check is for
    // division by 1
    if(!m) {
        int_copy(top, quotient);
        int_null(remainder);
        return;
    }

    // If bottom larger than top, division returns 0
    if(!l | (l<m)) {
        int_null(quotient);
        int_copy(top,remainder);
        return;
    }


    // To do division, shift bottom bits to top i.e
    //            3889 / 5
    //            ---------
    //            38
    //          /  5
    //             38
    //              5
    //        ..... and so on.


    n = l-m; // The difference between the msb of the denominator and the numerator - the distance to be shifted
    i = n;
    // First loop aligns the chunks
    // Obviously, as n is in bits, i > HALFSIZE is equivalent to saying the chunk represented by i is greater than 0
    while (i > HALFSIZE) {
        // Shift all the bits up one
        for(j=0; j<INTMAX; j++) e.hw[j] = e.hw[j+1];
        i-= HALFSIZE;
        // All the bits have been shifted up one, so now a 0 exists in the last place
        e.hw[INTMAX] = 0;
    }

    mask = 0;
    // This loop aligns the bits
    while(i > 0) {
        // Then we shift each section up bits until the MSB's align
        INTLOOP(j){
            // The mask stuff is to bring over the carry bit to the next chunk if necassary.


            e.hw[j] = (e.hw[j] << 1) | mask;
            mask = e.hw[j] & CARRY ? 1 : 0;
            e.hw[j] &= LOMASK;
        }
        i--;
    }


    int_null(quotient);
    while(n>=0) {
        // Finding the start index of the numbers
        i = INTMAX - l/HALFSIZE;
        // Finding the end index of the denominator
        j = INTMAX - n/HALFSIZE;

        // Loop iterates through the chunks until a differing pair is found
        while((d.hw[i] == e.hw[i]) && (i < INTMAX)) i++;

        // if the denominator can be subtracted from the numerator, subtract and store the result in the numerator
        if(d.hw[i] >= e.hw[i]) {
            int_sub(&d, &e, &d);
            // Find the corresponding location in the quotient
            // Modulo HALFSIZE to put a corresponding bit in the quotient.
            // Simmilar to how we divide and find the closest integer such that a*int = b, in binary, it's either it divides or doesn't.
            mask = 1L << (n % HALFSIZE);
            quotient->hw[j] |= mask;
        }

        INTLOOP(j){
            // decrement the denominator by unit - (this involves shifting each chunk by one, carrying the bit if necassary)
            if(j) mask = (e.hw[j-1] & 1) ? CARRY : 0;
            else mask = 0;
            e.hw[j] = (e.hw[j] | mask) >> 1;
        }
        n--;
        l--;
    }
    int_copy(&d, remainder);
}

void ascii_to_bigint(char *instring, BIGINT *outhex) {
    ELEMENT       ch;
    BIGINT        ten, digit, temp;
    INDEX         i = 0;

    int_null(&ten);
    // Tens is a bigint storing powers of 10 for representing the values.
    ten.hw[INTMAX] = 0xA;
    int_null(&digit);
    int_null(outhex);

    while(ch=*instring++){
        // Quick, crude way to convert an ASCII number into binary representation
        // Why? ASCII numbers '0' to '9' exist between the range 110000 to 1101001.
        //      Thus by keeping only the first 4 bits, we can quickly convert a valid
        //      ASCII character to it's binary representation
        digit.hw[INTMAX] = ch & 0xF;
        // Multiply the prior stored value by 10
        int_mul(outhex, &ten, &temp);
        // Add Value - (first check if valid input).
        if(digit.hw[INTMAX] > 9) continue;

        int_add(&temp, &digit, outhex);
    }
}


void bigint_to_ascii(BIGINT *inhex, char *outstring) {
    BIGINT top, ten, quotient, remainder;
    ELEMENT check;
    INDEX i;
    int_copy( inhex, &top);

    // Create a 10 value constant
    int_null(&ten);
    ten.hw[INTMAX] = 0xA;

    // Clear the string
    for(i = 0; i<MAXSTRING; i++) *outstring++ = ' ';
    *outstring-- = 0;

    check = 1;
    while(check) {
        int_div(&top, &ten, &quotient, &remainder);

        // Sneaky convert from binary to ascii
        *outstring-- = remainder.hw[INTMAX] | '0';

        check = 0;

        // Check remains false if all the chunks in the quotient are zero
        INTLOOP(i) check |= quotient.hw[i];

        // Replace the value by it's quotient
        int_copy(&quotient, &top);

    }
}


// Fast integer division by two using a right shift(Big Endian format)
void int_div2(BIGINT *x){
    INDEX j;
    ELEMENT mask;

    INTLOOP(j) {
        if(j) mask = (x->hw[j-1] & 1) ? CARRY : 0;
        else mask = 0;
        x->hw[j] = (x->hw[j] | mask) >> 1;

    }

}

void int_gcd(BIGINT *u, BIGINT *v, BIGINT *w) {
    INDEX k, i, flag;
    ELEMENT check, carry_bit;
    BIGINT t, U, V;

    // Make a copy of the input to prevent modification.
    int_copy(u, &U);
    int_copy(v, &V);

    // Counter K will keep track of common powers of two for both inputs.
    // Provides efficiency due to fast division by two through right shifts.
    k = 0;
    while(!(U.hw[INTMAX] & 1 || V.hw[INTMAX] & 1)){
        k++;
        int_div2(&U);
        int_div2(&V);

    }

    // If an even value remains, place it in T
    if(U.hw[INTMAX] &1) {
        int_copy(&V, &t);
        flag = -1;
    } else {
        int_copy(&U, &t);
        flag = 1;
    }


    check = 0;
    // Check is a variable used to test whether t == 0
    INTLOOP(i) check |= t.hw[i];
    while(check) {
        // Remove the factors of 2
        while(!(t.hw[INTMAX] & 1)) int_div2(&t);


        // Having divided the value, place it back into it's original variable
        if(flag > 0) int_copy(&t, &U);
        else int_copy(&t, &V);

        // Set t = v-u

        int_sub(&U, &V, &t);

        // Make sure the resulting value is positive.
        if(t.hw[0] & MSB_HW) {
            flag = -1;
            int_neg(&t);
        }
        else flag = 1;

        check = 0;

        INTLOOP(i) check |= t.hw[i];
    }

    // Once finished, apply extracted common multiples of 2 (stored in k) back into the value

    int_copy(&U, w);

    while(k > HALFSIZE) {
        for(i = 0; i<INTMAX; i++)w->hw[i] = w->hw[i+1];
        k-= HALFSIZE;
        w->hw[INTMAX] = 0;
    }
    carry_bit = 0;
    while(k>0) {
        INTLOOP(i) {
            w->hw[i] = (w->hw[i] << 1) | carry_bit;
            carry_bit = w->hw[i] & CARRY ? 1: 0;
            w->hw[i] &= LOMASK;
        }
        k--;

    }

}

// Calculates x ^ n mod q into z
void mod_exp(BIGINT *x, BIGINT *n, BIGINT *q, BIGINT *z) {
    BIGINT N, Y, Z, temp, dummy;
    ELEMENT check;
    INDEX i;


    // Y will be 1 initially
    int_copy(n, &N);
    int_null(&Y);
    Y.hw[INTMAX] = 1;
    int_copy(x, &Z);

    // Checking for 0
    check = 0 ;
    INTLOOP(i) check |= N.hw[i];

    // Loop invariant Y * Z ^ N == x ^ n mod q
    // Initially, Y = 1, Z = x, N = n, thus true at loop start
    while(check) {

        // (Y*Z)^N == x^n mod q

        // If odd
        if(N.hw[INTMAX] & 1) {
            // Y * Z * Z ^ (N-1) == x ^ n mod q

            // Update Y with Y*Z mod q

            int_mul(&Y, &Z, &temp);
            int_div(&temp, q, &dummy, &Y);


            // Y * Z ^ (N-1) == x ^ n mod q
        }


        // Divide N by two, as N-1 / 2 == N/2 (integer division)
        // Y * (Z ^ 2) ^ (N/2) == x ^ n mod q
        int_div2(&N);
        int_mul(&Z, &Z, &temp);
        int_div(&temp, q, &dummy, &Z);
        // Y * Z ^ N == x ^ n mod q      ====> Invariant

        // Finally, update loop condition
        check = 0;
        INTLOOP(i) check |= N.hw[i];
    }
    int_copy(&Y, z);
}

// Finds the inverse of a mod b and puts it into x
void mod_inv(BIGINT *a, BIGINT *b, BIGINT *x) {
    BIGINT m,n,p0,p1,p2,q,r,temp,dummy;
    ELEMENT check;
    INDEX sw, i;

    sw = 1;
    int_copy(b, &m);
    int_copy(b, &m);
    int_null(&p0);
    p0.hw[INTMAX] = 1;
    int_div(&m, &n, &p1, &r);
    int_copy(&p1, &q);

    check = 0;
    INTLOOP(i) check |= r.hw[i];
    while(check) {
        sw = -sw;
        int_copy(&n, &m);
        int_copy(&r, &n);
        int_div(&m, &n, &q, &r);
        int_mul(&q, &p1, &temp);
        int_add(&temp, &p0, &temp);
        int_div(&temp, b, &dummy, &p2);
        int_copy(&p1, &p0);
        int_copy(&p2, &p1);
        check = 0;
        INTLOOP(i) check |= r.hw[i];
    }

    if(sw < 0) int_sub(b, &p0, x);
    else int_copy(&p0,x);
}

int main() {
    BIGINT a;
    BIGINT b;
    BIGINT c;
    int_null(&a);

    int_null(&b);
    int_null(&c);
    ascii_to_bigint("7", &b);
    ascii_to_bigint("5", &a);
    mod_inv(&a,&b,&c);
    char *buffer[MAXSTRING];
    bigint_to_ascii(&c,buffer);

    printf("%s\n", buffer);

    debug_print_binary('b'&0xf);
    printf("%d\n", sizeof(unsigned long) * 8);
    return 0;
}

