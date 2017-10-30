#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "classifier.h"
#include "bits/pattern.h"
#include "core/random.h"
#include "collections/list.h"
#include "collections/set.h"

struct classifier_element_B {
	pattern_B pattern;
	bitstring_B result;
	unsigned char output;
	unsigned int freq;
	int life;
	double fitness;
};

struct classifier_B {
	unsigned int bitstring_size;
	bitstring_B zeroref;
	list_L elements;
	list_L message_board;
//	struct classifier_element_B *elements;
	bitstring_B output;
};


struct message_B {
	bitstring_B string;
	list_L activators;	
};

struct output_B {
	struct classifier_element_B *pattern;
	list_L activators;
};

struct classifier_element_B *classifier_elementrandom(unsigned int size) {
	struct classifier_element_B *elem;
	elem = malloc(sizeof(*elem));
	elem->result = bitstring_random(size);
	elem->pattern = pattern_random(size);
	elem->fitness = random_range(5,20);
	elem->life = elem->fitness;
	elem->output = random_range(0,1) > 0.5 ? 1 : 0;
	elem->freq = 0;
	return elem;
}

void classifier_elementdelete(struct classifier_element_B *elem) {
	bitstring_delete(elem->result);
	pattern_delete(elem->pattern);
	free(elem);
	return;
}

struct classifier_element_B *classifier_elementcrossover(struct classifier_element_B *elemA, struct classifier_element_B *elemB) {
	assert(bitstring_get_bitlength(elemA->result) == bitstring_get_bitlength(elemB->result));

	struct classifier_element_B *elem;
	elem = malloc(sizeof(*elem));

	elem->result = bitstring_crossover(elemA->result, elemB->result, random_range(0, bitstring_get_bitlength(elemA->result)));
	elem->pattern = pattern_crossover(elemA->pattern, elemB->pattern);
	elem->fitness = random_range((elemA->fitness < elemB->fitness ? elemA->fitness : elemB->fitness),(elemA->fitness > elemB->fitness ? elemA->fitness : elemB->fitness));
	elem->life = elem->fitness;
	elem->output = (unsigned int) (elemB->output + elemA->output)/2;
	elem->freq = 0;
	return elem;
}


void classifier_elementprint(struct classifier_element_B *elem) {
	pattern_print(elem->pattern);
	printf(" -> ");
	bitstring_print(elem->result);
	printf(" - %s (%-3.3lf, %5i) [%5i]", elem->output ? "OUTPUT  " : "FEEDBACK", elem->fitness, elem->life, elem->freq);
}

void classifier_print(struct classifier_B *classifier) {
	printf("MESSAGE BOARD:\n");
	struct L_iterator iter = list_iterator(classifier->message_board);
	while(list_iteratorhasnext(&iter)) {
		printf("\t");
		bitstring_print(((struct message_B *)list_iteratornext(&iter))->string);
		printf("\n");
	}
	iter = list_iterator(classifier->elements);
	printf("PATTERNS:\n");
	while(list_iteratorhasnext(&iter)) {
		printf("\t");
		classifier_elementprint(list_iteratornext(&iter));
		printf("\n");
	}

	printf("OUTPUT: ");
	if(classifier->output != NULL) {
		bitstring_print(classifier->output);
	}
	else {
		printf("NULL");
	}
	printf("\n");
}

struct classifier_B *classifier_new(unsigned int size, unsigned int count) {
	struct classifier_B *result;
	result = malloc(sizeof(*result));
	result->message_board = list_new();
	result->elements = list_new();
	result->bitstring_size = size;
	result->zeroref = bitstring_new(size);
	result->output = NULL;
	
	unsigned int i;
	for(i = 0; i < count; ++i) {
		list_append(result->elements, classifier_elementrandom(size));
	}

	return result;
}



struct output_B *classifier_outputnew(struct classifier_element_B *pattern, list_L activators) {
	struct output_B *msg;

