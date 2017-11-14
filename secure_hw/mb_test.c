#include <string.h>

/* Toggles input buffer case */
void toggle_case(unsigned char* input, unsigned char* output) {	
	for (int i = 0; i < strlen((char*)input); i++) {
		if(input[i] >= 'A' && input[i] <= 'Z') {
			output[i] = input[i] + 'a' - 'A';
		}
		else if(input[i] >= 'a' && input[i] <= 'z') {
			output[i] = input[i] + 'A' - 'a';
		}
		else {
			output[i] = input[i];
		}
	}
}