#include "simplega.h"

#include "bitstring.h"
#include "random.h"

#include <stdio.h>

#define f(x,a,b) ((x-a == 0) || (x+b == 0) ? 100000000 : (100/((x-a)*(x+b))))

double fitness(bitstring_B stream) {
// define fitness by a mathematical function
	uint16_t i = bitstring_int(stream);
      return f(((double)i), 5.0, 6.0);
//
// define fitness by proximity to a bitstring
//unsigned int score = 0;
// search for string 0*1*****001*0101
//                   0000101100101101
//                     !         !  
//(!bitstring_bittest(stream, 0)) ? score++ : score;
//(bitstring_bittest(stream, 1)) ? score++ : score;
//(bitstring_bittest(stream, 2)) ? score++ : score;
//(bitstring_bittest(stream, 3)) ? score++ : score;
//(bitstring_bittest(stream, 4)) ? score++ : score;
//(bitstring_bittest(stream, 5)) ? score++ : score;
//(bitstring_bittest(stream, 6)) ? score++ : score;
//(bitstring_bittest(stream, 7)) ? score++ : score;
//(!bitstring_bittest(stream, 8)) ? score++ : score;
//(!bitstring_bittest(stream, 9)) ? score++ : score;
//(bitstring_bittest(stream, 10)) ? score++ : score;
//(bitstring_bittest(stream, 11)) ? score++ : score;
//(!bitstring_bittest(stream, 12)) ? score++ : score;
//(bitstring_bittest(stream, 13)) ? score++ : score;
//(!bitstring_bittest(stream, 14)) ? score++ : score;
//(bitstring_bittest(stream, 15)) ? score++ : score;

//return score;
}

bitstring_B haploid_genetic_algorithm(unsigned int population_size, unsigned int individual_size, unsigned int iterations, double fitness(bitstring_B string),int print_result) {
	bitstring_B population[population_size];
	bitstring_B temporary[population_size];
	bitstring_B best_individual = NULL;
	double      population_fitness[population_size];

	// generate population
	unsigned int i;
	for(i = 0; i < population_size; ++i) {
		population[i] = bitstring_random(individual_size);
	}
	
	int k;
	double overall_best_seen;
	for(k = 0; (unsigned int)k < iterations; k++) {
		population_fitness[0] = fitness(population[0]);	
		double best_seen = 0;
		uint16_t best_seen_value = 0;

		// culmative population fitness
		for(i = 1; i < population_size; ++i) {
			double current_fitness = fitness(population[i]);
			population_fitness[i] = current_fitness + population_fitness[i-1];	
			if(current_fitness > best_seen) {
				best_seen = current_fitness;
				best_seen_value = bitstring_int(population[i]);
				if(current_fitness > overall_best_seen) {
					if(best_individual != NULL)
						bitstring_delete(best_individual);
					best_individual = bitstring_copy(population[i]);
					overall_best_seen = current_fitness;
				}
			}
		}
		double best_fitness = population_fitness[population_size-1];

		if(print_result) {
			printf("best seen fitness = %lf\n", best_seen);
			printf("best seen decoded integer = %u\n", best_seen_value);
		}


		if(best_fitness > 0) {
			for(i = 0; i < population_size; ++i) {
				double value = random_range(0, best_fitness);
				int parentA = 0, parentB = 0;

				while(population_fitness[parentA] < value && (unsigned int)parentA < population_size) parentA++;
				value = random_range(0, best_fitness);
				while(population_fitness[parentB] < value && (unsigned int)parentB < population_size) parentB++;
				//printf("parentA = %d, parentB = %d\n", parentA, parentB);
				temporary[i] = bitstring_crossover(population[parentA],population[parentB], (unsigned int) random_range(0, individual_size));
				if(random_range(0,1) > 0.9){
				       
					unsigned int position = random_range(0,individual_size);
					if(bitstring_bittest(temporary[i], position))
						bitstring_bitclear(temporary[i], position);
					else
						bitstring_bitset(temporary[i],position); 
					
				}
			}
		} else {
			for(i = 0; i < population_size; ++i) {
				temporary[i] = bitstring_random(individual_size);
			}
		}

		for(i = 0; i < population_size; ++i) {
			bitstring_delete(population[i]);
			population[i] = temporary[i];
		}
		
		if(print_result) {
			printf("Current population is\n");
			for(i = 0; i< population_size; i++) {
				printf("[%d]: ", i);
				bitstring_print(population[i]);
				printf("\n");
			}
		}
	}

	for(i = 0; i < population_size; ++i) {
		bitstring_delete(population[i]);
	}

	return best_individual;
}


int main() {
	uint16_t val = bitstring_int(haploid_genetic_algorithm(50, 16, 20000, fitness, 0));
	printf("result %u\n", val);
}
