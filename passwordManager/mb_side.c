#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#ifndef MBSIDE_H
#define MBSIDE_H
#include "mbside.h"
#endif


// #include "microblaze_protocol_header.h"
// #include "userclass.h"
#define SIZE 64

//#define KEY ((const unsigned char *) "B&/n^!v8G`3BJL:B~q`~K(y!~;SBDw0:\0")
uint8_t iv[16]  = { 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff };
uint8_t key[32] = { 0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
                        0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4 };

void encrypt(unsigned char *m_pword, uint8_t *master_iv) {
	//uint8_t iv[16]  = { 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff };
	//uint8_t _iv[16 + 1];
	for(int i=0; i < 16; i++) {
		sprintf((char*)master_iv + i, "%x", rand() % 16);
	}
	printf("Hex string (iv): %s", iv);
	struct AES_ctx ctx;
	AES_init_ctx_iv(&ctx, key, master_iv);
	AES_CTR_xcrypt_buffer(&ctx, m_pword, SIZE);
}

void decrypt(unsigned char *cipher_pword, uint8_t *master_iv) {
	struct AES_ctx ctx;
	AES_init_ctx_iv(&ctx, key, master_iv);
	AES_CTR_xcrypt_buffer(&ctx, cipher_pword, SIZE);
}

void create_user(unsigned char *create_pw, unsigned int *size, uint8_t *master_iv_in, unsigned char *cipher_pw, unsigned int *done_flag, uint8_t *master_iv_out) {
	memcpy(cipher_pw, create_pw, SIZE);
	printf("IN: %s\n", cipher_pw);
	encrypt(cipher_pw, master_iv_out);
	printf("here\n");
	printf("OUT: %s\n", cipher_pw);
	*done_flag = 1;
	/*
	struct AES_ctx ctx;
	AES_init_ctx_iv(&ctx, key, iv);
	printf("IN: %s\n", cipher_pw);
	AES_CTR_xcrypt_buffer(&ctx, cipher_pw, SIZE);// *size);
	printf("OUT: %s\n", cipher_pw);
	*/
	
	//encrypt_ctr(create_pw, cipher_pw);
	//encrypt_m_pword(create_pw, *size, cipher_pw);
	//*done_flag = 1;
}

//return found, done_flag
void check_user(unsigned char *login_attempt, unsigned int *size,
	unsigned char *cipher_data, uint8_t *master_iv, unsigned int *found, unsigned int *done_flag) {
	unsigned char tmp[SIZE];
	memcpy(tmp, cipher_data, SIZE);
	printf("Login pw attempt: %s\n", login_attempt);
	printf("Login stored cipher data: %s\n", tmp);
	decrypt(tmp, master_iv);
	//struct AES_ctx ctx;
	//AES_init_ctx_iv(&ctx, key, master_iv);
	//unsigned char tmp[SIZE];
	//memcpy(tmp, cipher_data, SIZE);
	//AES_CTR_xcrypt_buffer(&ctx, tmp, SIZE);//*size);
	printf("Decrypted stored pw: %s\n", tmp);
	if(!strcmp((char*)login_attempt, (char*)tmp)) {
		*found = true;
	}
	else {
		*found = false;
	}
	*done_flag = 1;
	/*
	unsigned char decrypted[*size];
	decrypt_m_pword(cipher_data, *size, decrypted);
	if(!strcmp((char*)login_attempt, (char*)decrypted)) {
		*found = true;
	}
	else {
		*found = false;
	}
	*done_flag = 1;
	*/
}

//return cred_found, done_flag
void decrypt_and_check_for_web_credentials(unsigned char *web_name,
	unsigned char *user_cred_get, unsigned int *size, unsigned int *cred_found,
	unsigned int *done_flag) {
	unsigned char tmp[SIZE];
	printf("web_name: %s\n", web_name);
	memcpy(tmp, web_name, SIZE);
	printf("tmp: %s\n", tmp);
	struct AES_ctx ctx;
	AES_init_ctx_iv(&ctx, key, iv);
	AES_CTR_xcrypt_buffer(&ctx, tmp, SIZE);
	printf("tmp after: %s\n", tmp);
	if(!strcmp((char*)tmp, (char*)user_cred_get)) {
		*cred_found = 1;
	}
	else {
		*cred_found = 0;
	}
	*done_flag = 1;
	/*
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
	*/
}

//return cipher_web_name/uname/pword/done_flag
void encrypt_credentials(unsigned char *web_name, unsigned char *a_uname,
	unsigned char *a_pword, unsigned int *size, unsigned char *cipher_web_name,
	unsigned char *cipher_a_uname, unsigned char *cipher_a_pword,
	unsigned int *done_flag) {
	memcpy(cipher_web_name, web_name, SIZE);
	memcpy(cipher_a_uname, a_uname, SIZE);
	memcpy(cipher_a_pword, a_pword, SIZE);
	struct AES_ctx ctx;
	AES_init_ctx_iv(&ctx, key, iv);
	AES_CTR_xcrypt_buffer(&ctx, cipher_web_name, SIZE);//*size);
	AES_CTR_xcrypt_buffer(&ctx, cipher_a_uname, SIZE);//*size);
	AES_CTR_xcrypt_buffer(&ctx, cipher_a_pword, SIZE);//*size);
	//memset(web_name,0,SIZE);
	//memset(a_uname,0,SIZE);
	//memset(a_pword,0,SIZE);

	/*
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
	*/
	*done_flag = 1;
}

//return ret_cred_web/uname/pword/done_flag
void return_credentials(unsigned char *web_name, unsigned char *a_uname,
	unsigned char *a_pword, unsigned int *size,
	unsigned char *ret_cred_web, unsigned char *ret_cred_uname,
	unsigned char *ret_cred_pword, unsigned int *done_flag) {
	memcpy(ret_cred_web, web_name, SIZE);
	memcpy(ret_cred_uname, a_uname,SIZE);
	memcpy(ret_cred_pword, a_pword, SIZE);
	struct AES_ctx ctx;
	AES_init_ctx_iv(&ctx, key, iv);
	AES_CTR_xcrypt_buffer(&ctx, ret_cred_web, SIZE);//*size);
	AES_CTR_xcrypt_buffer(&ctx, ret_cred_uname, SIZE);//*size);
	AES_CTR_xcrypt_buffer(&ctx, ret_cred_pword, SIZE);//*size);
	/*memcpy(ret_cred_web, web_name, SIZE);
	memcpy(ret_cred_uname, a_uname,SIZE);
	memcpy(ret_cred_pword, a_pword, SIZE);
	*/
	/*
	for(unsigned int k=0; k < *size; k++) {
		ret_cred_web[k] = web_name[k]^KEY[k];
		ret_cred_uname[k] = a_uname[k]^KEY[k];
		ret_cred_pword[k] = a_pword[k]^KEY[k];
	}
	*/
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
