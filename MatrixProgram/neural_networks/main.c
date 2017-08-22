#include "neuron.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#define STR_TO_SHORT 10
#define NOT_CSV_END  11
#define STR_OKAY     0

#define tuple_T T

struct T {
	matrix_G input;
	matrix_G expected;
};

typedef struct T *T;

matrix_G expected_inputs[10];

int check_filename(char *filename){
	if(strlen(filename) < 4) return STR_TO_SHORT;
	if(strcmp(filename + strlen(filename) - 4, ".csv") != 0) return NOT_CSV_END;
	return  STR_OKAY;
}


void initialize_expected_inputs() {

	UINT i;

	matrix_G base = matrix_fill(10, 1, 0.01);

	for(i = 0; i < 10; i++){
		expected_inputs[i] = matrix_copy(base);
		matrix_set(expected_inputs[i], i, 0, 1.0);
	}

	matrix_delete(base);
}

INT scale_by_256(INT val) {
	return 0.99*(val/256) + 0.01;
}

matrix_G normalize_input(matrix_G input) {
	return matrix_map(input, scale_by_256);
}

int read_csv(FILE *fp, T tuple, UINT sz) {
	int label;
	fscanf(fp, "%d,", &label);

	assert(label < 10 && label >= 0);
	tuple->expected = expected_inputs[label];

	matrix_G input_raw= matrix_csv_fload(fp, sz,1);

	tuple->input = normalize_input(input_raw);

	matrix_delete(input_raw);
	
	char c;
	if((c = getc(fp)) == EOF)
		return 0;
	else {
		ungetc(c,fp);
		return 1;
	}
}


void neural_train(neuron_N neural_net, T tuple, INT alpha){
	UINT layers = neuron_layers(neural_net); 

	matrix_G inputs[layers+1];
	matrix_G errors[layers+1];

	neuron_feedforward(neural_net, tuple->input, inputs);

	printf("mat_mult(tuple->expected[%ld,%ld], inputs[layers][%ld,%ld])\n",
			matrix_row_get(tuple->expected),
			matrix_col_get(tuple->expected),
			matrix_row_get(inputs[layers]),
			matrix_col_get(inputs[layers]));

			
	matrix_print(inputs[layers]);
	matrix_print(tuple->expected);

	matrix_G error = matrix_sub(tuple->expected, inputs[layers]);

	matrix_print(error);

	neuron_feedbackward(neural_net, error, errors);

	neuron_train(neural_net,errors,inputs,alpha);


	UINT i;
	for(i = 0; i <= layers; i++) {

		matrix_delete(inputs[i]);

	}

	for(i = 0; i < layers; i++) {
		matrix_delete(errors[i]);
	}

	matrix_delete(error);
}

void neural_predict(neuron_N neural_net, matrix_G input) {

	matrix_G result = neuron_apply(neural_net, input);

	UINT index = matrix_high_value_col(result,0);

	matrix_print(result);

	char buf[20];
	sprintf(buf, "%s is the closest match\n", MATRIX_DESC_FMT_STR);

	printf(buf, index);
}


neuron_N train_network(UINT epochs) {
	int result = -1;
	char filename[100];
	
	do {
	
	switch(result) {
		case STR_TO_SHORT:
			printf("The string you submitted was too short (must be > 4).\n");
			break;
		case NOT_CSV_END:
			printf("The filename you submitted did not end in csv.\n");
			break;
		case -1:
			break;
	}
	printf("Please enter the training set(csv) name: ");


	scanf(" %100[^\n ][^\n]", filename);
	getchar();


	} while((result = check_filename(filename)));


	printf("The data is assumed to be in the format - label (0-9) then 28*28 integral values between the range 0..256\n");

	printf("Loading the data...\n");

	FILE *fp = fopen(filename, "r");



	/*
	for(i = 0; i < 3*3; i++) {
		fscanf(fp, "%lf,", values + i);
		printf("%lf\n", *(values+i));
	}*/

	T tuple = malloc(sizeof(*tuple));
	neuron_N neural_net = neuron_new_uniform(28*28,100,10);

	printf("Begining to train network...\n");
	printf("Read input...\n");
	UINT j;
	for(j =0; j < epochs; j++){
	
		read_csv(fp, tuple, 28*28);
		neural_train(neural_net, tuple, 0.0025);

		matrix_delete(tuple->input);

	}
	printf("Training completed...\n");
	fclose(fp);
	return neural_net;
}


int main() {

	initialize_expected_inputs();

	neuron_N neural_net = train_network(1000);

	FILE *sp = fopen("neural.net", "w");

	neuron_serialize(sp, neural_net);

	fclose(sp);



}


