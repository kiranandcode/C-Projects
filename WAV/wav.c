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


void wav_insert_subchunk_descriptor(bitstream_B stream,
		uint16_t numchannels,
		uint32_t samplerate,
		uint16_t bitspersample
		) {

	// insert subchunk1id big 4 bytes
	bitstream_big_endian_insert(stream, 'f',8);
	bitstream_big_endian_insert(stream, 'm',8);
	bitstream_big_endian_insert(stream, 't',8);
	bitstream_big_endian_insert(stream, ' ',8);
	// insert subchunk1size little 4 bytes

	// 16 for pcm
	bitstream_insert(stream, ((uint32_t)(16)) & 0xFF, 8);
	bitstream_insert(stream, ((uint32_t)(16)>> 8) & 0xFF, 8);
	bitstream_insert(stream, ((uint32_t)(16)>> 16) & 0xFF, 8);
	bitstream_insert(stream, ((uint32_t)(16)>> 24) & 0xFF, 8);
	
	// insert audioformat  little 2 bytes
	// 1 for pcm, other than 1 means compression
	bitstream_insert(stream, ((uint16_t)(1)) & 0xFF, 8);
	bitstream_insert(stream, ((uint16_t)(1)>> 8) & 0xFF, 8);

	// insert numchannels  little 2 bytes
	bitstream_insert(stream, ((uint16_t)(numchannels)) & 0xFF, 8);
	bitstream_insert(stream, ((uint16_t)(numchannels)>> 8) & 0xFF, 8);
	// insert samplerate little 4 bytes
	bitstream_insert(stream, ((uint32_t)(samplerate)) & 0xFF, 8);
	bitstream_insert(stream, ((uint32_t)(samplerate)>> 8) & 0xFF, 8);
	bitstream_insert(stream, ((uint32_t)(samplerate)>> 16) & 0xFF, 8);
	bitstream_insert(stream, ((uint32_t)(samplerate)>> 24) & 0xFF, 8);
	// insert byterate little 4 bytes
	// byterate = samplerate * numchannels * bitspersample/8
	uint32_t byterate =
		samplerate * numchannels * bitspersample/8;
	bitstream_insert(stream, ((uint32_t)(byterate)) & 0xFF, 8);
	bitstream_insert(stream, ((uint32_t)(byterate)>> 8) & 0xFF, 8);
	bitstream_insert(stream, ((uint32_t)(byterate)>> 16) & 0xFF, 8);
	bitstream_insert(stream, ((uint32_t)(byterate)>> 24) & 0xFF, 8);
	// insert blockalign little 4 bytes
	// blockalign = numchannels * bitspersample/8
	uint32_t blockalign =
		numchannels * bitspersample/8;

	bitstream_insert(stream, ((uint32_t)(blockalign)) & 0xFF, 8);
	bitstream_insert(stream, ((uint32_t)(blockalign)>> 8) & 0xFF, 8);
	bitstream_insert(stream, ((uint32_t)(blockalign)>> 16) & 0xFF, 8);
	bitstream_insert(stream, ((uint32_t)(blockalign)>> 24) & 0xFF, 8);
	// insert bitspersample 2 bytes
	
	bitstream_insert(stream, ((uint16_t)(bitspersample)) & 0xFF, 8);
	bitstream_insert(stream, ((uint16_t)(bitspersample)>> 8) & 0xFF, 8);
	return;
}


void wav_insert_data_byte(bitstream_B stream,
		uint32_t subchunk_bytes,
		unsigned char *data) {

	// subchunk2id big 4 bytes 
	bitstream_big_endian_insert(stream, 'd', 8);
	bitstream_big_endian_insert(stream, 'a', 8);
	bitstream_big_endian_insert(stream, 't', 8);
	bitstream_big_endian_insert(stream, 'a', 8);


	// subchunk 2 size little 4 bytes
	// subchunk2size = numsamples * numchannels * bitspersample/8

	bitstream_insert(stream, ((uint32_t)(subchunk_bytes)) & 0xFF, 8);
	bitstream_insert(stream, ((uint32_t)(subchunk_bytes)>> 8) & 0xFF, 8);
	bitstream_insert(stream, ((uint32_t)(subchunk_bytes)>> 16) & 0xFF, 8);
	bitstream_insert(stream, ((uint32_t)(subchunk_bytes)>> 24) & 0xFF, 8);


	uint32_t i = 0;
	for(i = 0; i < subchunk_bytes; i++) {
	bitstream_insert(stream, data[i], 8);
	}
	return;
}
