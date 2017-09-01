#include <stdio.h>
#include "bitstring.h"
#include "random.h"
#define f(x) ((x-7 == 0) || (x+3 == 0) ? 100000000 : (100/((x-20)*(x+3))))

double fitness(bitstring_B stream) {
	uint16_t i = bitstring_int(stream);
	return f(((double)i));
}

int main() {
	bitstring_B population[10];
	bitstring_B temporary[10];
	double      population_fitness[10];

	unsigned int i;
	for(i = 0; i < 10; ++i) {
		population[i] = bitstring_random(16);
	}
	
	int k;
	for(k = 0; k < 2000; k++) {
		population_fitness[0] = fitness(population[0]);	
		double best_seen = 0;
		uint16_t best_seen_value = 0;
		for(i = 1; i < 10; ++i) {
			double current_fitness = fitness(population[i]);
			population_fitness[i] = current_fitness + population_fitness[i-1];	
			if(current_fitness > best_seen) {
				best_seen = current_fitness;
				best_seen_value = bitstring_int(population[i]);
			}
		}
		double best_fitness = population_fitness[9];
		printf("best seen = %lf\n", best_seen);
		printf("best seen value = %u\n", best_seen_value);


		if(best_fitness > 0) {
		for(i = 0; i < 10; ++i) {
			double value = random_range(0, best_fitness);
			int parentA = 0, parentB = 0;

			while(population_fitness[parentA] < value && parentA < 10) parentA++;
			value = random_range(0, best_fitness);
			while(population_fitness[parentB] < value && parentB < 10) parentB++;
			//printf("parentA = %d, parentB = %d\n", parentA, parentB);
			temporary[i] = bitstring_crossover(population[parentA],population[parentB], (unsigned int) random_range(0, 16));
			if(random_range(0,1) > 0.9){
			       
				unsigned int position = random_range(0,16);
				if(bitstring_bittest(temporary[i], position))
					bitstring_bitclear(temporary[i], position);
				else
					bitstring_bitset(temporary[i],position); 
				
			}
		}
		} else {
			for(i = 0; i < 10; ++i) {
				temporary[i] = bitstring_random(16);
			}
		}

		for(i = 0; i < 10; ++i) {
			bitstring_delete(population[i]);
			population[i] = temporary[i];
		}
		
		printf("Current population is\n");
		for(i = 0; i< 10; i++) {
			printf("[%d]: ", i);
			bitstring_print(population[i]);
			printf("\n");
		}
	}

}
