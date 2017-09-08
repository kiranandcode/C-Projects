#ifndef WAV_H
#define WAV_H

#include <stdio.h>
#include <inttypes.h>

void wav_create(uint8_t *bytes, uint32_t count, FILE *fp);

#endif
