#include "unistd.h"
#include <sys/syscall.h>

#ifndef USERCLASS_H
#define USERCLASS_H
#include "userclass.h"
#endif

#ifndef MBSIDE_H
#define MBSIDE_H
#include "mbside.h"
#endif
//#include "arm_protocol_header.h"
//#include "enclave_library.h"

#ifndef VAULT_H
#define VAULT_H
#include "vault.h"
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/time.h>

#define ITERATIONS 25

void print_vault(vault *vault) {
    int i;
	printf("Vault: num_users: %d\n", vault->num_accounts);
	printf("Vault master password: %s\n", vault->m_pword);
	for(i=0; i < MAX_ACCOUNTS; i++) {
		printf("Website: %s\n", vault->accounts[i].web_name);
		printf("Username: %s\n", vault->accounts[i].credentials.a_uname);
		printf("Password: %s\n", vault->accounts[i].credentials.a_pword);
	}
}

bool write_vault(vault *vault) {
	printf("accounts in vault: %d\n", vault->num_accounts);
	FILE *f = fopen("test.dat", "w+"); //will need to be changed to w+
	if(f == NULL) {
		fprintf(stderr, "\nError in file open (write)\n");
		exit(1);
	}
	fwrite(vault, sizeof(struct vault), 1, f);
	fclose(f);
	return true;
}

bool read_vault(vault *vault) {
	FILE *infile;
	infile = fopen("test.dat","r");
	if(infile == NULL) {
		fprintf(stderr, "\nError in file open (read)\n");
		exit(1);
	}
	if(!fread(vault, sizeof(struct vault), 1, infile)) {
		fclose(infile);
		return false; //file is emtpy
	}
	//printf("vault user: %s\n", vault->user_store[vault->num_users].m_uname);
	fclose(infile);
	//vault.full = true;
	return true;
}

bool vault_store_user(vault *vault, unsigned char *store, unsigned int *size, uint8_t *master_iv) {
    unsigned int i;
	if(vault->full == true) {
		return false;
	}
	//printf("create_pw_in_vault_store_user: %s\n", store);
	//printf("num users in vault: %d\n", vault->num_users);
	for(i = 0; i < *size; i++) {
		vault->m_pword[i] = store[i];
	}
	memcpy(vault->m_iv, master_iv, IV_SIZE);
	vault->full = true;
	//printf("vault_store_user: uname in vault: %s\n", vault->user_store[vault->num_users].m_uname);
	//printf("vault_store_user: pword in vault: %s\n", vault->m_pword);
	//printf("num users in vault after store: %d\n", vault->num_users);
	return true;
}

void create_account(unsigned char *new_user) {
	/*unsigned int seed, i;
	if(syscall(SYS_getrandom, (unsigned char*)(&seed), 4, 0) < 0){
    	fprintf(stderr, "Error getting random seed\n");
		return;
  	}*/

	//srand(time(NULL));
	unsigned int i;
	for(i=0; i < 256; i++) {
//		new_user[i] = password[i];
		new_user[i] = (unsigned char)(rand());
	}
}

void create_vault() {
	FILE *f;
	f = fopen("test.dat","a+");
	if(f == NULL) {
		fprintf(stderr, "\nError in file open here(read)\n");
		exit(1);
	}
	fclose(f);
}

void gen_random_password(unsigned char *s, const unsigned int len) {
  	int i;
	static const char alphanum[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_";
	for (i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    s[len] = 0;
}

int main(int argc, char** argv) {
	int i,k, itr;
	//enclave_init_with_file("password_manager.bin");
	remove("test.dat");
	time_t t;
	/*unsigned int seed;
	if(syscall(SYS_getrandom, (unsigned char*)(&seed), 4, 0) < 0){
    	fprintf(stderr, "Error getting random seed\n");
    	return -1;
  	}*/
  	srand(time(NULL));
	create_vault();
	vault vault;
	uint32_t a = BUFF_SIZE;
	uint32_t *size = &a;
	if(!read_vault(&vault)) {
		vault.num_accounts = 0;
		vault.full = false;
	}
	unsigned char create_pw[BUFF_SIZE];
	unsigned char cipher_pw[BUFF_SIZE];
	unsigned char login_attempt[BUFF_SIZE];
	unsigned char login_password_test[BUFF_SIZE];
	uint8_t iv_in[IV_SIZE + 1];
	uint8_t iv_out[IV_SIZE + 1];
	uint8_t master_iv_out[IV_SIZE + 1];
	
	if(vault.full == true) {
		printf("Vault full\n");
		return 0;
	}
	gen_random_password(create_pw, BUFF_SIZE);
	memcpy(login_password_test, create_pw, BUFF_SIZE);
	create_user(create_pw, size, iv_in, cipher_pw, master_iv_out);

	if(!vault_store_user(&vault, cipher_pw, size, master_iv_out)) {
		printf("bad store!\n");
		exit(1);
	}

	unsigned int found = false;

	check_user(login_password_test, size, vault.m_pword, vault.m_iv, &found);
	if(!found)
		return 0;
	printf("Found!\n");

	/******** AFTER LOGIN ********/
	clock_t begin, end;	
	unsigned char current_web[BUFF_SIZE];
	unsigned char current_user[BUFF_SIZE];
	unsigned char current_password[BUFF_SIZE];
	unsigned char tmp_name[BUFF_SIZE];
	website encrypted_user_cred;
	website user_ret;	
	unsigned int loop_count = 0;
	double create_time, read_time;
	printf("PASSWORD_SIZE,ADD_TIME,GET_TIME\n");
	for(k = 4; k < BUFF_SIZE; k+=4) {
		for(itr = 0; itr < ITERATIONS; itr++) {
			memset(current_web, 0, BUFF_SIZE);
	        memset(tmp_name, 0, BUFF_SIZE);
	        memset(current_user, 0, BUFF_SIZE);
	        memset(current_password, 0, BUFF_SIZE);
	        sprintf((char*)current_web, "test%d_%d", itr,k);
			sprintf((char*)tmp_name, "test%d_%d", itr,k);
			sprintf((char*)current_user, "user%d_%d", itr,k);
			gen_random_password(current_password, k);

			bool success;
			uint32_t cred_found = false;
			if(vault.num_accounts < MAX_ACCOUNTS) {
				begin = clock();
				encrypt_credentials(current_web, current_user,
					current_password, size, iv_in,
					encrypted_user_cred.web_name,
					encrypted_user_cred.credentials.a_uname,
					encrypted_user_cred.credentials.a_pword,
					iv_out);
				memcpy(encrypted_user_cred.web_iv, iv_out, IV_SIZE);
				vault.accounts[vault.num_accounts] = encrypted_user_cred;
				vault.num_accounts++;
				write_vault(&vault);
				end = clock();
				create_time = ((double)end-begin)/CLOCKS_PER_SEC;
			
				unsigned int i;
				begin = clock();

				for(i=0; i<vault.num_accounts; i++) {
					check_and_return_credentials(vault.accounts[i].web_name,
						vault.accounts[i].credentials.a_uname,
						vault.accounts[i].credentials.a_pword,
						vault.accounts[i].web_iv, tmp_name, size,
						user_ret.web_name, user_ret.credentials.a_uname,
						user_ret.credentials.a_pword, &cred_found);
					if(cred_found)
						break;
				}
				end = clock();
				read_time = ((double)end-begin)/CLOCKS_PER_SEC;
				if(!cred_found) {
					fprintf(stderr, "ERROR: Data not found\n");
					return 0;
				}
				printf("%d,%f,%f\n", k, create_time, read_time);
			}
		}
	}
}