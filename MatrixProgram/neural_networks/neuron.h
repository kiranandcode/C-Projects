#ifndef NEURON_H
#define NEURON_H

//------------------------------------------------
//         GOPIANDCODE's NEURON LIBRARY
//         ----------------------------
//  "when you want to ML, but you don't mind
//               doing it slowly..."
//------------------------------------------------
//
//  Don't forget to link with the math's library
//
//------------------------------------------------

#include "../matrix.h"
#define N neuron_N
struct N;
typedef struct N *N;
#define G matrix_G


G neuron_apply(N neural_net, G inp);
void neuron_feedforward(N neural_net, G inp, G *output);
void neuron_feedbackward(N neural_net, G inp, G *output);
void neuron_train(N neural_net, G *err, G *inp, INT alpha);

#undef G
#undef N
#endif
