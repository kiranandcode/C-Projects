#include "input.h"

#include <stdlib.h>
#include <stdio.h>

static int bsize;
//           used for tail, used for the line
//          <----------->|<------------------------------->
//             MAXLINE                 BUFSIZE
// buffer = [............|________________________________]
static unsigned char buffer[MAXLINE+1 + BUFSIZE+1];

void inputInit() {
	limit = cp = &buffer[MAXLINE+1];
	bsize = -1;
	lineno = 0;
	file =NULL;
	//refill buffer
	
	nextline();
}

void nextline() {
	do {
		// if reached end of buffer
		if(cp >= limit) {
			//refill buffer
			fillbuf();
			// if cp still == limit then eof
			if(cp == limit)
				return;
		} else
			// increment line
			lineno++;
		// skip whitespace
		for(line = (char *)cp; *cp==' ' || *cp == '\t'; cp++)
			;
// while the current character is '\n' and cp == limit
// i.e until !(*cp == '\n' && cp == limit)
// demorgans!!!
// (*cp != '\n' || cp != limit)
// i.e until the current character is not newline or cp != limit
	} while(*cp == '\n' && cp == limit);
}


void fillbuf() {
	// if eof return
	if(bsize == 0)
		return;
	// if reached end of buffer, move cp to beginning of buffer
	if(cp >= limit)
		cp = &buffer[MAXLINE+1];
	else {
		// move the tail portion
		// calculate the remaining chars
		int n = limit - c;

		unsigned char *s =  &buffer[MAXLINE+1]-n;
		// we offset line, so that cp - line gives the 
		// correct length
		// line = s - (position of cp)
		line = (char *)s - ((char *)cp - line);
		while(cp < limit)
			*s++ = *cp++;
		cp = &buffer[MAXLINE+1] - n;
	}
	// read from file into buffer
	bsize = read(infd, &buffer[MAXLINE+1], BUFSIZE);
	if(bsize < 0) {
		error("read error\n");
		exit(1);
	}
	limit = &buffer[MAXLINE+1+bsize];
	*limit='\n';
}
