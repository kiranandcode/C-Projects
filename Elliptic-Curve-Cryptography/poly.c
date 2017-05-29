#include "poly.h"
#include "field2n.h"

/**
 *         Gopiandcode - C Code
 * ------------------------------------
 *  This code was created by gopia on 29/05/2017.
 *  
 *  Enjoyed? Check out my github for more: https://github.com/Gopiandcode
**/

void poly_add(FIELD2N *a, FIELD2N *b, FIELD2N *c){
    INDEX i;
    SUMLOOP(i) c->e[i] = a->e[i] ^ b->e[i];
}

// Simple shifts all bits in a DBLFIELD along one
void poly_shift(DBLFIELD *a) {
    ELEMENT *eptr, temp, bit;
    INDEX i;
    eptr = &a->e[DBLWORD];
    bit = 0;
    DBLLOOP(i) {
        // Shift the value - if bit from prior section, or it onto the result.
        temp = (*eptr << 1) | bit;

        // Check if a carry occurs, remember for next iteration.
        bit = (*eptr & MSB) ? 1L : 0L;

        // Insert into location, then iterate
        *eptr-- = temp;
    }
}

// Clears out all the bits
void null(FIELD2N *a) {
    INDEX i;
    SUMLOOP(i) a->e[i] = 0L;

}

void dblnull(DBLFIELD *a) {
    INDEX i;
    DBLLOOP(i) a->e[i] = 0L;

}

// Copies a field2 variable
void copy(FIELD2N *from, FIELD2N *to) {
    INDEX i;

    SUMLOOP(i) to->e[i] = from->e[i];
}


void sngltodbl(FIELD2N *from, DBLFIELD *to) {
    INDEX  i;

    dblnull(to);

    // DBLFIELDS are of size DBLWORD, due to big endian formatting, positions [.....xxxxxx], thus when
    // copying over data, we start from DBLWORD-NUMWORD
    SUMLOOP(i) to->e[i] = from->e[DBLWORD-NUMWORD+i];

}


void poly_mul_partial(FIELD2N *a,FIELD2N *b,DBLFIELD *c) {
    INDEX i,bit_count,word;
    ELEMENT mask;
    DBLFIELD B;

    dblnull(c);

    sngltodbl(b, &B);

    mask = 1;
    for(bit_count = 0; bit_count < NUMBITS; bit_count++) {
        word = NUMWORD - bit_count/WORDSIZE;

        // for each bit in a, if bit is 1, then add c to accumulating sum
        if(mask & a->e[word]) {
            DBLLOOP(i) c->e[i] ^= B.e[i];
        }

        // As we're iterating through the bits from most significant to least (due to internal big endian config)
        // After considering each bit, shift the accumulating sum one - this is simmilar to constructing an int from
        // a decimal string
        poly_shift(&B);

        // Consider the next bit along
        mask <<= 1;

        // Once the bit reaches the end, return to start, as next word has been reached
        if(!mask) mask = 1;

    }

}