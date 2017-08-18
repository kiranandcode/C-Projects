#include "neuron.h"
#include <math.h>

#define N neuron_N 

struct neuron_N {
	UINT layers;		
	matrix_G *weights;
};


INT sigmoid(INT val) {
	INT result = 1 / (1 + exp(-1 * val));
	return result;
}

matrix_G neuron_feedforward(N neural_net, matrix_G inp) {
	UINT i;


	matrix_G inputs = matrix_copy(inp);
	matrix_G result;
	for(i = 0; i< neural_net->layers; ++i) {
		result = matrix_mult(neural_net->weights[i],inputs);
		matrix_delete(inputs);
		inputs = matrix_map(result, sigmoid);
		matrix_delete(result);
	}
	return inputs;
}

void neuron_feedforward_acc(N neural_net, matrix_G inp, matrix_G *outputs) {
	UINT i;


	matrix_G inputs = matrix_copy(inp);
	matrix_G result;
	for(i = 0; i< neural_net->layers; ++i) {
		result = matrix_mult(neural_net->weights[i],inputs);
		matrix_delete(inputs);
		inputs = matrix_map(result, sigmoid);
		outputs[i] = matrix_copy(inputs);
		matrix_delete(result);
	}
	matrix_delete(inputs);
}


void neuron_feedbackward(N neural_net, matrix_G err, matrix_G *errors){

	UINT i;
	matrix_G error = matrix_copy(err);
	matrix_G result;

	for(i = neural_net->layers-1; i >= 0 && i < neural_net->layers; i--) {

		result = matrix_transpose(neural_net->weights[i]);

		errors[i] = matrix_mult(result, error);

		matrix_delete(result);
		matrix_delete(error);

		error = matrix_copy(errors[i]);
	}

	matrix_delete(error);

	return;
}
