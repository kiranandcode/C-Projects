#include "string.h"
#include "alloc.h"
#include <limits.h>

#define PERM 0

// ------------------------------------------------------
// data 
// ------------------------------------------------------

// hash table of strings
// using str allows for strings with null bytes
static struct string {
	char *str;
	int len;
	struct string *link;
} *buckets[1024];


static unsigned int scatter[] = {7743,7998,2121,9554,617,97,2795,6564,6832,6516,6408,8141,2097,1849,4587,9854,3695,9263,2653,2765,7358,5718,6215,9171,1212,7620,1145,4018,3383,6478,5299,9106,9161,2598,4845,1381,287,1317,9285,7291,1037,2581,8411,5638,5751,8327,785,2489,9183,801,4397,9528,9733,5004,3712,8584,5375,6028,3336,7698,953,7122,4222,9279,2690,3676,4659,4229,5422,5183,3764,1553,6201,8513,5497,9647,286,2102,520,3897,2700,8021,9918,6108,9768,9503,3488,3139,8831,1597,914,25,3901,8470,2339,3881,3682,9151,2466,8631,6437,4150,4884,603,9082,8643,9811,4527,708,8050,5188,603,7808,4924,2342,1990,9845,6781,4546,9105,8245,3493,7071,7967,5780,1930,4957,6079,9634,1893,9923,1138,7697,7071,3343,4001,633,9783,6198,2345,4531,5360,421,8117,3489,1712,3202,5591,1757,5504,8273,9565,883,9247,3080,1312,1033,6506,8354,1855,8815,1808,9232,5925,5879,8617,3254,9711,8675,2704,6460,944,8545,9966,820,3427,806,6944,1024,9823,849,6937,4525,560,3983,6418,9386,2797,1975,260,846,6560,5059,1953,9099,2749,8069,1297,9727,8537,1670,773,8733,7775,4582,1436,5280,4519,7828,9091,9320,9524,4437,6906,8717,9627,8727,3586,5798,5266,3733,4995,9882,5855,8602,184,339,6107,7085,4400,3121,3397,4731,6042,625,7135,6039,1115,2380,784,8924,8614,8641,8703,5520,4587,9360,374,840,3450,5014,8540,8978,3377,1331,6782};

// ------------------------------------------------------
// functions 
// ------------------------------------------------------

// returns a copy of a string
char *string(str) char *str; {
	char *s;

	for(s = str; *s; s++);

	return stringn(str, s-str);
}


// converts an integer to a string representation
char *stringd(n) int n; {
	char str[25], *s = str + sizeof(str);
	unsigned m;

	if(n == INT_MIN)
		m = (unsigned)INT_MAX + 1;
	else if(n < 0)
		m = -n;
	else
		m = n;
	do
		*--s = m%10 + '0';
	while ((m /= 10) != 0);
	if(n < 0)
		*--s = '-';
	return stringn(s, str + sizeof(str) - s);
}


char *stringn(str, len) char *str; int len;{
	int i;
	unsigned int h;
	char *end;
	struct string *p;

	// calculate hashcode for string
	//  hash done using a random mapping
	for(h = 0, i = len, end = str; i > 0; i--)
		h = (h << 1) + scatter[*(unsigned char *)end++];
	// equivalent of a modulo, but faster.
	//  at the expense of non prime hash table
	h &= NELEMS(buckets)-1;
	
	for(p =  buckets[h]; p; p = p->link)
		if(len == p->len) {
			char *s1 = str, *s2 = p->str;
			do {
				if(s1 == end)
					return p->str;
			} while(*s1++ == *s2++);
		}
	// if we reach this point, the string was not found in the table
	// install the string
	{
		// strings are stored in chunks of perm memory
		static char *next, *strlimit;
		// if memory exceeded allocate another 4k
		if(next + len + 1 >= strlimit) {
			int n = len + 4*1024;
			next = allocate(n, PERM);
			strlimit = next + n;
		}
		NEW(p, PERM);
		p->len = len;
		// copy over ths string
		for(p->str = next; str < end; )
			*next++ = *str++;
		// also add a null terminator
		*next++ = 0;
		p->link = buckets[h];
		buckets[h] = p;
		return p->str;
	
	}

}
