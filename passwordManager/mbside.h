#include <stdint.h>
#include <stdbool.h>
#ifndef USERCLASS_H
#define USERCLASS_H
#include "userclass.h"
#endif

#ifndef PTHREAD_H
#define PTHREAD_H
#include <pthread.h>
#endif
/*
void create_user(user_account *user_data, uint32_t *pword_len, 
	user_account *user_cipher);
*/

void create_user(user_account *user_data, uint32_t *pword_len, 
	user_account *user_cipher, locks *lock);

void check_user(login_struct *login_attempt, uint32_t *size, 
	user_account *cipher_data, uint32_t *found);

/* have option to add login once check_user called */
//void add_login(user_account *user_data, uint32_t *pword_len,
//	user_account *cipher_account);

void encrypt_credentials(website *user_cred, uint32_t *size, 
	website *encrypted_user_cred);

void return_credentials(user_account *user, website *user_cred, 
	uint32_t *size, website *ret_cred, uint32_t *cred_found);