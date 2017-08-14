#define q 33554393
#define d 32
#include <string.h>
#include <stdio.h>

int r_index(char c) {
	if(c == '0')
		return 0;
	else return (c - 'a') + 1;
}

/* returns the index of the string matching the pattern*/
int rksearch(char *p, char *a) {

	int i, dM = 1, h1 = 0, h2 = 0;
	int M = strlen(p), N = strlen(a);
	// Calculate d^(M-1) mod q and store in dM
	for(i = 1; i < M; i++) dM = (d*dM) % q;

	for(i = 0; i < M; i++) {
		// h1 contains the hash for p1
		//  the hashing function is a polynomial in terms of d, where each coefficient is the char, mod q
		h1 = (h1 * d + r_index(p[i])) % q;

		// h2 contains the hash for p2
		//  the hashing function is a polynomial in terms of d, where each coefficient is the char, mod q
		h2 = (h2 * d + r_index(a[i])) % q;
	}

	// while the hash for the string is not equal
	for(i = 0; h1 != h2;i++) {
		// this is the genius part.
		// recalculate the hash quickly, by rather than recalculating the hash, we calculate the polynomial by,
		// 
		// old_hash = a[i]*dM-1 + a[i+1]*dM-2 ..... a[i+M-1]
		//
		// new hash = old_hash + d*q - a[i]*d^(M-1)  mod q
		//
		// as d*q == 0 mod q if q is a prime (we add it to prevent underflow)
		//
		// new_hash = a[i]*dM-1 + a[i+1]*dM-1 ...a[i+M-1] - a[i]*d^(M-1)
		//
		// new_hash = a[i+1]*dM-2 + a[i+2]*dM-3 .....a[i+M-1]
		h2 = (h2 + d * q - r_index(a[i])*dM) %q;
		

		// then we multiply by d, and add i+M
		//
		// new_hash = new_hash * d + a[i+M] mod q
		//
		// thus new hash is
		//
		// new_hash = a[i+1]*dM-1 + a[i+2]*dM-2 + .... + a[i+M]
		//
		// which is what we would expect from new_hash
		h2 = (h2 * d + r_index(a[i+M])) % q;
		if(i > N-M) return N;
	}
	return i;

}




int main() {

	char long_string_buf[999];
	char short_string_buf[999];
	printf("%40s\n", "Rabin Karp Pattern Matching Demo");
	printf("----------------------------------------\n");
	printf("To begin, please enter a long string.\nAs long as you would like(max 999 chars):\n");
	scanf("%999[^\n] ",long_string_buf);

	int long_string_len = strlen(long_string_buf);
	int short_string_len = 0;

	do{
		printf("Please enter a short string.\nPlease make it shorter than the long string:\n");
		scanf("%999[^\n]", short_string_buf);
		short_string_len = strlen(short_string_buf);
	
		if(short_string_len > long_string_len) {
			printf("The short string MUST be smaller than the long one.\n");
		}

	} while(long_string_len < short_string_len && short_string_len == 0);
	


	int search_result = rksearch(short_string_buf, long_string_buf);

	if(search_result == long_string_len) printf("Your string was not found in the text.\n");

	else {
		printf("Your string was found in the long text.\n");
		printf("The string was found at index %d of the long text.\n", search_result);
		if(search_result > 5 && search_result < long_string_len - short_string_len - 5) {
			char pre_buf[5];
			sprintf(pre_buf, "%%%ds", short_string_len + (10 > long_string_len - short_string_len ? long_string_len - short_string_len : 10));
			printf(pre_buf, long_string_buf + search_result - 5);
		} else {
			char pre_buf[5];
			sprintf(pre_buf, "%%%ds", short_string_len);
			printf(pre_buf, long_string_buf + search_result);
		}
	}
}
