#ifndef DEFLATE_H
#define DEFLATE_H

#include "bitstream.h"

#define B bitstream_B

void deflate_insert(B stream, unsigned int length);


#undef B
#endif
