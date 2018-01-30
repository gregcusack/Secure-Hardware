#include <string.h>
#include <stdlib.h>

#ifndef MBSIDE_H
#define MBSIDE_H
#include "mbside.h"
#endif

// #include "microblaze_protocol_header.h"

unsigned char key[32] = { 0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 
						0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81, 0x1f, 0x35, 
						0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 
						0xa3, 0x09, 0x14, 0xdf, 0xf4 };

void gen_iv(unsigned char *_iv) {
	for(int i=0; i < IV_SIZE; i++) {
		sprintf((char*)_iv + i, "%x", rand() % 16);
	}
}

void xcrypt(unsigned char *in_str, unsigned char *_iv) {
	struct AES_ctx ctx;
	AES_init_ctx_iv(&ctx, key, _iv);
	AES_CTR_xcrypt_buffer(&ctx, in_str, BUFF_SIZE);
}

/*
unsigned int compare(unsigned char *input, unsigned char *decrypted) {
	return (!strncmp((char*)input, (char*)decrypted,BUFF_SIZE));
}
*/

void create_user(unsigned char *create_pw, unsigned int *size, 
	unsigned char *master_iv_in, unsigned char *cipher_pw, 
	unsigned char *master_iv_out) {
	memcpy(cipher_pw, create_pw, BUFF_SIZE);
	gen_iv(master_iv_out);
	xcrypt(cipher_pw, master_iv_out);
}

//return found
void check_user(unsigned char *login_attempt, unsigned int *size,
	unsigned char *cipher_data, unsigned char *master_iv, unsigned int *found) {
	unsigned char tmp[BUFF_SIZE];
	memcpy(tmp, cipher_data, BUFF_SIZE);
	xcrypt(tmp, master_iv);
	*found = !strncmp((char*)login_attempt, (char*)tmp, BUFF_SIZE);
	//*found = compare(login_attempt,tmp);
}

//return cred_found
void decrypt_and_check_for_web_credentials(unsigned char *web_name, 
	unsigned char *iv_in, unsigned char *user_cred_get,
	unsigned int *size, unsigned int *found) {
	unsigned char tmp[BUFF_SIZE];
	memcpy(tmp, web_name, BUFF_SIZE);
	xcrypt(tmp, iv_in);
	*found = !strncmp((char*)user_cred_get, (char*)tmp, BUFF_SIZE);
	//*found = compare(user_cred_get,tmp);
}

//return cipher_web_name/uname/pword
void encrypt_credentials(unsigned char *web_name, unsigned char *a_uname,
	unsigned char *a_pword, unsigned int *size, unsigned char *iv_in, 
	unsigned char *cipher_web_name, unsigned char *cipher_a_uname, 
	unsigned char *cipher_a_pword, unsigned char *iv_out) {
	memcpy(cipher_web_name, web_name, BUFF_SIZE);
	memcpy(cipher_a_uname, a_uname, BUFF_SIZE);
	memcpy(cipher_a_pword, a_pword, BUFF_SIZE);
	gen_iv(iv_out);
	xcrypt(cipher_web_name, iv_out);
	xcrypt(cipher_a_uname, iv_out);
	xcrypt(cipher_a_pword, iv_out);
}

//return ret_cred_web/uname/pword
void return_credentials(unsigned char *web_name, unsigned char *a_uname,
	unsigned char *a_pword, unsigned char *iv_in, unsigned int *size,
	unsigned char *ret_cred_web, unsigned char *ret_cred_uname,
	unsigned char *ret_cred_pword) {
	memcpy(ret_cred_web, web_name, BUFF_SIZE);
	memcpy(ret_cred_uname, a_uname, BUFF_SIZE);
	memcpy(ret_cred_pword, a_pword, BUFF_SIZE);
	xcrypt(ret_cred_web, iv_in);
	xcrypt(ret_cred_uname, iv_in);
	xcrypt(ret_cred_pword, iv_in);
}

//return cred_found
void new_decrypt_and_check_for_web_credentials(unsigned char *web_name, 
	unsigned char *web_uname, unsigned char *web_pword,
	unsigned char *iv_in, unsigned char *user_cred_get, unsigned int *size, 
	unsigned char *ret_cred_web, unsigned char *ret_cred_uname,
	unsigned char *ret_cred_pword, unsigned int *found) {
	unsigned char tmp_web_name[BUFF_SIZE];
	memcpy(tmp_web_name, web_name, BUFF_SIZE);
	xcrypt(tmp_web_name, iv_in);
	if(!strncmp((char*)user_cred_get, (char*)tmp_web_name, BUFF_SIZE)) {
		memcpy(ret_cred_web, web_name, BUFF_SIZE);
		memcpy(ret_cred_uname, web_uname, BUFF_SIZE);
		memcpy(ret_cred_pword, web_pword, BUFF_SIZE);
		xcrypt(ret_cred_web, iv_in);
		xcrypt(ret_cred_uname, iv_in);
		xcrypt(ret_cred_pword, iv_in);
		*found = 1;
	}
	else {
		*found = 0;
	}
}

