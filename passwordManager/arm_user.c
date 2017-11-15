#include <stdio.h>
#include "mb_enclave.h"

int main(int argc, char** argv) {
	unsigned char* p = (unsigned char*)"Greg";
	uint32_t a = 4;
	uint32_t* p_len = &a;
	unsigned char cipher_p[*p_len];
	uint32_t* cp_len;
	//printf("size p_len: %u\n", *p_len);
	encrypt_password(p, p_len, cipher_p, cp_len);
	printf("cipher: %s\n", cipher_p);

	return 0;
}