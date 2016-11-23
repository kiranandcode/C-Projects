#ifndef MAIN_H
#define MAIN_H

#define TESTING 0
#include "stateQueue.h"

#define SAMPLESIZE 10
#define MAXLENGTH 10
#define OUTPUTTEST 5

int stateMasks[16] = {0b1100100000000000, 0b1110010000000000, 0b0111001000000000, 0b0011000100000000, 0b1000110010000000, 0b0100111001000000, 0b0010011100100000, 0b0001001100010000, 0b0000100011001000, 0b0000010011100100, 0b0000001001110010, 0b0000000100110001, 0b0000000010001100, 0b0000000001001110, 0b0000000000100111, 0b0000000000010011};

int startState(int count, int*);
void printBin(int n);
void unitTest(void);
int checkCompatible(int state, int position);
void generateStates(int state, head *queue, head *positions);
int weightState(int state);
int stateScore(int state);
int randPos(void);
head *monteCarlo(int state);
void iterTest(int state);
int getInput(void);
int convToGood(int badRef);
int convToBad(int goodRef);


#endif // MAIN_H
