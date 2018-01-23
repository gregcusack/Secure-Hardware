#include <stdint.h>
#include <string.h>
#include <stdbool.h>


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
/*
static void encrypt_ctr(unsigned char *create_pw, unsigned char *cipher_pw);
static void decrypt_ctr(unsigned char *create_pw, unsigned char *cipher_pw);
static void xcrypt_ctr(const char* xcrypt, unsigned char *create_pw, unsigned char *cipher_pw);

static void encrypt_ctr(unsigned char *create_pw, unsigned char *cipher_pw) {
    xcrypt_ctr("encrypt", create_pw, cipher_pw);
}

static void decrypt_ctr(unsigned char *create_pw, unsigned char *cipher_pw) {
    xcrypt_ctr("decrypt", create_pw, cipher_pw);
}

static void xcrypt_ctr(const char* xcrypt, unsigned char *create_pw, unsigned char *cipher_pw) {
	struct AES_ctx ctx;
	printf("IN: %s\n", create_pw);
	AES_CTR_xcrypt_buffer(ctx, create_pw, *size);
	printf("OUT: %s\n", create_pw);

	uint8_t key[32] = { 0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
                        0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4 };
    uint8_t iv[16]  = { 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff };
    uint8_t out[64] = { 0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
                        0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
                        0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
                        0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17, 0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10 };
    struct AES_ctx ctx;
    printf("IN: %s\n", create_pw);

    AES_init_ctx_iv(&ctx, key, iv);
    //AES_CTR_xcrypt_buffer(&ctx, in, SIZE);
    AES_CTR_xcrypt_buffer(&ctx, create_pw, SIZE);
    printf("CTR %s: ", xcrypt);
    printf("OUT: %s\n", create_pw);
    
  
}
*/
/*
void encrypt_m_pword(unsigned char *input_user, unsigned int size, unsigned char *ret_user) {
	for(unsigned int i=0; i < size - 1; i++) {
		ret_user[i] = input_user[i]^KEY[i];
	}
	ret_user[size-1] = '\0';
	memset(input_user, 0, size); //zero fill buffer
}
*/
/*                  
void decrypt_m_pword(unsigned char* cipher_p, uint32_t size, unsigned char* decrypted) {
	for(uint32_t i=0; i < size; i++) {
		decrypted[i] = cipher_p[i]^KEY[i];
	}
}
*/
void create_user(unsigned char *create_pw, unsigned int *size, unsigned char *cipher_pw, unsigned int *done_flag) {
	memcpy(cipher_pw, create_pw, SIZE);
	struct AES_ctx ctx;
	AES_init_ctx_iv(&ctx, key, iv);
	printf("IN: %s\n", cipher_pw);
	AES_CTR_xcrypt_buffer(&ctx, cipher_pw, SIZE);// *size);
	printf("OUT: %s\n", cipher_pw);
	
	//encrypt_ctr(create_pw, cipher_pw);
	//encrypt_m_pword(create_pw, *size, cipher_pw);
	*done_flag = 1;
}

//return found, done_flag
void check_user(unsigned char *login_attempt, unsigned int *size,
	unsigned char *cipher_data, unsigned int *found, unsigned int *done_flag) {
	struct AES_ctx ctx;
	AES_init_ctx_iv(&ctx, key, iv);
	unsigned char tmp[SIZE];
	memcpy(tmp, cipher_data, SIZE);
	printf("Login pw attempt: %s\n", login_attempt);
	printf("Login stored cipher data: %s\n", tmp);
	AES_CTR_xcrypt_buffer(&ctx, tmp, SIZE);//*size);
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
