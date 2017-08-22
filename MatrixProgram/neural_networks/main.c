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

int neural_predict(neuron_N neural_net, T tuple) {

	matrix_G result = neuron_apply(neural_net, tuple->input);

	UINT predicted = matrix_high_value_col(result,0);
	UINT actual    = matrix_high_value_col(tuple->expected,0);


	if(predicted == actual) {
		
		matrix_delete(result);
		return 1;
	}
	else {
		// got it wrong, print info
		char buf[60];
		sprintf(buf, "%s was the actual value, but the network predicted %s\n", MATRIX_DESC_FMT_STR, MATRIX_DESC_FMT_STR);
		printf(buf, actual, predicted);
		printf("Here is the expected value:\n");
		matrix_print(tuple->expected);
		printf("Here is the network's output:\n");
		matrix_print(result);
		matrix_delete(result);
		return 0;
	}
}


neuron_N train_network(UINT epochs) {
	int result = -1;
	char filename[100];
	FILE *fp;
	
	do {
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

		fp = fopen(filename, "r");

		if(fp == NULL)
			printf("ERROR: Could not open the specified file\n");

	}while(fp == NULL);


	/*
	for(i = 0; i < 3*3; i++) {
		fscanf(fp, "%lf,", values + i);
		printf("%lf\n", *(values+i));
	}*/

	T tuple = malloc(sizeof(*tuple));
	neuron_N neural_net = neuron_new_uniform(28*28,100,10);

	double training_rate;
	do{
		printf("What training rate should be used to train the network?:\n");
		scanf(" %lf", &training_rate);
		if(training_rate < 0)
			printf("ERROR: The training rate must be a positive real value\n");
		else
			printf("ERROR: You DON'T want a training rate > 10...\n");
	} while(training_rate < 0 || training_rate > 10);

	printf("Begining to train network...\n");
	printf("Read input...\n");
	UINT j;
	if(epochs == 0) {
		while(read_csv(fp,tuple, 28*28)){
			neural_train(neural_net, tuple, training_rate);
			matrix_delete(tuple->input);
		}
	}
	else
	for(j =0; j < epochs; j++){
	
		read_csv(fp, tuple, 28*28);
		neural_train(neural_net, tuple, training_rate);

		matrix_delete(tuple->input);

	}
	printf("Training completed...\n");
	fclose(fp);
	return neural_net;
}


int main() {

	initialize_expected_inputs();

	printf("Gopiandcode's Neural Network C Library\n");
	printf("--------------------------------------\n");
	printf("%38s", "Intented to be used with MNIST csv data");
	
	printf("\n--------------------------------------\n\n");

	printf("The cmd line interface supports:\n");
	printf(" a) Train a new network?\n");
	printf(" b) Test an existing network?\n");


	printf("What is your choice? (e to exit):\n");

	char c;

	do {
		scanf(" %c", &c);
	} while(c != 'e' && c != 'a' && c != 'b');

	//neuron_N neural_net = train_network(3000);



	switch(c) {


		case 'a': {
				  // train new network
				  
				  printf("How many epochs would you like?(select 0 to retrieve as many as in the training set):\n");

				  long long epochs = 1;


				  do {
					  if(epochs < 0)
						  printf("That value was <0. Please enter a positive value:\n");
					  scanf("%lld", &epochs);
				  } while(epochs < 0);

				  
				  neuron_N network = train_network(epochs);

				  char buf[100];
			          FILE *fp;
				  char c = '\0';

				  do {
					  printf("What should I call the serialized network?:\n");
					  scanf(" %100s", buf);

					  fp = fopen(buf, "r");
					  if(fp) {
						  printf("WARNING: The filename specified already exists on the system.\n");
						  printf("         Are you sure you would like to continue?(y/n):\n");
						  do {
							  scanf(" %c", &c);
						  } while(!(c == 'y' || c == 'n'));

						  fclose(fp);
					  }
					  else c = 'y';
				  }
				  while(c == 'n');

				  fp = fopen(buf, "w");

				  neuron_serialize(fp, network);

				  return 0;
			break;
			  }
		case 'b':{
				 char buf[100];
				 FILE *fp;
				 do {
					 // begin test set
					 printf("What is the name of the neural network serialized file to open?:\n");

					 scanf(" %s", buf);

					 fp = fopen(buf, "r");
					 
					 if(fp == NULL)
						 printf("ERROR: Could not open the file at the specified location\n");

			  	 } while(fp == NULL);

				 printf("Loading network...\n");
				 neuron_N network = neuron_deserialize(fp);
				 printf("Loaded!\n");
				 fclose(fp);

				 int result = -1;
				 

			         do {
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
							printf("Please enter the testing set(csv) name: ");


							scanf(" %100[^\n ][^\n]", buf);
							getchar();


						} while((result = check_filename(buf)));


						printf("The data is assumed to be in the format - label (0-9) then 28*28 integral values between the range 0..256\n");

						printf("Loading the data...\n");

						fp = fopen(buf , "r");

						if(fp == NULL)
							printf("ERROR: Could not open the specified file\n");

					}while(fp == NULL);

					  printf("How many epochs would you like?(select 0 to retrieve as many as in the testing set):\n");
					  long long epochs = 1;


					  do {
						  if(epochs < 0)
							  printf("That value was <0. Please enter a positive value:\n");
						  scanf("%lld", &epochs);
					  } while(epochs < 0);

					  T tuple = malloc(sizeof(*tuple));
					  long long count = 0;
					  long long correct = 0;
					  
					  if(epochs == 0) {

						while(read_csv(fp,tuple, 28*28)){
							count++;
							if(neural_predict(network, tuple))
								correct++;
							matrix_delete(tuple->input);
						}
	

					  } else {
						  long long j;
						for(j =0; j < epochs; j++){
							printf("Iteration %lld/%lld\n", j, epochs);
							int val = read_csv(fp, tuple, 28*28);
							count++;
							if(neural_predict(network, tuple))
								correct++;
							
							matrix_delete(tuple->input);

							if(!val) break;
						}
					  }

					  fclose(fp);


					  printf("Testing Completed\n");
					  printf("Out of %lld tests, the network correctly identified %lld\n",
							  count,correct);

					  printf("Thus the network has a %2.2lf%% accuracy stat.\n",
							  (double)correct/count * 100);

					break;
			 }

		default:
			return 0;
	}




}


