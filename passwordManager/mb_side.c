#include <stdint.h>
#include <string.h>
#include <stdbool.h>

// #ifndef MBSIDE_H
// #define MBSIDE_H
// #include "mbside.h"
// #endif
// #include "microblaze_protocol_header.h"
// #include "userclass.h"

#define KEY ((const unsigned char *) "B&/n^!v8G`3BJL:B~q`~K(y!~;SBDw0:\0")

void encrypt_m_pword(unsigned char *input_user, unsigned int size, unsigned char *ret_user) {
	for(unsigned int i=0; i < size - 1; i++) {
		ret_user[i] = input_user[i]^KEY[i];
	}
	ret_user[size-1] = '\0';
	memset(input_user, 0, size); //zero fill buffer
}

void decrypt_m_pword(unsigned char* cipher_p, uint32_t size, unsigned char* decrypted) {
	for(uint32_t i=0; i < size; i++) {
		decrypted[i] = cipher_p[i]^KEY[i];
	}
}

void create_user(unsigned char *create_pw, unsigned int *size, unsigned char *cipher_pw, unsigned int *done_flag) {
	encrypt_m_pword(create_pw, *size, cipher_pw);
	*done_flag = 1;
}

//return found, done_flag
void check_user(unsigned char *login_attempt, unsigned int *size,
	unsigned char *cipher_data, unsigned int *found, unsigned int *done_flag) {
	unsigned char decrypted[*size];
	decrypt_m_pword(cipher_data, *size, decrypted);
	if(!strcmp((char*)login_attempt, (char*)decrypted)) {
		*found = true;
	}
	else {
		*found = false;
	}
	*done_flag = 1;
}

//return cred_found, done_flag
void decrypt_and_check_for_web_credentials(unsigned char *web_name,
	unsigned char *user_cred_get, unsigned int *size, unsigned int *cred_found,
	unsigned int *done_flag) {
	unsigned char decrypted[*size];
	for(unsigned int j=0; j < *size; j++) {
		decrypted[j] = web_name[j]^KEY[j];
	}
	if(!strcmp((char*)decrypted, (char*)user_cred_get)) {
		*cred_found = 1;
	}
	else {
		*cred_found = 0;
	}
	*done_flag = 1;
}

//return cipher_web_name/uname/pword/done_flag
void encrypt_credentials(unsigned char *web_name, unsigned char *a_uname,
	unsigned char *a_pword, unsigned int *size, unsigned char *cipher_web_name,
	unsigned char *cipher_a_uname, unsigned char *cipher_a_pword,
	unsigned int *done_flag) {
	for(unsigned int i=0; i < *size; i++) {
		cipher_web_name[i] = web_name[i]^KEY[i];
		cipher_a_uname[i] = a_uname[i]^KEY[i];
		cipher_a_pword[i] = a_pword[i]^KEY[i];
	}
	cipher_web_name[*size-1] = '\0';
	cipher_a_uname[*size-1] = '\0';
	cipher_a_pword[*size-1] = '\0';
	memset(web_name,0,*size);
	memset(a_uname,0,*size);
	memset(a_pword,0,*size);
	*done_flag = 1;
}

//return ret_cred_web/uname/pword/done_flag
void return_credentials(unsigned char *web_name, unsigned char *a_uname,
	unsigned char *a_pword, unsigned int *size,
	unsigned char *ret_cred_web, unsigned char *ret_cred_uname,
	unsigned char *ret_cred_pword, unsigned int *done_flag) {
	for(unsigned int k=0; k < *size; k++) {
		ret_cred_web[k] = web_name[k]^KEY[k];
		ret_cred_uname[k] = a_uname[k]^KEY[k];
		ret_cred_pword[k] = a_pword[k]^KEY[k];
	}
	*done_flag = 1;
}



/*
void create_user(user_account *user_data, uint32_t *size,
	user_account *user_cipher, locks *lock) {
	pthread_mutex_lock(&(lock->m));
	encrypt_m_pword(user_data, *size, user_cipher);
	lock->done = 1;
	pthread_cond_signal(&(lock->w));
	pthread_mutex_unlock(&(lock->m));
}
*/


/*
void check_user(login_struct *login_attempt, uint32_t *size,
	user_account *cipher_data, uint32_t *found, locks *lock) {
	unsigned char decrypted[*size];
	pthread_mutex_lock(&(lock->m));
	decrypt_m_pword(cipher_data->m_pword, *size, decrypted);
	if(!strcmp(login_attempt->l_pword, decrypted)) {
		*found = true;
	}
	else {
		*found = false;
	}
	lock->done = 1;
	pthread_cond_signal(&(lock->w));
	pthread_mutex_unlock(&(lock->m));
}*/

/*
void encrypt_credentials(website *user_cred, uint32_t *size,
	website *encrypted_user_cred, locks *lock) {
	pthread_mutex_lock(&(lock->m));
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
	lock->done = 1;
	pthread_cond_signal(&(lock->w));
	pthread_mutex_unlock(&(lock->m));
}
*/

/*
void return_credentials(user_account *user, website *user_cred,
	uint32_t *size, website *ret_cred, uint32_t *cred_found, locks *lock) {
	//we may get a seg fault here.
	unsigned char decrypted[*size];
	pthread_mutex_lock(&(lock->m));
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
			lock->done = 1;
			pthread_cond_signal(&(lock->w));
			pthread_mutex_unlock(&(lock->m));
			return;
		}
	}
	*cred_found = false;
	lock->done = 1;
	pthread_cond_signal(&(lock->w));
	pthread_mutex_unlock(&(lock->m));
}
*/





















	//login_struct *cipher_data, uint32_t *pword_len,
	//user_account *login_attempt, uint32_t *found);
