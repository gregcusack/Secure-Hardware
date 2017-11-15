#include <stdint.h>

void rand_string(unsigned char *str, uint16_t size);
void encrypt_password(unsigned char* p, uint16_t* p_len, 
	unsigned char* ciphertext);
