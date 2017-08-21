#include "neuron.h"
#include <stdio.h>


int main() {

	printf("Hello neurons!\n");

	neuron_N neural_net = neuron_new_uniform(3,3,3);

	neuron_print(neural_net);


}