	msg = malloc(sizeof(*msg));
	assert(msg);
	msg->pattern = pattern;
	msg->activators = activators == NULL ? list_new() : list_copy(activators, NULL);

	return msg;
}



void classifier_outputdelete(struct output_B *msg) {
	list_delete(msg->activators, NULL);
	free(msg);
	return;
}

void classifier_printoutput(classifier_B classifier) {
	//printf("OUTPUT:\n");
	//printf("\t");
	if(classifier->output == NULL)
		printf("NULL");
	else
		bitstring_print(classifier->output);
	//printf("\n");
}


struct message_B *classifier_messagenew(bitstring_B string, list_L activators) {
	struct message_B *msg;

	msg = malloc(sizeof(*msg));
	assert(msg);
	msg->string = bitstring_copy(string);
	msg->activators = activators == NULL ? list_new() : list_copy(activators, NULL);

	return msg;
}


void classifier_messagedelete(struct message_B *msg) {
	bitstring_delete(msg->string);
	list_delete(msg->activators, NULL);
	free(msg);
	return;
}

// input : 10111111111111
// result: 1####1####0###
//         ----------e---
//                   error
//
// string: 10000100010010
//   mask: 10000100001000 
// result: 1####1####0###
//
// inp&st: 10000100010010
//
// inp^st: 00111011101101
//   mask: 10000100001000 
//in^st&m: 00000000001000
//
void classifier_messageboard_insert(classifier_B classifier, bitstring_B string, list_L activators);
void classifier_outputlist_insert(list_L outputlist, struct classifier_element_B *pattern, list_L activators);

void classifier_messageboard_feedthrough(classifier_B classifier, list_L outputlist) {
	double i = 1.01;
	while(list_length(classifier->message_board) > 0) {
//		printf("Message_board length %d\n", list_length(classifier->message_board));
		struct message_B *msg = list_pop(classifier->message_board);
		struct L_iterator patterns = list_iterator(classifier->elements);
		while(list_iteratorhasnext(&patterns)) {
			struct classifier_element_B *pattern = list_iteratornext(&patterns);
			if(pattern_matches(pattern->pattern, msg->string, classifier->zeroref)) {
				pattern->life -= 1; //pattern_strength(pattern->pattern)/bitstring_get_bitlength(pattern->output);
				if(pattern->output) {
					printf("Inserting into outputlist\n");
					classifier_outputlist_insert(outputlist, pattern, msg->activators);
				}
				else {
					pattern->freq++;
					pattern->fitness *= i;
					i *= i;
					classifier_messageboard_insert(classifier, pattern->result, msg->activators);
				}
			}
		}
		classifier_messagedelete(msg);
	}


}

void classifier_resetpopulationlife(classifier_B classifier) {
    struct L_iterator patterns = list_iterator(classifier->elements);
    while(list_iteratorhasnext(&patterns)) {
			struct classifier_element_B *pattern = list_iteratornext(&patterns);
        	pattern->life = pattern->fitness;
    }
}

void classifier_coverinput(classifier_B classifier, bitstring_B input, bitstring_B expected) {
		struct classifier_element_B *elem;
		elem = malloc(sizeof(*elem));

		elem->result = bitstring_random(bitstring_get_bitlength(input));
		elem->pattern = pattern_cover(input);
		elem->fitness = random_range(5,20);
		elem->life = elem->fitness*10;
		elem->output = random_range(0,1) > 0.5 ? 1 : 0;
		elem->freq = 1;

		struct L_iterator iter = list_iterator(classifier->elements);
		while(list_iteratorhasnext(&iter)) {
			struct classifier_element_B *elem = list_iteratornext(&iter);

			elem->fitness *= 0.97;
		}


		list_append(classifier->elements, elem);
		if(classifier->output != NULL) {
			bitstring_delete(classifier->output);
		}
		classifier->output = bitstring_copy(elem->result);
		if(!elem->output) {
			if(expected != NULL) {
				classifier_supervise(classifier, input, expected);
			}
			else
				classifier_input(classifier, input);
		}

}





