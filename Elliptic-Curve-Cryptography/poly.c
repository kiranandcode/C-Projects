#include "poly.h"
#include "field2n.h"

/**
 *         Gopiandcode - C Code
 * ------------------------------------
 *  This code was created by gopia on 29/05/2017.
 *  
 *  Enjoyed? Check out my github for more: https://github.com/Gopiandcode
**/


FIELD2N poly_prime = {
        0x08000000, 0x0000000, 0x00000000, 0x00000000, 0x000000B1 //155
};

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
    deg_quot = deg_top - deg_bottom;
    bit_count = deg_quot + 1;
    // 1 1 1 1 0 0 0 0
    // | | | | | | | |
    // 0 0 0 0 0 1 1 1

    // The difference between the msb is 5
    // Copy denominator into shift
    sngltodbl(bottom, &shift);
    // shift to align with numerator
    for(i = 0; i<deg_quot; i++) {
        poly_shift(&shift);
    }

    // Bitmask to check msb of top
    topbit = 1L << (deg_top % WORDSIZE);

    // tptr points to the topmost element - equiv to (top + 1 - deg_top)/WORDSIZE
    // Take top, points to the msb, shift it to the end, and then subtract the number of words in the bits in the MSB,
    tptr = (ELEMENT *) top + DBLWORD - deg_top/WORDSIZE;


    while(bit_count) {
        // Is the bit in the topmost element 1
        if(*tptr & topbit) {
            // Then just add shift to subtract - in base 2 + == -
            DBLLOOP(i)
                top->e[i] ^= shift.e[i];

            // Update equot to indicate that division occurred at this power
            equot = NUMWORD - deg_quot/WORDSIZE;
            quotient->e[equot] |= 1L << (deg_quot % WORDSIZE);
        }

        // Consider the next bit along.
        bit_count--;
        deg_quot--;
        div_shift(&shift);
        topbit >>= 1;

        // Once you reach the end of one element, jump back to considering the MSB, but of the previous element
        if(!topbit) {
            topbit = MSB;
            tptr++;
        }

    }
}

// Multiplication modulo a prime.
void poly_mul(FIELD2N *a, FIELD2N *b, FIELD2N *c) {
    DBLFIELD temp;
    FIELD2N dummy;

    poly_mul_partial(a, b, &temp);
    poly_div(&temp, &poly_prime, &dummy, c);
}


// Finds the inverse of a mod the poly prime, into inverse
void poly_inv(FIELD2N *a, FIELD2N *inverse) {
    FIELD2N pk, pk1, pk2;
    FIELD2N rk, rk1;
    FIELD2N qk, qk1, qk2;

    INDEX i;
    DBLFIELD rk2;

    // rk2 will contain the base
    sngltodbl(&poly_prime, &rk2);
    // rk1 will contain the initial value

    // Clear all varialbes and set pk1 and qk1 to 1 and pk
    copy(a, &rk1);
    null(&pk2);
    null(&pk1);
    pk1.e[NUMWORD] = 1L;
    null(&qk2);
    null(&qk1);
    qk1.e[NUMWORD] = 1L;
    null(&pk);
    pk.e[NUMWORD] = 1L;

    // divide poly_prime by a and place quotient and remainder into qk, rk
    poly_div(&rk2, &rk1, &qk, &rk);

    // While the remainder is not 0
    while(degreeof(&rk, NUMWORD) >= 0) {

        // Multiply the quotient by pk1 (initailly 1, and place into rk2);
        poly_mul_partial(&qk, &pk1, &rk2);

        // then subtract a from quotient multiplied by pk1
        SUMLOOP(i) pk.e[i] = rk2.e[i+DBLWORD-NUMWORD] ^ pk2.e[i];

        sngltodbl(&rk1, &rk2);
        copy(&rk, &rk1);
        copy(&qk1, &qk2);
        copy(&qk, &qk1);
        copy(&pk1, &pk2);
        copy(&pk, &pk1);

        poly_div(&rk2, &rk1, &qk, &rk);
    }

    // Finally copy over the result.
    copy(&pk, inverse);
}



void poly_gcd(FIELD2N *u, FIELD2N *v, FIELD2N *gcd) {
    DBLFIELD top;
    FIELD2N r, dummy, temp;

    // Gotta use a double for the division functionalities
    sngltodbl(u, &top);
    copy(v, &r);

    // Simple gcd algorithm, keep on swapping the remainder with the divisor...
    while(degreeof(&r, NUMWORD) >= 0) {
        poly_div(&top, &r, &dummy, &temp);
        sngltodbl(&r, &top);
        copy(&temp, &r);
    }

    // Finally copy it over.
    dbltosngl(&top, gcd);
}

// operation to multiply a polynomial in x by x mod a polynomial base

void mul_x_mod(FIELD2N *u, FIELD2N *v, FIELD2N *w) {
    DBLFIELD mulx;
    INDEX  i, deg_v;

    deg_v = degreeof(v, NUMWORD);

    // Put u into mul x
    sngltodbl(u, &mulx);

    // Multiply the u polynomial by x - in this representation this is done by shifting left - every coefficient goes along one.
    poly_shift( &mulx);

    // Put it back into a normal field.
    dbltosngl(&mulx, w);

    // If the degree of the polynomial is equal to v, subtract v.
    if(w->e[NUMWORD - (deg_v/WORDSIZE)] & (1L << (deg_v % WORDSIZE)))
        SUMLOOP(i) w->e[i] ^= v->e[i];

}

INDEX irreducible(FIELD2N *v) {
    FIELD2N  vprm, gcd, x2r, x2rx, temp;
    FIELD2N  sqr_x[NUMBITS+1];
    INDEX    i, r, deg_v, k;


    SUMLOOP(i) vprm.e[i] = (v->e[i] >> 1) & DERIVMASK;
    poly_gcd(v, &vprm, &gcd);
    if(gcd.e[NUMWORD] > 1) return 0;

    for(i = 0; i<NUMWORD; i++) if(gcd.e[i]) return 0;

    deg_v = degreeof(v, NUMWORD);

    null(&sqr_x[0]);
    sqr_x[0].e[NUMWORD] = 1;
    for(i = 1; i<= deg_v; i++) {
        mul_x_mod(&sqr_x[i-1], v, &temp);
        mul_x_mod(&temp, v, &sqr_x[i]);
    }

    null(&x2r);
    x2r.e[NUMWORD] = 2;

    for(r = 1; r<=deg_v/2; r++) {
        null(&x2rx);
        for(i = 0; i<=deg_v; i++) {
            if(x2r.e[NUMWORD - (i/WORDSIZE)] & (1L << (i%WORDSIZE)))
                SUMLOOP(k) x2rx.e[k] ^= sqr_x[i].e[k];
        }

        copy(&x2rx, &x2r);
        x2rx.e[NUMWORD] ^= 2;

        poly_gcd(&x2rx, v, &gcd);
        if(gcd.e[NUMWORD] > 1) return 0;
        for(i = 0; i<NUMWORD; i++)
            if(gcd.e[i]) return 0;
    }

    return 1;

}