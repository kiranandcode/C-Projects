#include "neuron.h"

#include <math.h>
#include <stdlib.h>

#define N neuron_N 
#define G matrix_G

struct neuron_N {
	UINT layers;		
	matrix_G *weights;
};

N neuron_new_uniform(UINT layers, UINT nodes) {
	N neuron = malloc(sizeof(*neuron));
	

	return neuron;
}

INT sigmoid(INT val) {
	INT result = 1 / (1 + exp(-1 * val));
	return result;
}

INT oneminus(INT val) {
	return 1.0 - val;
}

G neuron_apply(N neural_net, matrix_G inp) {
	UINT i;


	G inputs = matrix_copy(inp);
	G result;
	for(i = 0; i< neural_net->layers; ++i) {
		result = matrix_mult(neural_net->weights[i],inputs);
		matrix_delete(inputs);
		inputs = matrix_map(result, sigmoid);
		matrix_delete(result);
	}
	return inputs;
}

void neuron_feedforward(N neural_net, G inp, G *outputs) {
	UINT i;


	outputs[0] = matrix_copy(inp);
	G inputs = matrix_copy(inp);
	G result;
	for(i = 0; i< neural_net->layers; ++i) {
		result = matrix_mult(neural_net->weights[i],inputs);
		outputs[i+1] = matrix_copy(result);
		matrix_delete(inputs);
		inputs = matrix_map(result, sigmoid);
		matrix_delete(result);
	}
	matrix_delete(inputs);
}


void neuron_feedbackward(N neural_net, G err, G *errors){

	UINT i;
	G error = matrix_copy(err);
	G result;

	for(i = neural_net->layers-1; i >= 0 && i < neural_net->layers; --i) {

		result = matrix_transpose(neural_net->weights[i]);

		errors[i] = matrix_mult(result, error);

		matrix_delete(result);
		matrix_delete(error);

		error = matrix_copy(errors[i]);
	}

	matrix_delete(error);

	return;
}


void neuron_train(N neural_net, G *err, G *inp, INT alpha) {

	UINT i;
	for(i = 0; i< neural_net->layers; ++i){

/*
 *  Delta Weights = -1 * error * sigmoid(input+1) * (1 - sigmoid(input+1) * input^transport
 *
 * neg_err = mat_scalar_mult(-alpha, error)
 *
 * sigmoid = mat_map(input[i+1],sigmoid);
 * minus_sigmoid = mat_scalar_mult(-1,sigmoid);
 * one_m_sigmoid = mat_scalar_add(minus_sigmoid,1)
 *
 * sigmoid_result - mat_mult(sigmoid, one_m_sigmoid)
 *
 * prev_transpose = mat_transpose(input[i])
 *
 * sigmultprev = mat_mult(sigmoid_result, prev_transpose)
 *
 * delta_wjk = mat_mult(neg_err, sigmultprev);
 *
 *
 *
 */


		G neg_alpha_err = matrix_scalar_mult(err[i], -alpha);
		G sigmoid_inp = matrix_map(inp[i+1],sigmoid);
		G one_minus_sigmoid = matrix_map(sigmoid_inp, oneminus);
		G sigmoid_result = matrix_mult(sigmoid_inp, one_minus_sigmoid);	

		matrix_delete(one_minus_sigmoid);
		matrix_delete(sigmoid_inp);

		G prev_transpose = matrix_transpose(inp[i]);

		G sigmoid_mult_prev = matrix_mult(sigmoid_result, prev_transpose);	

		matrix_delete(prev_transpose);
		matrix_delete(sigmoid_result);


		G delta_wjk = matrix_mult(neg_alpha_err, sigmoid_mult_prev);

		matrix_delete(sigmoid_mult_prev);
		matrix_delete(neg_alpha_err);

		G new_weights = matrix_add(neural_net->weights[i], delta_wjk);
		matrix_delete(delta_wjk);

		matrix_delete(neural_net->weights[i]);
		neural_net->weights[i] = new_weights;

	}
}
