#include "neuron.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define N neuron_N 
#define G matrix_G

struct neuron_N {
	UINT layers;		
	matrix_G *weights;
};

static INT sigmoid(INT val);
static INT normal_random(INT val);

N neuron_new_uniform(UINT input_size,
		UINT hidden_size,
		UINT output_size) {
	N neural_net = malloc(sizeof(*neural_net));
	neural_net->layers = 2;

	neural_net->weights = malloc(sizeof(*neural_net->weights)*neural_net->layers);

	G new = matrix_new(hidden_size, input_size);
		
	G random = matrix_map(new, normal_random);

	matrix_delete(new);

	neural_net->weights[0] = random;

	
	new = matrix_new(output_size, hidden_size);

	random = matrix_map(new, normal_random);

	matrix_delete(new);

	neural_net->weights[1] = random;

	return neural_net;
}

static INT sigmoid(INT val) {
	INT result = 1 / (1 + exp(-1 * val));
	return result;
}

static INT normal_random(INT val) {
	// generates random values,
	// using marsaglia method
	static INT prev = -1.0;
	INT x,y,s,f;

	if(prev != -1.0){
		x = prev;
		prev = -1.0;
		return x;
	}

	do {
		x = (2.0*rand()/RAND_MAX) - 1.0;
		y = (2.0*rand()/RAND_MAX) - 1.0;
		s = x*x + y*y;
	} while(s >= 1 || s == 0);


	f = sqrt(-2.0 * log(s)/s);
	x = x*f;
	y = y*f;

	x = ((x)/5.0)*0.99 + 0.01;
	y = ((y)/5.0)*0.99 + 0.01;

	prev = y;
	return x;
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
		matrix_delete(inputs);
		inputs = matrix_map(result, sigmoid);
		matrix_delete(result);
		outputs[i+1] = matrix_copy(inputs);
	}
	matrix_delete(inputs);
}


void neuron_feedbackward(N neural_net, G err, G *errors){

	UINT i;
	G error = matrix_copy(err);
	G result;

	errors[neural_net->layers] = matrix_copy(err);
	for(i = neural_net->layers-1; i >= 0 && i < neural_net->layers; --i) {
		printf("Feedbackward Iteration[%ld]:\n", neural_net->layers - i);

		result = matrix_transpose(neural_net->weights[i]);

		errors[i] = matrix_mult(result, error);

//		printf("Deleting result[%ld,%ld]\n",
//			matrix_row_get(result),
//			matrix_col_get(result));
		matrix_delete(result);

//		printf("Deleting error[%ld,%ld]\n",
//			matrix_row_get(error),
//			matrix_col_get(error));

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


		
//		printf("neg_alpha_err calculated\n");
//		printf("matrix_dimensions(%ld,%ld)\n",
//				matrix_row_get(neg_alpha_err),
//				matrix_col_get(neg_alpha_err));
		

//		printf("sigmoid_inp_calculated\n");
//		printf("matrix_dimensions(%ld,%ld)\n",
//				matrix_row_get(sigmoid_inp),
//				matrix_col_get(sigmoid_inp));
		G one_minus_sigmoid = matrix_map(inp[i+1], oneminus);


//		printf("one_minus_sigmoid_calculated\n");
//		printf("matrix_dimensions(%ld,%ld)\n",
//				matrix_row_get(one_minus_sigmoid),
//				matrix_col_get(one_minus_sigmoid));

		G sigmoid_result = matrix_dot(inp[i+1], one_minus_sigmoid);	


		
//		printf("sigmoid_result_calculated\n");
//		printf("matrix_dimensions(%ld,%ld)\n",
//				matrix_row_get(sigmoid_result),
//				matrix_col_get(sigmoid_result));


		matrix_delete(one_minus_sigmoid);

		G prev_transpose = matrix_transpose(inp[i]);



//		printf("prev_transpose_calculated\n");
//		printf("matrix_dimensions(%ld,%ld)\n",
//				matrix_row_get(prev_transpose),
//				matrix_col_get(prev_transpose));

		G sigmoid_and_alpha = matrix_dot(err[i+1], sigmoid_result);

		matrix_delete(sigmoid_result);


//		printf("sigmoid_and_alpha_calculated\n");
//		printf("matrix_dimensions(%ld,%ld)\n",
//				matrix_row_get(sigmoid_and_alpha),
//				matrix_col_get(sigmoid_and_alpha));



		G delta_wjk = matrix_mult(sigmoid_and_alpha, prev_transpose);
		matrix_delete(prev_transpose);
		matrix_delete(sigmoid_and_alpha);


//		printf("delta_wjk calculated\n");
//		printf("matrix_dimensions(%ld,%ld)\n",
//				matrix_row_get(delta_wjk),
//				matrix_col_get(delta_wjk));



//		printf("matrix_dimensions(%ld,%ld)\n",
//				matrix_row_get(neural_net->weights[i]),
//				matrix_col_get(neural_net->weights[i]));
		G new_weights = matrix_add(neural_net->weights[i], delta_wjk);


//		printf("new_weights calculated\n");
//		printf("matrix_dimensions(%ld,%ld)\n",
//				matrix_row_get(new_weights),
//				matrix_col_get(new_weights));

		matrix_delete(delta_wjk);

		matrix_delete(neural_net->weights[i]);
		neural_net->weights[i] = new_weights;

	}
}

UINT neuron_layers(N neural_net) {
	return neural_net->layers;
}

void neuron_print(N neural_net) {
	char buf[50];
	buf[0] = '\0';
	char ext[20];
	char ext2[20];
	sprintf(ext, "neural_net(%s):", MATRIX_DESC_FMT_STR);
	printf("%s\n", ext);
	sprintf(buf, ext, neural_net->layers);

	UINT i;

	for(i = 0; i < neural_net->layers; ++i) {
		sprintf(ext, "%s",MATRIX_DESC_FMT_STR);
		printf("%s\n", ext);
		sprintf(ext2, ext, matrix_col_get(neural_net->weights[i]));
		strcat(buf,ext2);
		strcat(buf, "->");
		if(i == neural_net->layers-1){
		sprintf(ext, "%s",MATRIX_DESC_FMT_STR);
		sprintf(ext2, ext, matrix_row_get(neural_net->weights[i]));
		strcat(buf,ext2);

		}
	}

	printf("%s\n", buf);
	
	for(i = 0; i < neural_net->layers; ++i) {
		matrix_print(neural_net->weights[i]);
	}
}
