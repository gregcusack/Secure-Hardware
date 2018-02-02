#include <string.h>
#include <stdlib.h>

#ifndef MBSIDE_H
#define MBSIDE_H
#include "mbside.h"
#endif

#include "microblaze_protocol_header.h"

unsigned char key[32] = { 0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 
						0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81, 0x1f, 0x35, 
						0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 
						0xa3, 0x09, 0x14, 0xdf, 0xf4 };

void gen_iv(unsigned char *_iv) {
	for(int i=0; i < IV_SIZE; i++) {
		sprintf((char*)_iv + i, "%x", rand() % 16);
	}
}

void hmac_xcrypt(unsigned char *in_str, unsigned char *_iv,
	unsigned char *master_key, unsigned int size) {
	struct AES_ctx ctx;
	AES_init_ctx_iv(&ctx, master_key, _iv);
	AES_CTR_xcrypt_buffer(&ctx, in_str, size);
}

void create_user(unsigned char *create_pw, unsigned int *size, 
	unsigned char *master_iv_in, unsigned char *cipher_pw, 
	unsigned char *master_iv_out) {
	memcpy(cipher_pw, create_pw, *size);
	
	unsigned char master_key[32];
	hmac_sha256(key, 32, create_pw, *size, master_key, 32);
	gen_iv(master_iv_out);

	hmac_xcrypt(cipher_pw, master_iv_out, master_key, *size);
}

//return found
void check_user(unsigned char *login_attempt, unsigned int *size,
	unsigned char *cipher_data, unsigned char *master_iv, unsigned int *found) {
	unsigned char tmp[*size];
	memcpy(tmp, cipher_data, *size);

	unsigned char master_key[32];
	hmac_sha256(key, 32, login_attempt, *size, master_key, 32);
	
	hmac_xcrypt(login_attempt, master_iv, master_key, *size);
	*found = !strncmp((char*)login_attempt, (char*)cipher_data, *size);
}

//return cipher_web_name/uname/pword
void encrypt_credentials(unsigned char *create_pw, unsigned char *web_name, unsigned char *a_uname,
	unsigned char *a_pword, unsigned int *size, unsigned char *iv_in, 
	unsigned char *cipher_web_name, unsigned char *cipher_a_uname, 
	unsigned char *cipher_a_pword, unsigned char *iv_out) {
	
	memcpy(cipher_web_name, web_name, *size);
	memcpy(cipher_a_uname, a_uname, *size);
	memcpy(cipher_a_pword, a_pword, *size);
	
	gen_iv(iv_out);
	unsigned char master_key[32];
	hmac_sha256(key, 32, create_pw, *size, master_key, 32);

	hmac_xcrypt(cipher_web_name, iv_out, master_key, *size);
	hmac_xcrypt(cipher_a_uname, iv_out, master_key, *size);
	hmac_xcrypt(cipher_a_pword, iv_out, master_key, *size);
}

//return cred_found
void check_and_return_credentials(unsigned char *create_pw, unsigned char *web_name, 
	unsigned char *web_uname, unsigned char *web_pword,
	unsigned char *iv_in, unsigned char *user_cred_get, unsigned int *size, 
	unsigned char *ret_cred_web, unsigned char *ret_cred_uname,
	unsigned char *ret_cred_pword, unsigned int *found) {

	unsigned char master_key[32];
	hmac_sha256(key, 32, create_pw, *size, master_key, 32);
	unsigned char tmp_web_name[*size];
	memcpy(tmp_web_name, web_name, *size);
	hmac_xcrypt(tmp_web_name, iv_in, master_key, *size);
	if(!strncmp((char*)user_cred_get, (char*)tmp_web_name, *size)) {
		memcpy(ret_cred_web, web_name, *size);
		memcpy(ret_cred_uname, web_uname, *size);
		memcpy(ret_cred_pword, web_pword, *size);
		hmac_xcrypt(ret_cred_web, iv_in, master_key, *size);
		hmac_xcrypt(ret_cred_uname, iv_in, master_key, *size);
		hmac_xcrypt(ret_cred_pword, iv_in, master_key, *size);
		*found = 1;
	}
	else {
		*found = 0;
	}
}