void classifier_update(classifier_B classifier, bitstring_B input) {
	list_L outputlist = list_new();
	// while msg_board is not empty
	// 	msg m = msg_board.pop()
	// 	
	// 	for pattern in patterns:
	// 		if pattern.match(m)
	// 			if pattern is output:
	//	 			outputlist.add(pattern.string)
	//	 		if pattern is input:
	//		 		 msgboard_insert(pattern.string, pattern);
	//
	//assert(msg_board is empty)
	//
	//if outputlist.empty():
	//	patterns.add(new rule {
	//		pattern: new pattern(input)
	//		output: random()
	//		})
	//	classifier.output = output
	//else:
	//	int max_fit = outputlist[0].fit()
	//	winner = outputlist[0]
	//
	//	for output in outputlist:
	//		if output.fit > max_fit:
	//			winner = output
	//			max_fit = output.fit
	//
	//	classifier.output = output;
	classifier_messageboard_feedthrough(classifier, outputlist);


	if(list_length(outputlist) == 0) {
		classifier_coverinput(classifier, input, NULL);
	}
	else {
		struct output_B *best = list_get(outputlist, 0);
		float max_fit = best->pattern->fitness;

		struct L_iterator outputlistiter = list_iterator(outputlist);

		while(list_iteratorhasnext(&outputlistiter)) {
			struct output_B *output = list_iteratornext(&outputlistiter);
			if(output->pattern->fitness > max_fit) {
				best = output;
				max_fit = output->pattern->fitness;
			}
		}

		if(classifier->output != NULL) {
			bitstring_delete(classifier->output);
		}
		classifier->output = bitstring_copy(best->pattern->result);

		best->pattern->fitness *= 1.05;
		best->pattern->freq++;

		outputlistiter = list_iterator(outputlist);

		while(list_iteratorhasnext(&outputlistiter)) {
			struct output_B *output = list_iteratornext(&outputlistiter);
			struct L_iterator iter = list_iterator(output->activators);
			if(output != best) {
				output->pattern->fitness *= 0.99;
				while(list_iteratorhasnext(&iter)) {
					struct classifier_element_B *elem = list_iteratornext(&iter);
					elem->fitness *= 0.99;
					
				}
			} else {
				while(list_iteratorhasnext(&iter)) {
					struct classifier_element_B *elem = list_iteratornext(&iter);
					elem->fitness *= 1.05;
				}
			}
			classifier_outputdelete(output);
		}

	}

	classifier_resetpopulationlife(classifier);
    


	list_delete(outputlist, NULL);
}


