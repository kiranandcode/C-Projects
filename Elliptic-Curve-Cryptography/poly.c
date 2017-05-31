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


void dbltosngl(DBLFIELD *from, FIELD2N *to) {
    INDEX  i;

    // Copy the lower bits into the output
    SUMLOOP(i) to->e[i] = from->e[DBLWORD-NUMWORD + i];

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

void div_shift(DBLFIELD *a) {
// shifts to the right
    ELEMENT  *eptr, temp, bit;
    INDEX i;
    // Pointer to the elements in the array
    eptr = (ELEMENT*) &a->e[0];

    bit = 0;
    DBLLOOP(i) {
        // Get the value shifted back one + the bit from prior iterations
        temp = (*eptr >> 1) | bit;
        // If before shifting, there was a bit in the first position, set bit to MSB for the next iteration
        bit = (*eptr & 1) ? MSB : 0L;

        // Finally, assign new value to location and shift the reference along one.
        *eptr++ = temp;
    }

}

// Finds the msb in a word x
INDEX log_2(ELEMENT x) {
    ELEMENT ebit, bitsave, bitmask;
    INDEX k, lg2;

    lg2 = 0;
    bitsave = x;
    k = WORDSIZE/2;
    // Recently learned that -1 returns a binary representation of all 1s, thus shifting left by k ignores the lower bits
    bitmask = -1 <<k;

    while(k) {
        // Are there any bits in the upper half?
        ebit = bitsave & bitmask;

        if(ebit) {
            // If yes, increment the position by k, (the binary search is now starting from its original postion + k
            lg2 += k;
            // Remove the lower bits from the mask, no point keeping them around
            bitsave = ebit;
        }

        // Binary search, so decrement the scale by 1/2
        k/=2;

        // Very nice section of code, understandable using the following illustration
        // Initial bit mask      =           111111111111111000000000000000000
        // New     bit mask      =                     11111111111110000000000
        //                                                  kkkkkkkk
        //                                               difference of size k,
        //          xored mask   =                          111111110000000000
        // decreases the mask by k.
        bitmask ^= (bitmask >> k);
    }
    // So this loop essentially zeroes in on the msb - the first if produces an every increasing lower bound by eliminating values below the mask
    //                                                 the second one produces a decreasing upper bound by repeatedly shifting the mask to a smaller space


    return (lg2);
}


// Finds the msb of a ELEMENT array - compatible with all Bigint and DoubleNum implementations provided correct dim given
INDEX degreeof(ELEMENT *t, INDEX dim) {
    INDEX degree, k;
    ELEMENT ebit, bitsave, bitmask;

    degree = dim * WORDSIZE;

    // Starting from the highest bit, decrement downwards until a non-zero element is found
    for(k = 0; k<dim; k++) {
        if(*t) break;
        degree -= WORDSIZE;
        t++;
    }
    // If the entire sequence is empty, return -1
    if(!*t) return (-1);

    // Finally, find the specific bit using the prior binary search algorithm.
    degree += log_2(*t);

    // return;
    return  degree;
}

void poly_div(DBLFIELD *top, FIELD2N *bottom, FIELD2N *quotient, FIELD2N *remainder) {
    INDEX deg_top;
    INDEX deg_bottom;
    INDEX deg_quot;
    INDEX bit_count;
    INDEX i;
    INDEX equot;
    ELEMENT topbit, *tptr;
    DBLFIELD shift;



    // Find the MSB of both polynomials, if numerator has a lower msb that denominator, then division returns 0
    deg_top = degreeof(top, DBLWORD);
    deg_bottom = degreeof(bottom, NUMWORD);


    null(quotient);

    if(deg_top < deg_bottom) {
        dbltosngl(top,remainder);
        return;
    }

    // shift the bottom to align with msb of numerator - as in hand schoolbook multiplication

}