#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#ifndef MBSIDE_H
#define MBSIDE_H
#include "mbside.h"
#endif

#define KEY ((const unsigned char *) "B&/n^!v8G`3BJL:B~q`~K(y!~;SBDw0\0")

void encrypt(user_account *input_user, uint32_t size, user_account *ret_user) {
	for(uint16_t i=0; i < size - 1; i++) {
		ret_user->pword[i] = input_user->pword[i]^KEY[i];
		ret_user->uname[i] = input_user->uname[i];
	}
	ret_user->pword[size-1] = '\0';
	memset(input_user->pword, 0, size);
}

void decrypt(unsigned char* cipher_p, uint32_t size, unsigned char* decrypted) {
	for(uint32_t i=0; i < size; i++) {
		decrypted[i] = cipher_p[i]^KEY[i];
	}
}

void create_user(user_account *user_data, uint32_t *pword_len, 
	user_account *user_cipher) {
	encrypt(user_data, *pword_len, user_cipher);
}

void check_user(user_account *cipher_data, uint32_t *pword_len, 
	user_account *login_attempt, uint32_t *found) {

	unsigned char decrypted[*pword_len];
	decrypt(cipher_data->pword, *pword_len, decrypted);
	if(!(strcmp(decrypted,login_attempt->pword))) {
		*found = true;
	}
	else {
		*found = false;
	}
}

