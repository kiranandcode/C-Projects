#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <ctype.h>
#include <math.h>
#include "assert.h"
#include "../CH4/except.h"
#include "fmt.h"
#include "../CH5/mem.h"
#define T Fmt_T

/* Types */
struct buf {
	char *buf;
	char *bp;
	int size;
};

/* Macros */

/* conversion functions */

/* data */
const Except_T Fmt_Overflow = { "Formatting Overflow" };

static T cvt[256] = {
	/*  0- 7  */    0,    0, 0,    0,    0,    0,    0,    0,
	/*  8- 15 */    0,    0, 0,    0,    0,    0,    0,    0,
	/* 16- 23 */    0,    0, 0,    0,    0,    0,    0,    0,
	/* 24- 31 */    0,    0, 0,    0,    0,    0,    0,    0,
	/* 32- 39 */    0,    0, 0,    0,    0,    0,    0,    0,
	/* 40- 47 */    0,    0, 0,    0,    0,    0,    0,    0,
	/* 48- 55 */    0,    0, 0,    0,    0,    0,    0,    0,
	/* 56- 63 */    0,    0, 0,    0,    0,    0,    0,    0,
	/* 64- 71 */    0,    0, 0,    0,    0,    0,    0,    0,
	/* 72- 79 */    0,    0, 0,    0,    0,    0,    0,    0,
	/* 80- 87 */    0,    0, 0,    0,    0,    0,    0,    0,
	/* 88- 95 */    0,    0, 0,    0,    0,    0,    0,    0,
	/* 96- 103*/    0,    0, 0,cvt_c,cvt_d,cvt_f,cvt_f,cvt_f,
	/*104- 111*/    0,    0, 0,    0,    0,    0,    0,cvt_o,
	/*112- 119*/    cvt_p,0, 0,cvt_s,    0,cvt_u,    0,    0,
	/*120- 127*/    cvt_x,0, 0,    0,    0,    0,    0,    0
};



/* static functions */
static int outc(int c, void *cl) {
	FILE *f = cl;
	return putc(c, f);
}

	// private function for inserting into a buffer
static int insert(int c, void *cl) {
	struct buf *p = cl;

	// if the bufferpointer is greater than (or equal) to the size of the buffer raise an error
	if(p -> bp >= p->buf + p->size)
		RAISE(Fmt_Overflow);

	// insert the character
	*p->bp++ = c;


	return c;
}

	// Private function for increasing the size of a buffer 
static int append(int c, void *cl) {
	struct buf *p = cl;

	// if there is space
	if(p->bp >= p->buf + p->size) {
		// double the size of the buffer
		RESIZE(p->buf, 2 * p->size);

		// then change the buffer attributes to match 
		p->bp = p->buf + p->size;
		p->size *= 2;
	}
	*p->bp++ = c;
	return c;
}


/* functions */

// still not a 1005 sure why this extra level of abstraction is used.
// - edit: 
// 	worked it out - vfmt is a common function used by multiple many arg functions.
// 	to abstract out the general code from any indivdiual mutliarg function, vfmt is used
void Fmt_fmt(int put(int c, void *), void *cl,
		const char *fmt, ...) {
	va_list ap;
	
	// Start iterating through the variable args
	va_start(ap, fmt);
	Fmt_vfmt(put, cl, fmt, ap);
	va_end(ap);
}

void Fmt_print(const char *fmt, ...) {
	va_list ap;

	va_start(ap, fmt);

	Fmt_vfmt(outc, stdout, fmt, ap);

	va_end(ap);
}

int Fmt_vsfmt(char *buf, int size, const char *fmt, va_list ap) {
	// internal buffer object keeps track of buffer position
	struct buf cl;

	// First we check that everything is present in the function
	assert(buf);
	assert(size > 0);
	assert(fmt);

	
	// set up the buffer object to point to the buffer and start at 0.
	cl.buf = cl.bp = buf;
	cl.size = size;

	// run vfmt on the buffer object and format it
	Fmt_vfmt(insert, &cl, fmt, ap);
	
	// inserts '\0' at the end of the object
	insert(0, &cl);          // <-- Private insert function keeps track of whether the character can be inserted
	return cl.bp - cl.buf-1;
}


char *Fmt_string(const char *fmt, ...) {
	char *str;
	va_list ap;

	assert(fmt);
	va_start(ap, fmt);
	str = Fmt_vsstring(fmt, ap);
	va_end(ap);
	return str;
}


char *Fmt_vstring(const char *fmt, va_list ap) {
	struct buf cl;

	assert(fmt);

	cl.size = 256;
	cl.buf = cl.bp = ALLOC(cl.size);
	Fmt_vfmt(append, &cl, fmt, ap);
	append(0, &cl);
	return RESIZE(cl.buf, cl.bp - cl.buf);
}


void Fmt_vfmt(int put(int c, void *cl), void *cl, const char *fmt, va_list ap) {
	assert(put);
	assert(fmt);
	while(*fmt)
		if(*fmt != '%' || *++fmt == '%')
			put((unsigned char)*fmt++, cl);
		else {
			unsigned char c, flags[256];
			int width = INT_MIN, precision = INT_MIN;
			memset(flags, '\0', sizeof flags);

			// get the optional flags
			

			// get optional field width
			

			// get optional precision
			

			c = *fmt++;
			assert(cvt[c]);
			(*cvt[c])(c, &ap, put, cl, flags, width, precision);
		}
}
