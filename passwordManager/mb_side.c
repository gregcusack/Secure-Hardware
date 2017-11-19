#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#ifndef MBSIDE_H
#define MBSIDE_H
#include "mbside.h"
#endif

#define KEY ((const unsigned char *) "B&/n^!v8G`3BJL:B~q`~K(y!~;SBDw0:\0")

void encrypt_m_pword(user_account *input_user, uint32_t size, user_account *ret_user) {
	for(uint8_t i=0; i < size - 1; i++) {
		ret_user->m_pword[i] = input_user->m_pword[i]^KEY[i];
		ret_user->m_uname[i] = input_user->m_uname[i];
	}
	ret_user->m_pword[size-1] = '\0';
	memset(input_user->m_pword, 0, size);
}

void decrypt_m_pword(unsigned char* cipher_p, uint32_t size, unsigned char* decrypted) {
	for(uint32_t i=0; i < size; i++) {
		decrypted[i] = cipher_p[i]^KEY[i];
	}
}

void create_user(user_account *user_data, uint32_t *size, 
	user_account *user_cipher) {
	encrypt_m_pword(user_data, *size, user_cipher);
}

void check_user(login_struct *login_attempt, uint32_t *size, user_account *cipher_data, uint32_t *found) {
	unsigned char decrypted[*size];
	decrypt_m_pword(cipher_data->m_pword, *size, decrypted);
	if(!strcmp(login_attempt->l_pword, decrypted)) {
		*found = true;
	}
	else {
		*found = false;
	}
}

void encrypt_credentials(website *user_cred, uint32_t *size, 
	website *encrypted_user_cred) {
	for(uint8_t i=0; i < *size; i++) {
		encrypted_user_cred->web_name[i] = user_cred->web_name[i]^KEY[i];
		encrypted_user_cred->credentials.a_uname[i] = user_cred->credentials.a_uname[i]^KEY[i];
		encrypted_user_cred->credentials.a_pword[i] = user_cred->credentials.a_pword[i]^KEY[i];
	}
	encrypted_user_cred->web_name[*size-1] = '\0';
	encrypted_user_cred->credentials.a_uname[*size-1] = '\0';
	encrypted_user_cred->credentials.a_pword[*size-1] = '\0';
	memset(user_cred->web_name,0,*size);
	memset(user_cred->credentials.a_uname,0,*size);
	memset(user_cred->credentials.a_pword,0,*size);
}

void return_credentials(user_account *user, website *user_cred, 
	uint32_t *size, website *ret_cred, uint32_t *cred_found) {
	//we may get a seg fault here.
	unsigned char decrypted[*size];
	for(uint8_t i=0; i < user->num_accounts; i++) {
		for(uint8_t j=0; j < *size; j++) {
			decrypted[j] = user->accounts[i].web_name[j]^KEY[j];
		}
		if(!strcmp(decrypted, user_cred->web_name)) {
			for(uint8_t k=0;k < *size; k++) {
				ret_cred->web_name[k] = decrypted[k];
				ret_cred->credentials.a_uname[k] = user->accounts[i].credentials.a_uname[k]^KEY[k];
				ret_cred->credentials.a_pword[k] = user->accounts[i].credentials.a_pword[k]^KEY[k];
			}
			*cred_found = true;
			return;
		}
	}
	*cred_found = false;
}





















	//login_struct *cipher_data, uint32_t *pword_len, 
	//user_account *login_attempt, uint32_t *found);