#include <stdio.h>
#ifndef USERCLASS_H
#define USERCLASS_H
#include "userclass.h"
#endif

#define CBC 1
#define CTR 1
#define ECB 0

#ifndef AES_H
#define AES_H
#include "aes.h"
#endif

#include "hmac_sha2.h"

void create_user(unsigned char *create_pw, unsigned int *size, 
	unsigned char *master_iv_in, unsigned char *cipher_pw, unsigned char *master_iv_out);

//return found, done flag
void check_user(unsigned char *login_attempt, unsigned int *size,
	unsigned char *cipher_data, unsigned char *master_iv, unsigned int *found);

//return cipher_web_name/uname/pword/done_flag
void encrypt_credentials(unsigned char *create_pw, unsigned char *web_name, unsigned char *a_uname,
	unsigned char *a_pword, unsigned int *size, unsigned char *iv_in, unsigned char *cipher_web_name,
	unsigned char *cipher_a_uname, unsigned char *cipher_a_pword, unsigned char *iv_out);

void check_and_return_credentials(unsigned char *create_pw, unsigned char *web_name, 
	unsigned char *web_uname, unsigned char *web_pword,
	unsigned char *iv_in, unsigned char *user_cred_get, unsigned int *size, 
	unsigned char *ret_cred_web, unsigned char *ret_cred_uname,
	unsigned char *ret_cred_pword, unsigned int *found);