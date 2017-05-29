/**
 *         Gopiandcode - C Code
 * ------------------------------------
 *  This code was created by gopia on 29/05/2017.
 *  
 *  Enjoyed? Check out my github for more: https://github.com/Gopiandcode
**/

#ifndef PLAYGROUND_POLY_H
#define PLAYGROUND_POLY_H

#include "field2n.h"

#define DBLBITS    2*NUMBITS

#define DBLWORD    (DBLBITS/WORDSIZE)

#define DBLSHIFT   (DBLBITS%WORDSIZE)

#define MAXDBL     (DBLWORD+1)

#define DERIVMASK  (0x55555555)

#define DBLLOOP(i) for(i = 0; i<MAXDLB; i++)

typedef struct {
    ELEMENT e[MAXDBL];
} DBLFIELD;

#endif //PLAYGROUND_POLY_H
