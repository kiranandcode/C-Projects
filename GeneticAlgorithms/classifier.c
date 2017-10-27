#include <assert.h>
#include <stdlib.h>
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
	while(list_length(classifier->message_board) > 0) {
		struct message_B *msg = list_pop(classifier->message_board);
		struct L_iterator patterns = list_iterator(classifier->elements);
		while(list_iteratorhasnext(&patterns)) {
			struct classifier_element_B *pattern = list_iteratornext(&patterns);
			if(pattern_matches(pattern->pattern, msg->string, classifier->zeroref) && pattern->life > 0) {
				pattern->life -= pattern_strength(pattern->pattern);
				if(pattern->output) {
					classifier_outputlist_insert(outputlist, pattern, msg->activators);
				}
				else {
					pattern->freq++;
					classifier_messageboard_insert(classifier, pattern->result, msg->activators);
				}
			}
		}
		classifier_messagedelete(msg);
	}

	if(list_length(outputlist) == 0) {
		struct classifier_element_B *elem;
		elem = malloc(sizeof(*elem));

		elem->result = bitstring_random(bitstring_get_bitlength(input));
		elem->pattern = pattern_cover(input);
		elem->fitness = random_range(0,20);
		elem->life = elem->fitness;
		elem->output = random_range(0,1) > 0.5 ? 1 : 0;
		elem->freq = 1;

		list_append(classifier->elements, elem);

		if(classifier->output != NULL) {
			bitstring_delete(classifier->output);
		}
		classifier->output = bitstring_copy(elem->result);
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

		best->pattern->fitness *= 1.5;
		best->pattern->freq++;

		outputlistiter = list_iterator(outputlist);

		while(list_iteratorhasnext(&outputlistiter)) {
			struct output_B *output = list_iteratornext(&outputlistiter);
			struct L_iterator iter = list_iterator(output->activators);
			if(output != best) {
				output->pattern->fitness *= 0.9;
				while(list_iteratorhasnext(&iter)) {
					struct classifier_element_B *elem = list_iteratornext(&iter);
					elem->fitness *= 0.9;
					
				}
			} else {
				while(list_iteratorhasnext(&iter)) {
					struct classifier_element_B *elem = list_iteratornext(&iter);
					elem->fitness *= 1.1;
				}
			}
			classifier_outputdelete(output);
		}

	}

	list_delete(outputlist, NULL);
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
	
	list_append(classifier->message_board, bitstring_copy(input));

	classifier_update(classifier, input);
}


