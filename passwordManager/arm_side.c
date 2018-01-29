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

/*
#ifndef PTHREAD_H
#define PTHREAD_H
#include <pthread.h>
#endif
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

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
	//printf("num users in vault: %d\n", vault->num_users);
	for(i = 0; i < *size; i++) {
		vault->m_pword[i] = store[i];
	}
	memcpy(vault->m_iv, master_iv, IV_SIZE);
	vault->full = true;
	//printf("vault_store_user: uname in vault: %s\n", vault->user_store[vault->num_users].m_uname);
	printf("vault_store_user: pword in vault: %s\n", vault->m_pword);
	//printf("num users in vault after store: %d\n", vault->num_users);
	return true;
}

void create_account(unsigned char *new_user) {
	printf("Create account\n");
	printf("Enter Master Password: ");
	scanf("%s",new_user);
	getchar();
	printf("user_store: pword: %s\n", new_user);
	//printf("new_user: uname: %s, pword: %s\n", new_user->m_uname, new_user->m_pword);
}

void login(unsigned char *login_attempt) {
	printf("Login to account.\n");
	printf("Enter master password: ");
	scanf("%s", login_attempt);
	getchar();
}

uint32_t get_or_add() {
	char c;
	while(1) {
		printf("Add web account credentials or get web account credentials (a/g): ");
		scanf(" %c", &c);
		getchar();
		switch(c) {
			case 'g':
			case 'G':
				return 1;
				break;
			case 'a':
			case 'A':
				return 2;
			default:
				printf("Unrecognized command.  Try again.\n");
				break;
		}
	}
}

void add_credentials(website *new_cred) {
	printf("Enter website name: ");
	scanf("%s",new_cred->web_name);
	getchar();
	printf("Enter website username: ");
	scanf("%s",new_cred->credentials.a_uname);
	getchar();
	printf("Enter website password: ");
	scanf("%s",new_cred->credentials.a_pword);
	getchar();
}

void get_credentials(unsigned char *get_cred) {
	printf("Enter website name to get credentials: ");
	scanf("%s",get_cred);
	getchar();
}

//void thread_join(unsigned int *done_flag) {
//	while (done_flag == 0)
//		continue;
	/*
	pthread_mutex_lock(&(lock->m));
 	while (lock->done == 0)
  		pthread_cond_wait(&(lock->w), &(lock->m));
  	pthread_mutex_unlock(&(lock->m));
  	*/
//}

void create_vault() {
	FILE *f;
	f = fopen("test.dat","a+");
	if(f == NULL) {
		fprintf(stderr, "\nError in file open here(read)\n");
		exit(1);
	}
	fclose(f);
}