void classifier_supervise(classifier_B classifier, bitstring_B input, bitstring_B expected) {
	list_L outputlist = list_new();

	classifier_messageboard_insert(classifier, input, NULL);

	classifier_messageboard_feedthrough(classifier, outputlist);

	if(list_length(outputlist) == 0) {

//		printf("Covering input\n");
		classifier_coverinput(classifier, input, expected);
	}
	else {
		struct output_B *best = list_get(outputlist, 0);
		double max_fit = bitstring_simmilarity(best->pattern->result, expected);

		struct L_iterator outputlistiter = list_iterator(outputlist);

		while(list_iteratorhasnext(&outputlistiter)) {
			struct output_B *output = list_iteratornext(&outputlistiter);
			double max_len = bitstring_get_bitlength(output->pattern->result);
			double simmilarity = bitstring_simmilarity(output->pattern->result, expected);

			// value between 0 and 1 representing how close the output is - the closer to 1, the better
			double closeness = simmilarity/(double)max_len;

			// value between -0.5 and 0.5 representing how close the output is
			double modifier = closeness;

			unsigned int label = bitstring_asbuckets(expected, 10);
			unsigned int actual = bitstring_asbuckets(output->pattern->result, 10);
			double goodness = (double)(label-(double)actual);
			goodness = (goodness < 0 ? -1 *goodness : goodness);
			// at this point goodness is absolute difference between 0 and 10

			goodness = goodness/10;
			// at this point goodness is between 0 and 1
			
			goodness = (1-goodness);
			// at this point goodness is between 1 and 0 where 1 is close and 0 is not close

			goodness = (goodness < 0.01 && goodness > 0 ? 0.01 : goodness);

			if(label == actual) {
				goodness = 0.1;
			}
			else
				goodness = 1/goodness;

			modifier = modifier < 0 ? (modifier > 0.001 ? 0.001 : modifier) : (modifier < 0.001 ? 0.001 : modifier);

			printf("fitness(%lf -> ", output->pattern->fitness);
			output->pattern->fitness -= (1-modifier)*goodness;
			printf("%lf)\n", output->pattern->fitness);

			printf("label (%d) - actual(%d) - delta(%lf)\n", label, actual, -1*(1-modifier)*goodness);

			if(simmilarity > max_fit) {
				best = output;
				max_fit = simmilarity;
			}
		}

		if(classifier->output != NULL) {
			bitstring_delete(classifier->output);
		}
		classifier->output = bitstring_copy(best->pattern->result);

		best->pattern->fitness *= 1.0005;
		best->pattern->freq++;

		outputlistiter = list_iterator(outputlist);

		while(list_iteratorhasnext(&outputlistiter)) {
			struct output_B *output = list_iteratornext(&outputlistiter);
			struct L_iterator iter = list_iterator(output->activators);
			if(output != best) {
				output->pattern->fitness *= 0.999;
				while(list_iteratorhasnext(&iter)) {
					struct classifier_element_B *elem = list_iteratornext(&iter);
					elem->fitness *= 0.999;
					
				}
			} else {
				while(list_iteratorhasnext(&iter)) {
					struct classifier_element_B *elem = list_iteratornext(&iter);
					elem->fitness *= 1.05;
				}
			}
			classifier_outputdelete(output);
		}

	}

	classifier_resetpopulationlife(classifier);

	list_delete(outputlist, NULL);
}



void classifier_evolve(classifier_B classifer, unsigned int clipping_size, unsigned int mutation_count, double mutation_probability) {
	// while size > clipping_size, eliminate randomly
	// then for 1/4 the population size, mutate based on frequency
	struct L_iterator elements_iter = list_iterator(classifer->elements);
	double total = 0;
	double avg = 0;
	unsigned int total_freq = 0;
	unsigned int count = 0;

	while(list_iteratorhasnext(&elements_iter)) {
		struct classifier_element_B *elem = list_iteratornext(&elements_iter);
		total += (double)elem->fitness;
		total_freq += elem->freq;
		count++;
	}
	avg = total/count;


	//printf("Population avg fitness %lf\n", avg);
	//printf("Population total fitness %lf\n", total);
	//printf("Population total freq %u\n", total_freq);
	//printf("Avg freq %lf\n", (double)total_freq/count);

	// preelimination
	unsigned int iterations = 0;
	while(list_length(classifer->elements) > clipping_size*2 && iterations < 1000) {
		//iterations++;
		double low_fitness = -1;
		struct classifier_element_B *lowest = NULL;
			
		unsigned int i;
		struct L_iterator iter = list_iterator(classifer->elements);
		while(list_iteratorhasnext(&iter)) {
			struct classifier_element_B *elem = list_iteratornext(&iter);
			if(lowest == NULL) {
				lowest = elem;
				low_fitness = elem->fitness;
			}


			if(elem->fitness < low_fitness) {
				low_fitness = elem->fitness;
				lowest = elem;
			}
		}


		if(lowest != NULL) {
			list_remove(classifer->elements, lowest);
			classifier_elementdelete(lowest);
		}
	}
	iterations = 0;
	while(list_length(classifer->elements) > clipping_size && iterations < 10000) {
		//iterations++;
		double elimination_cutoff = random_range(0, total);	
		double current = 0;

		struct L_iterator iter = list_iterator(classifer->elements);

		struct classifier_element_B *elemA = NULL;
		struct classifier_element_B *elemB = NULL;

		while(list_iteratorhasnext(&iter) && current < elimination_cutoff) {
			elemA = list_iteratornext(&iter);
			if(elemA != NULL){
				current += elemA->fitness;
			if(elemA->fitness > avg*1.05) break;
			}
		//	if(elemA->fitness < 0.001) break;
		}

		list_remove(classifer->elements, elemA);
		elimination_cutoff = random_range(0, total);	
		current = 0;
		iter = list_iterator(classifer->elements);

		while(list_iteratorhasnext(&iter) && current < elimination_cutoff) {
			elemB = list_iteratornext(&iter);
            if(elemB != NULL) {
			current += elemB->fitness;

//			if(elemB->fitness > avg) break;
	    }
		//	if(elemB->fitness < 0.001) break;
		}
		list_remove(classifer->elements, elemB);
		struct classifier_element_B *elemC = classifier_elementcrossover(elemA, elemB);
		list_append(classifer->elements, elemC);
		classifier_elementdelete(elemA);
		classifier_elementdelete(elemB);
	}

	unsigned int i;

	for(i = 0; i < clipping_size/4; ++i){
		double freq_val = random_range(0, total_freq/2 > 0 ? total_freq/2  : total_freq);
		double current = 0;
		struct L_iterator iter = list_iterator(classifer->elements);
		struct classifier_element_B *elemA = NULL;

		while(list_iteratorhasnext(&iter) && current < freq_val) {
			elemA = list_iteratornext(&iter);
			if(elemA != NULL) {
				current += elemA->freq;
				if(elemA->freq == 0) break;
				if(elemA->fitness < 0.001) break;
			}
		}
		if(elemA != NULL) {
			elemA->output = random_range(0, 1) > 0.5 + (elemA->output ? 0.1 : -0.1) ? 0 : 1;
			bitstring_mutate(elemA->result, mutation_count, mutation_probability);
			pattern_mutate(elemA->pattern, mutation_count, mutation_probability);
		}
	}
	
}


