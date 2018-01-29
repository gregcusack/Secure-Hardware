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
#define SIZE BUFF_SIZE


uint8_t key[32] = { 0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
                        0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4 };

void gen_iv(uint8_t *_iv) {
	for(int i=0; i < 16; i++) {
		sprintf((char*)_iv + i, "%x", rand() % 16);
	}
	//printf("Hex string (iv): %s", _iv);
}

void xcrypt(unsigned char *in_str, uint8_t *_iv) {
	struct AES_ctx ctx;
	AES_init_ctx_iv(&ctx, key, _iv);
	AES_CTR_xcrypt_buffer(&ctx, in_str, SIZE);
}

void create_user(unsigned char *create_pw, unsigned int *size, uint8_t *master_iv_in, unsigned char *cipher_pw, uint8_t *master_iv_out) {
	memcpy(cipher_pw, create_pw, SIZE);
	//printf("IN: %s\n", cipher_pw);
	gen_iv(master_iv_out);
	xcrypt(cipher_pw, master_iv_out);
	//printf("here\n");
	//printf("OUT: %s\n", cipher_pw);
}

//return found
void check_user(unsigned char *login_attempt, unsigned int *size,
	unsigned char *cipher_data, uint8_t *master_iv, unsigned int *found) {
	unsigned char tmp[SIZE];
	memcpy(tmp, cipher_data, SIZE);
	printf("Login pw attempt: %s\n", login_attempt);
	printf("Login stored cipher data: %s\n", tmp);
	xcrypt(tmp, master_iv);
	printf("Decrypted stored pw: %s\n", tmp);
	for(unsigned int i = 0; i < SIZE; i++) {
		if(login_attempt[i] != tmp[i]) {
			*found = false;
			return;
		}
	}
	*found = true;
}

//return cred_found
void decrypt_and_check_for_web_credentials(unsigned char *web_name, uint8_t *iv_in,
	unsigned char *user_cred_get, unsigned int *size, unsigned int *cred_found) {
	unsigned char tmp[SIZE];
	//printf("web_name: %s\n", web_name);
	memcpy(tmp, web_name, SIZE);
	//printf("tmp: %s\n", tmp);
	xcrypt(tmp, iv_in);
	//printf("tmp after: %s\n", tmp);
	if(!strcmp((char*)tmp, (char*)user_cred_get))
		*cred_found = 1;
	else
		*cred_found = 0;
}

//return cipher_web_name/uname/pword
void encrypt_credentials(unsigned char *web_name, unsigned char *a_uname,
	unsigned char *a_pword, unsigned int *size, uint8_t *iv_in, unsigned char *cipher_web_name,
	unsigned char *cipher_a_uname, unsigned char *cipher_a_pword, uint8_t *iv_out) {
	memcpy(cipher_web_name, web_name, SIZE);
	memcpy(cipher_a_uname, a_uname, SIZE);
	memcpy(cipher_a_pword, a_pword, SIZE);
	gen_iv(iv_out);
	xcrypt(cipher_web_name, iv_out);
	xcrypt(cipher_a_uname, iv_out);
	xcrypt(cipher_a_pword, iv_out);
}

//return ret_cred_web/uname/pword
void return_credentials(unsigned char *web_name, unsigned char *a_uname,
	unsigned char *a_pword, uint8_t *iv_in, unsigned int *size,
	unsigned char *ret_cred_web, unsigned char *ret_cred_uname,
	unsigned char *ret_cred_pword) {
	memcpy(ret_cred_web, web_name, SIZE);
	memcpy(ret_cred_uname, a_uname,SIZE);
	memcpy(ret_cred_pword, a_pword, SIZE);
	xcrypt(ret_cred_web, iv_in);
	xcrypt(ret_cred_uname, iv_in);
	xcrypt(ret_cred_pword, iv_in);
}


