#include <stdio.h>
#include <stdlib.h>


int dotProduct(int[], int[], int);




int dotProduct(int x[], int y[], int n) {
	int i, sum;
	
	sum = 0;
	for(i = 0; i<n; i++){
		sum += x[i] * y[i];
	}
	return sum;
}



int main() {
	int x[] = {1,2,3,4};
	int y[] = {1,2,3,4};
	printf("x dot y = %d\n", dotProduct(x,y,4));

}
