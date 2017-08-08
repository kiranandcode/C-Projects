#include <stdio.h>
#include <stdlib.h>

void print_binary(int binary) {

	int i = 1;

	while(i <= binary) i*=2;



	while(i > 1) {
		if(binary % i == 0) putc('0', stdout);
		else putc('1', stdout);
		i /= 2;
	}

}


int main() {
	int value;

	while(scanf("%d", &value)) {

		print_binary(value);
		printf("\n");

	}
}
