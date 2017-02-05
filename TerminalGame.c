#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

int DEFAULTFLOORLENGTH = 30;


struct termios initial_settings, new_settings;


// prints a floor + the number of deletes to return to the initial position
int printfloor(int length) {
	printf("\n");
	int i;
	for(i = 0; i<length; i++) {
		printf("-");
	}
	printf("\n");
	return length+2;
}

// prints a character at a position, returns number of deletes to return to initial pos
int printCharacter(int pos) {
	int returnVal = 0;
	int i;
	for(i = 0; i<pos; i++) printf(" ");
	printf("X");

	returnVal = pos + 1;

	int floorLength = (pos > DEFAULTFLOORLENGTH) ? pos + 10 : DEFAULTFLOORLENGTH; 
	returnVal += printfloor(floorLength);

	return returnVal;
}

void clearScreen(int deletes) {
	int i = 0;
	for(i = 0; i<deletes; i++) printf("\b\n\b");
	printf("\n");
	printf("\b");
	return;
}



int main() {
	int n;
	int characterPos = 4, charactersToDelete = 0;	
	unsigned char key;

	tcgetattr(0, &initial_settings);

	new_settings = initial_settings;
	new_settings.c_lflag &= ~ICANON;
	new_settings.c_lflag &= ~ECHO;
	new_settings.c_lflag &= ~ISIG;
	new_settings.c_cc[VMIN] = 0;
	new_settings.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &new_settings);


	while(1) {
		n = getchar();
		clearScreen(charactersToDelete);
		charactersToDelete = 0;
		if(n != EOF) {
			key = n;
			if(key == 27) { //Escape Key
				break;
			}
			if(key == 'a') {
				characterPos--;
			}
			if(key == 'd') {
				characterPos++;
			}
		}		
		charactersToDelete = printCharacter(characterPos);
	}

	tcsetattr(0, TCSANOW, &initial_settings);

	return 0;
}
