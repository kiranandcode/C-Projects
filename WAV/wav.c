#include "wav.h"

#include "bitstream.h"

#include <inttypes.h>
#include <assert.h>

// littleendian: 0111 0111 1011 0111
// bigendian   : 1110 1101 1110 1110
// thus to insert littleendian a multiple bytes,
// (a >> 24) & 0xFF
// (a >> 16) & 0xFF
// (a >> 8) & 0xFF
// (a) & 0xFF

void wav_insert_descriptor(bitstream_B stream, uint32_t chunk_size) {
	assert(stream);

	// insert chunkid RIFF
	bitstream_big_endian_insert(stream, 'R',8);
	bitstream_big_endian_insert(stream, 'I',8);
	bitstream_big_endian_insert(stream, 'F',8);
	bitstream_big_endian_insert(stream, 'F',8);


	// insert littleendian chunck size
	bitstream_insert(stream, (chunk_size) & 0xFF, 8);
	bitstream_insert(stream, (chunk_size>> 8) & 0xFF, 8);
	bitstream_insert(stream, (chunk_size>> 16) & 0xFF, 8);
	bitstream_insert(stream, (chunk_size>> 24) & 0xFF, 8);

	// insert format wave
	bitstream_big_endian_insert(stream, 'W', 8);
	bitstream_big_endian_insert(stream, 'A', 8);
	bitstream_big_endian_insert(stream, 'V', 8);
	bitstream_big_endian_insert(stream, 'E', 8);

	return;
}