int main(int argc, char** argv) {
	//locks lock;
	//pthread_mutex_t m;								//40 bytes
	//pthread_cond_t w = PTHREAD_COND_INITIALIZER;
	//enclave_init_with_file("password_manager.bin");
	//lock.m = m;
	//lock.w = w;
	create_vault();
	vault vault;
	uint32_t a = BUFF_SIZE;
	uint32_t *size = &a;
	unsigned int done_flag;
	while (1) {
		if(!read_vault(&vault)) {
			vault.num_accounts = 0;
			vault.full = false;
			//printf("Vault empty\n");
		}
		unsigned char create_pw[BUFF_SIZE];
		unsigned char cipher_pw[BUFF_SIZE];
		unsigned char login_attempt[BUFF_SIZE];
		uint8_t master_iv_in[IV_SIZE + 1];
		uint8_t master_iv_out[IV_SIZE + 1];
		printf("Create account or login? (C/L): ");
		char c;
		scanf(" %c", &c);
		getchar();
		switch(c) {
			case 'c':
			case 'C': {
				if(vault.full == true) {
					printf("Error adding acound: max number of users (%d) reached\n", MAX_USERS);
					break;
				}
				create_account(create_pw);
				done_flag = 0;
				create_user(create_pw, size, master_iv_in, cipher_pw, &done_flag, master_iv_out);
				printf("after create user_store\n");
				//thread_join(&done_flag);
				if(!vault_store_user(&vault, cipher_pw, size, master_iv_out)) { //must wait for lock before calling this
					printf("Error adding acound: max number of users (%d) reached\n", MAX_USERS);
				}
				else {
					printf("Successfully added user\n");
				}
				write_vault(&vault);
				break;
			}
			case 'l':
			case 'L': {
					printf("login: Vault stored pword: %s\n", vault.m_pword);
					login(login_attempt);
					unsigned int found = false;
					done_flag = 0;
					check_user(login_attempt, size, vault.m_pword, vault.m_iv, &found, &done_flag);
					printf("after check user: Vault stored pword: %s\n", vault.m_pword);
					//thread_join(&done_flag);
					if(found) {
						printf("User found!\n");
						while(1) {
							uint32_t k = get_or_add();
							unsigned char user_cred_get[BUFF_SIZE];
							done_flag = 0;
							if(k == 1) {
								website user_ret;
								unsigned char ret_cred_web[BUFF_SIZE];
								unsigned char ret_cred_uname[BUFF_SIZE];
								unsigned char ret_cred_pword[BUFF_SIZE];
								uint32_t cred_found = false;
								printf("Get credentials\n");
								get_credentials(user_cred_get);
								unsigned int i;
								for(i=0; i<vault.num_accounts; i++) {
									decrypt_and_check_for_web_credentials(vault.accounts[i].web_name,
										user_cred_get, size, &cred_found, &done_flag);
									//thread_join(&done_flag);
									done_flag = 0;
									if(cred_found)
										break;
								//printf("end: Vault stored pword: %s\n", vault.m_pword);
								}
								if(cred_found) {
									return_credentials(vault.accounts[i].web_name,
										vault.accounts[i].credentials.a_uname,
										vault.accounts[i].credentials.a_pword,
										size,
										user_ret.web_name, user_ret.credentials.a_uname,
										user_ret.credentials.a_pword,
										&done_flag);
									//thread_join(&done_flag);
									done_flag = 0;
									printf("Credentials found for \"%s\".\nUsername: %s\nPassword: %s\n",
										user_ret.web_name, user_ret.credentials.a_uname,
										user_ret.credentials.a_pword);
								}
								else {
									printf("No credentials for the website (%s) found!\n", user_cred_get);
								}
							}
							else if(k == 2) {
								printf("Vault stored pword: %s\n", vault.m_pword);
								website user_add;
								website encrypted_user_cred;
								printf("Add credentials\n");
								if(vault.num_accounts < MAX_ACCOUNTS) {
									add_credentials(&user_add);
									done_flag = 0;
									encrypt_credentials(user_add.web_name, user_add.credentials.a_uname,
										user_add.credentials.a_pword, size,
										encrypted_user_cred.web_name,
										encrypted_user_cred.credentials.a_uname,
										encrypted_user_cred.credentials.a_pword,
										&done_flag);
									//thread_join(&done_flag);
									done_flag = 0;
									vault.accounts[vault.num_accounts] = encrypted_user_cred;
									vault.num_accounts++;
									write_vault(&vault);
									printf("user_account info. website name: %s, web uname: %s, web pword %s\n",
										vault.accounts[vault.num_accounts-1].web_name,
										vault.accounts[vault.num_accounts-1].credentials.a_uname,
										vault.accounts[vault.num_accounts-1].credentials.a_pword);
								}
								else {
									printf("Max number of credentials (%d) already added.\n", MAX_ACCOUNTS);
								}
							}
							else {
								printf("Error: Should not have made it here\n");
							}
						}
					}
					else {
						printf("Invalid username or password\n");
					}
				break;
			}
			//For debugging
			case 'p':
			case 'P': {
				print_vault(&vault);
				break;
			}
			default:
				printf("Bad input. try again\n");
				break;
		}
	}
	printf("End program\n");
}
