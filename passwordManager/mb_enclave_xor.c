#include <time.h>
#include <stdio.h>
#include <stdlib.h>

/* generate random string for cipher key */
void rand_string(unsigned char *str, uint32_t size) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()_+=-][{}';\":?><,./;`~";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
}

/* Basic XOR cipher */
void encrypt_password(unsigned char* p, uint32_t* p_len, 
	unsigned char* ciphertext, uint32_t* cp_len) {

	srand(time(0));
	unsigned char key[*p_len];
	rand_string(key, *p_len);
	unsigned char decrypted[*p_len];

	for(int i=0; i < *p_len; i++) {
		ciphertext[i] = p[i]^key[i];
 	}
 	for(int i=0; i < *p_len; i++) {
 		decrypted[i] = ciphertext[i]^key[i];
 	}
	printf("Cipher text: %s\n", ciphertext);
	printf("Decrypted ciphertext: %s\n", decrypted);
	p = NULL;

}

//another idea, use srand to generate a key and then XOR that key with your string
