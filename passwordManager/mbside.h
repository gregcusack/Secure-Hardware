#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#ifndef USERCLASS_H
#define USERCLASS_H
#include "userclass.h"
#endif

#ifndef AES_H
#define AES_H
#include "tiny-AES-c/aes.h"
#endif

// #ifndef PTHREAD_H
// #define PTHREAD_H
// #include <pthread.h>
// #endif
/*
void create_user(user_account *user_data, uint32_t *pword_len,
	user_account *user_cipher);
*/
//return cipher_pw, done_flag
//void create_user(unsigned char *create_pw, unsigned int *size,
//	unsigned char *cipher_pw, unsigned int *done_flag);

void create_user(unsigned char *create_pw, unsigned int *size, 
	uint8_t *master_iv_in, unsigned char *cipher_pw, uint8_t *master_iv_out);

//return found, done flag
void check_user(unsigned char *login_attempt, unsigned int *size,
	unsigned char *cipher_data, uint8_t *master_iv, unsigned int *found);

//return cred_found, done_flag
void decrypt_and_check_for_web_credentials(unsigned char *web_name, uint8_t *iv_in,
	unsigned char *user_cred_get, unsigned int *size, unsigned int *cred_found);

//return ret_cred_web/uname/pword/done_flag
void return_credentials(unsigned char *web_name, unsigned char *a_uname,
	unsigned char *a_pword, uint8_t *iv_in, unsigned int *size,
	unsigned char *ret_cred_web, unsigned char *ret_cred_uname,
	unsigned char *ret_cred_pword);

//return cipher_web_name/uname/pword/done_flag
void encrypt_credentials(unsigned char *web_name, unsigned char *a_uname,
	unsigned char *a_pword, unsigned int *size, uint8_t *iv_in, unsigned char *cipher_web_name,
	unsigned char *cipher_a_uname, unsigned char *cipher_a_pword, uint8_t *iv_out);

/*
void create_user(user_account *user_data, uint32_t *size,
	user_account *user_cipher, locks *lock);
*/

/*
void check_user(login_struct *login_attempt, uint32_t *size,
	user_account *cipher_data, uint32_t *found, locks *lock);
*/

/* have option to add login once check_user called */
//void add_login(user_account *user_data, uint32_t *pword_len,
//	user_account *cipher_account);



/*
void encrypt_credentials(website *user_cred, uint32_t *size,
	website *encrypted_user_cred, locks *lock);
*/


/*
void return_credentials(user_account *user, website *user_cred,
	uint32_t *size, website *ret_cred, uint32_t *cred_found, locks *lock);
*/