void classifier_messageboard_insert(classifier_B classifier, bitstring_B string, list_L activators) {
	// for msg_B in message_board:
	// 	if msg_B.str = string
	// 		msg_B.activators.add(activator)
	// 		return
	//
	//insert into message_board:
	//	new msg_B {
	//		str: string
	//		activators: {activator}
	//		}
	//
	struct L_iterator iter = list_iterator(classifier->message_board);
	while(list_iteratorhasnext(&iter)) {
		struct message_B *msg = list_iteratornext(&iter);
		if(bitstring_eq(msg->string, string)) {
			if(activators != NULL)
				list_concat(msg->activators, activators, NULL);
			return;
		}
	}
	struct message_B *msg = classifier_messagenew(string, activators);
	list_append(classifier->message_board, msg);
	return;

}

void classifier_outputlist_insert(list_L outputlist, struct classifier_element_B *pattern, list_L activators) {
	// for msg_B in message_board:
	// 	if msg_B.str = string
	// 		msg_B.activators.add(activator)
	// 		return
	//
	//insert into message_board:
	//	new msg_B {
	//		str: string
	//		activators: {activator}
	//		}
	//
	struct L_iterator iter = list_iterator(outputlist);
	while(list_iteratorhasnext(&iter)) {
		struct output_B *msg = list_iteratornext(&iter);
		if(msg->pattern == pattern) {
			list_concat(msg->activators, activators, NULL);
			return;
		}
	}
	struct output_B *msg = classifier_outputnew(pattern, activators);
	list_append(outputlist, msg);
	return;

}



void classifier_input(classifier_B classifier, bitstring_B input) {

	assert(classifier->bitstring_size == bitstring_get_bitlength(input));
	
	//list_append(classifier->message_board, bitstring_copy(input));
	classifier_messageboard_insert(classifier, input, NULL);

	classifier_update(classifier, input);
}


bitstring_B classifier_getoutput(classifier_B classifier) {
	return classifier->output;
}
