#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#ifndef MBSIDE_H
#define MBSIDE_H
#include "mbside.h"
#endif

#define KEY ((const unsigned char *) "B&/n^!v8G`3BJL:B~q`~K(y!~;SBDw0:\0")

void encrypt_m_pword(user_account *input_user, uint32_t size, user_account *ret_user) {
	for(uint8_t i=0; i < size - 1; i++) {
		ret_user->m_pword[i] = input_user->m_pword[i]^KEY[i];
		ret_user->m_uname[i] = input_user->m_uname[i];
	}
	ret_user->m_pword[size-1] = '\0';
	memset(input_user->m_pword, 0, size);
}

void decrypt_m_pword(unsigned char* cipher_p, uint32_t size, unsigned char* decrypted) {
	for(uint32_t i=0; i < size; i++) {
		decrypted[i] = cipher_p[i]^KEY[i];
	}
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

void *create_user(void *arguments) {
	thread_args *args = arguments;
	pthread_mutex_lock(&(args->lock.m));
	printf("args_user_account: %s\n", args->u_acct_t.m_uname);
	encrypt_m_pword(&args->u_acct_t, args->size_th, &args->u_store_t);
	args->lock.done = 1;
	pthread_cond_signal(&(args->lock.w));
	pthread_mutex_unlock(&(args->lock.m));
}

void *check_user(void *arguments) {
	thread_args *args = arguments;
	pthread_mutex_lock(&(args->lock.m));
	unsigned char decrypted[args->size_th];
	decrypt_m_pword(&args->u_store_t.m_pword, args->size_th, decrypted);
	if(!strcmp(args->login_t.l_pword, decrypted)) {
		args->found = true;
	}
	else {
		args->found = false;
	}
	args->lock.done = 1;
	pthread_cond_signal(&(args->lock.w));
	pthread_mutex_unlock(&(args->lock.m));
}

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
}
*/

void *encrypt_credentials(void *arguments) {
	thread_args *args = arguments;
	pthread_mutex_lock(&args->lock.m);
	for(uint8_t i=0; i < args->size_th; i++) {
		args->web_b.web_name[i] = args->web_a.web_name[i]^KEY[i];
		args->web_b.credentials.a_uname[i] = args->web_a.credentials.a_uname[i]^KEY[i];
		args->web_b.credentials.a_pword[i] = args->web_a.credentials.a_pword[i]^KEY[i];
	}
	args->web_b.web_name[args->size_th-1] = '\0';
	args->web_b.credentials.a_uname[args->size_th-1] = '\0';
	args->web_b.credentials.a_pword[args->size_th-1] = '\0';
	memset(args->web_a.web_name,0,args->size_th);
	memset(args->web_a.credentials.a_uname,0,args->size_th);
	memset(args->web_a.credentials.a_pword,0,args->size_th);
	args->lock.done = 1;
	pthread_cond_signal(&args->lock.w);
	pthread_mutex_unlock(&args->lock.m);
}
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
void *return_credentials(void *arguments) {
	thread_args *args = arguments;
	pthread_mutex_lock(&(args->lock.m));
	unsigned char decrypted[args->size_th];
	for(uint8_t i=0; i < args->u_store_t.num_accounts; i++) {
		for(uint8_t j=0; j < args->size_th; j++) {
			decrypted[j] = args->u_store_t.accounts[i].web_name[j]^KEY[j];
		}
		if(!strcmp(decrypted, args->web_a.web_name)) {
			for(uint8_t k=0;k < args->size_th; k++) {
				args->web_b.web_name[k] = decrypted[k];
				args->web_b.credentials.a_uname[k] = args->u_store_t.accounts[i].credentials.a_uname[k]^KEY[k];
				args->web_b.credentials.a_pword[k] = args->u_store_t.accounts[i].credentials.a_pword[k]^KEY[k];
			}
			args->found = true;
			args->lock.done = 1;
			pthread_cond_signal(&args->lock.w);
			pthread_mutex_unlock(&args->lock.m);
			break;
		}
	}
	if(!args->found) {
		args->found = false;
		args->lock.done = 1;
		pthread_cond_signal(&args->lock.w);
		pthread_mutex_unlock(&args->lock.m);
	}
}

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