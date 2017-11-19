#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#ifndef MBSIDE_H
#define MBSIDE_H
#include "mbside.h"
#endif

#define KEY ((const unsigned char *) "B&/n^!v8G`3BJL:B~q`~K(y!~;SBDw0:\0")

void encrypt_m_pword(user_account *input_user, uint32_t size, user_account *ret_user) {
	for(uint16_t i=0; i < size - 1; i++) {
		ret_user->m_pword[i] = input_user->m_pword[i]^KEY[i];
		ret_user->m_uname[i] = input_user->m_uname[i];
	}
	ret_user->m_pword[size-1] = '\0';
	memset(input_user->m_pword, 0, size);
}

void encrypt_login(user_account *input_user, uint32_t size, 
	user_account *ret_user) {
	for(uint8_t i=0;i<size;i++) {
		ret_user->accounts[ret_user->num_accounts].web_name[i] = input_user->accounts[input_user->num_accounts].web_name[i]^KEY[i];
		ret_user->accounts[ret_user->num_accounts].credentials.a_uname[i] = input_user->accounts[input_user->num_accounts].credentials.a_uname[i]^KEY[i];
		ret_user->accounts[ret_user->num_accounts].credentials.a_pword[i] = input_user->accounts[input_user->num_accounts].credentials.a_pword[i]^KEY[i];
	}
	//not totally sure if i need to do this
	ret_user->accounts[ret_user->num_accounts].web_name[size-1] = '\0';
	ret_user->accounts[ret_user->num_accounts].credentials.a_uname[size-1] = '\0';
	ret_user->accounts[ret_user->num_accounts].credentials.a_pword[size-1] = '\0';
	//remove access to user account from ARM
	memset(input_user->m_pword,0,size);
	input_user->num_accounts = 0;  //
	memset(input_user->accounts[ret_user->num_accounts].web_name, 0, size);
	memset(input_user->accounts[ret_user->num_accounts].credentials.a_uname, 0, size);
	memset(input_user->accounts[ret_user->num_accounts].credentials.a_pword, 0, size);
}

void decrypt_m_pword(unsigned char* cipher_p, uint32_t size, unsigned char* decrypted) {
	for(uint32_t i=0; i < size; i++) {
		decrypted[i] = cipher_p[i]^KEY[i];
	}
}

void create_user(user_account *user_data, uint32_t *pword_len, 
	user_account *user_cipher) {
	encrypt_m_pword(user_data, *pword_len, user_cipher);
}

void check_user(user_account *cipher_data, uint32_t *pword_len, 
	user_account *login_attempt, uint32_t *found) {

	unsigned char decrypted[*pword_len];
	decrypt_m_pword(cipher_data->m_pword, *pword_len, decrypted);
	if(!(strcmp(decrypted,login_attempt->m_pword))) {
		*found = true;
	}
	else {
		*found = false;
	}
}

//encrypt the account that corresponds to the number of accounts
void add_login(user_account *user_data, uint32_t *pword_len,
	user_account *cipher_account) {
	//user_data here is passed in encrypted (except for m_uname)
	encrypt(user_data, *pword_len, cipher_account);
}






















