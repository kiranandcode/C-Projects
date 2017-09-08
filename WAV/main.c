#include "wav.h"
#include <math.h>

int main() {

	uint32_t count = 10000;
	uint8_t items[count];
	uint32_t i = 0;

	for(i = 0; i < count; i++) {
		items[i] = (100) * sin(13 * i) + (200) * cos(3*i);
	}
	FILE *fp = fopen("music.wav", "wb");


	wav_create(items, count, fp);

	fclose(fp);
}
