#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mb_enclave_xor.h"

/* generate random string for cipher key */
void rand_string(unsigned char *str, uint16_t size) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()_+=-][{}';\":?><,./;`~";
    if (size) {
        --size;
        for (uint16_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
            //printf("%c", str[n]);
        }
        //printf("\n");
        str[size] = '\0';
    }
}

/* Basic XOR cipher */
void encrypt_password(unsigned char* p, uint16_t* p_len,
	unsigned char* ciphertext) {

	unsigned char *key = "B&/n^!v8G`3BJL:B~q`~K(y!~;SBDw0\0";

	srand(time(0));
	//unsigned char key[*p_len];
	//rand_string(key, *p_len);
	unsigned char decrypted[*p_len];

	for(int i=0; i < *p_len; i++) {
		ciphertext[i] = p[i]^key[i];
 	}
 	for(int i=0; i < *p_len; i++) {
 		decrypted[i] = ciphertext[i]^key[i];
 	}
	printf("Cipher text: %s\n", ciphertext);
	printf("Decrypted ciphertext: %s\n", decrypted);
	memset(p,0,*p_len); //remove access to password from untrusted function
}