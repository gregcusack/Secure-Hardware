#include <stdio.h>
#include "mb_enclave_xor.h"

#define BUFF_SIZE 32


int main(int argc, char** argv) {
	/* Added input args to main.
	 * Not sure if this is the 
	 * right design choice */
	if(argc != 2) {
		printf("ERROR: Invalud # of input arguments.  Run: ./<exec> <password>\n");
		return -1;
	}
	unsigned char *p = argv[1];
	//unsigned char p[] = "Hello World"; // if don't want command line input
										 // use this line
	uint16_t a = BUFF_SIZE;
	uint16_t* p_len = &a;
	unsigned char cipher_p[*p_len];

	encrypt_password(p, p_len, cipher_p);
	printf("Encrypted Password: %s\n", cipher_p);
	printf("Input Password: %s\n", p);

	return 0;
}