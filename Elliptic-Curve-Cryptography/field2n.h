/**
 *         Gopiandcode - C Code
 * ------------------------------------
 *  This code was created by gopia on 29/05/2017.
 *  
 *  Enjoyed? Check out my github for more: https://github.com/Gopiandcode
**/

#ifndef PLAYGROUND_FIELD2N_H
#define PLAYGROUND_FIELD2N_H

#define WORDSIZE (sizeof(long) * 8)

#define NUMBITS   93

#define NUMWORD   (NUMBITS/WORDSIZE

#define UPRSHIFT  (NUMWORD % WORDSIZE)

#define MAXLONG   (NUMWORD+1)

#define MAXBITS   (MAXLONG * WORDSIZE

#define MAXSHIFT  (WORDSIZE - 1)

#define MSB       (1L << MAXSHIFT)

#define UPRBIT    (1L << (UPRSHIFT-1))

#define UPRMASK   (~(-1l<<UPRSHIFT))

#define SUMLOOP(i) for(i = 0; i<MAXLONG; i++)

typedef short int INDEX;

typedef unsigned long ELEMENT;

typedef struct {
        ELEMENT e[MAXLONG];
} FIELD2N;

#endif //PLAYGROUND_FIELD2N_H
