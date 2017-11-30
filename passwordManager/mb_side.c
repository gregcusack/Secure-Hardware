#include <stdint.h>
#include <string.h>
#include <stdbool.h>

// #include "microblaze_protocol_header.h"
// #include "userclass.h"

#define KEY ((const unsigned char *) "aGkecI6VuFTecB4hpTC9YLNjwkvFOMos9qGjulqlRhPBdysglPOhl4O077S5iei7kHx4mrqyw6WauleRgMvl2gXW1nUkLWzh0C8R8C0x6hRIhJNkOOVDhQjtuCUQd718IqxvcEsODa5Zu9TbkB2RcDMcAoXPruJkf4fwYwO4QqEP3NlqE2jCr5qi70pmePUIijBY2vVPJDdGj1lIjTplHmgf0ZHhBN75nD1yWutGLcmDwfS4dd8idwoM2yxMOz8n\0")
									  
void encrypt_m_pword(unsigned char *input_user, unsigned int size, unsigned char *ret_user) {
	//printf("encrypt this: %s\n", input_user);
	//printf("size: %d\n", size);
	for(unsigned int i=0; i < size - 1; i++) {
		ret_user[i] = input_user[i]^KEY[i];
	}
	ret_user[size-1] = '\0';
	//printf("encrypted: %s\n", ret_user);
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
	uint8_t x = 1;
	for(unsigned int j=0; j < *size; j++) {
		decrypted[j] = web_name[j]^KEY[j];
	}
	unsigned int i;
	for(i=0;i<*size;i++) {
		if(decrypted[i] != user_cred_get[i])
			break;
	}
	if(i != *size)
		*cred_found = 0;
	else
		*cred_found = 1;
	/*
	if(!strcmp((char*)decrypted, (char*)user_cred_get)) {
		*cred_found = 1;
	}
	*/
	//else {
	//	*cred_found = 0;
	//}
	*done_flag = 1;
}

//return cipher_web_name/uname/pword/done_flag
void encrypt_credentials(unsigned char *web_name, unsigned char *a_uname,
	unsigned char *a_pword, unsigned int *size, unsigned char *cipher_web_name,
	unsigned char *cipher_a_uname, unsigned char *cipher_a_pword,
	unsigned int *done_flag) {
	printf("web_name: %s\n", web_name);
	for(unsigned int i=0; i < *size; i++) {
		cipher_web_name[i] = web_name[i]^KEY[i];
		cipher_a_uname[i] = a_uname[i]^KEY[i];
		cipher_a_pword[i] = a_pword[i]^KEY[i];
	}
	printf("encrypted: %s\n", cipher_web_name);
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
