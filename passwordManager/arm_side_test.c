#ifndef USERCLASS_H
#define USERCLASS_H
#include "userclass.h"
#endif

// #ifndef MBSIDE_H
// #define MBSIDE_H
// #include "mbside.h"
// #endif
//include "arm_protocol_header.h"
//include "enclave_library.h"

#ifndef VAULT_H
#define VAULT_H
#include "vault.h"
#endif

#ifndef PTHREAD_H
#define PTHREAD_H
#include <pthread.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/time.h>

#define M_PASSWORD ((const unsigned char *) "B8D:09m-?<yvz[C|NM-?l-b!%rqoyvNL3'T8l7`v9Z^_vJK/9IE171<_#Qkwhe-1{WZ[T:v.mib80iTzc-0R^K{/u!jZ$=Q*4-cBDcb>(zHgz8ZY0_2Zu[?,p|g8P@3sf0n;E?T--LMw)PEMODkY9O}ZZz1$8WfL&(~2x-Y`F%v.R5woNW$gN>_9WV[4d1qZ3yH_;?ZzIK%KAn~q[..dUUbK-Y7}+|#>A~W_zCCVCOtW7}|O*qs-3xtK)u>lE`rz\0");

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
	//printf("accounts in vault: %d\n", vault->num_accounts);
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

bool vault_store_user(vault *vault, unsigned char *store, unsigned int *size) {
    unsigned int i;
	if(vault->full == true) {
		return false;
	}
	//printf("num users in vault: %d\n", vault->num_users);
	for(i = 0; i < *size; i++) {
		vault->m_pword[i] = store[i];
	}
	vault->full = true;
	//printf("vault_store_user: uname in vault: %s\n", vault->user_store[vault->num_users].m_uname);
	//printf("vault_store_user: pword in vault: %s\n", vault->m_pword);
	//printf("num users in vault after store: %d\n", vault->num_users);
	return true;
}

void create_account(unsigned char *new_user) {
	unsigned char password[BUFF_SIZE] = {"U1TonO9yhUbzfi2WvCjXn4YIHV37vh7PolEI5UcMsT40wbiUDkLWPTFJWUXPgDn2vbToBqIxyHKWCul66pr48AliAwr8qEuSWrwpOPlhRK1UuPfcHY7B5pZR0UX4gUPjoJLx74VOQwXGp3wJXQLHLT2ttkpyTe7teXMCXfDbfko3a3258CQpbxqB79mqkOnDQanZrY9DXwfmw7TEqNUuToWODIBFtWJebY3gkYTAKYsJhvIEP9MtMrO3i24XnB29\0"};
	for(unsigned int i=0; i < 256; i++) {
		new_user[i] = password[i];
		//new_user[i] = M_PASSWORD[i];
	}
	//printf("new user: %s\n", password);
	//printf("Create account\n");
	//printf("Enter Master Password: ");
	//scanf("%s",new_user);
	//getchar();
	//printf("user_store: pword: %s\n", new_user);
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
		//printf("Add web account credentials or get web account credentials (a/g): ");
		printf("Add or get web: ");
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

void add_credentials(unsigned char *new_cred_web, unsigned char *new_cred_uname,
	unsigned char *new_cred_pword) {
	//printf("Enter website name: ");
	scanf("%s",new_cred_web);
	getchar();
	//printf("Enter website username: ");
	scanf("%s",new_cred_uname);
	getchar();
	//printf("Enter website password: ");
	scanf("%s",new_cred_pword);
	getchar();
}

void get_credentials(unsigned char *get_cred) {
	//printf("Enter website name to get credentials: ");
	scanf("%s",get_cred);
	getchar();
}

void thread_join(unsigned int *done_flag) {
	while (done_flag == 0)
		continue;
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

char ask_user_c_l() {
	printf("Create account or login? (C/L): ");
	char c;
	scanf(" %c", &c);
	getchar();
	return c;
}

/*** 
 *** Function takes new user master password, enrypts, and stores in vault 
 ***/
uint8_t case_C_encrypt_and_write(unsigned int *done_flag, unsigned char *create_pw, 
	unsigned int *size, unsigned char *cipher_pw) {
	done_flag = 0;
	create_user(create_pw, size, cipher_pw, &done_flag);

	thread_join(&done_flag);
	vault vault;
	if(!read_vault(&vault)) {
		vault.num_accounts = 0;
		vault.full = false;
	}
	if(!vault_store_user(&vault, cipher_pw, size)) //must wait for lock before calling this
		return 0;
	write_vault(&vault);
	return 1;
}

/*** 
 *** Function checks to see if entered master password matches the encrypted
 *** one stored in the vault (authenticates user) 
 ***/
unsigned int login_check_user(unsigned int *done_flag, unsigned char *login_attempt, 
	unsigned int *size, unsigned char *m_pword, unsigned int *found) {
	//login(login_attempt);
	//printf("login attempt: %s\n", login_attempt);
	*done_flag = 0;
	check_user(login_attempt, size, m_pword, &found, &done_flag);
	thread_join(&done_flag);
	return found;
}

/*** 
 *** Function checks to see if desired credentials exist in the vault
 *** Credentials are returned to user (can be accessed from main)
 *** Should take longer if more credentials are in vault (O(n)) 
 ***/

bool k1_get_cred_check(unsigned char *user_cred_get, 
	unsigned char *user_ret_web, unsigned char *user_ret_uname, 
	unsigned char *user_ret_pword, unsigned int *size, unsigned int *done_flag) {
	uint32_t cred_found = false;
	unsigned int i;
	vault vault;
	read_vault(&vault);
	for(i=0; i<vault.num_accounts; i++) {
		decrypt_and_check_for_web_credentials(vault.accounts[i].web_name,
			user_cred_get, size, &cred_found, &done_flag);
		thread_join(&done_flag);
		done_flag = 0;
		if(cred_found)
			break;
	}
	if(cred_found) {
		return_credentials(vault.accounts[i].web_name,
			vault.accounts[i].credentials.a_uname,
			vault.accounts[i].credentials.a_pword,
			size,
			user_ret_web, user_ret_uname, user_ret_pword,
			&done_flag);
		thread_join(&done_flag);
		done_flag = 0;
		return 1;
	}
	return 0;
}

/*** 
 *** Function takes a new set of credentials from user, encrypts them,
 *** and stores them in the vault.
 ***/
bool k2_add_cred_encrypt_write_vault(unsigned char *user_add_web,
	unsigned char *user_add_uname, unsigned char *user_add_pword,
	unsigned char *encrypted_user_cred_web, unsigned char *encrypted_user_cred_uname,
	unsigned char *encrypted_user_cred_pword, unsigned int *size, 
	vault *vault, unsigned int *done_flag) {
	if(vault->num_accounts < MAX_ACCOUNTS) {
		done_flag = 0;
		encrypt_credentials(user_add_web, user_add_uname, user_add_pword, size,
			encrypted_user_cred_web, encrypted_user_cred_uname,
			encrypted_user_cred_pword, &done_flag);
		thread_join(&done_flag);
		done_flag = 0;
		
		strncpy(vault->accounts[vault->num_accounts].web_name, encrypted_user_cred_web, *size);
		strncpy(vault->accounts[vault->num_accounts].credentials.a_uname, encrypted_user_cred_uname, *size);
		strncpy(vault->accounts[vault->num_accounts].credentials.a_pword, encrypted_user_cred_pword, *size);
		memset(encrypted_user_cred_web,0,*size);
		memset(encrypted_user_cred_uname,0,*size);
		memset(encrypted_user_cred_pword,0,*size);
		vault->num_accounts++;
		write_vault(vault);
		return 1;
	}
	return 0;
}

int main(int argc, char** argv) {
	//enclave_init_with_file("password_manager.bin");
	create_vault();
	vault vault;
	uint32_t a = BUFF_SIZE;
	uint32_t *size = &a;
	unsigned int done_flag;
	while (1) {
		if(!read_vault(&vault)) {
			vault.num_accounts = 0;
			vault.full = false;
		}
		unsigned char create_pw[BUFF_SIZE];
		unsigned char cipher_pw[BUFF_SIZE];
		unsigned char login_attempt[BUFF_SIZE];
		unsigned char login_password_test[BUFF_SIZE];
		char c = ask_user_c_l();
		switch(c) {
			case 'c':
			case 'C': {
				if(vault.full == true) {
					printf("Vault full\n");
					break;
				}
				create_account(create_pw);							/******** User input ********/
				//printf("password: %s\n",create_pw);
	//------>	/********** Store user master password in vault **********/
				//Begin time here
				
				for(int i=0;i<BUFF_SIZE;i++) {
					login_password_test[i] = create_pw[i];
				}
				//printf("login_password_test: %s\n", login_password_test);
				uint8_t status = case_C_encrypt_and_write(&done_flag, create_pw, size, cipher_pw);
				//end time here
				
				if(!status)
					printf("Error adding acount: max number of users (%d) reached\n", MAX_ACCOUNTS);
				break;
			}
			case 'l':
			case 'L': {

	//------>	/********** Check if master password matches **********/
				//begin time here
				//unsigned int found = login_check_user(&done_flag, 
				//	login_attempt, size, vault.m_pword, &found);
				unsigned int found = login_check_user(&done_flag, 
					login_password_test, size, vault.m_pword, &found);
				//end time here
				
				if(found) {
					while(1) {
						//uint32_t k = get_or_add();
						bool success;
						unsigned char user_cred_get[BUFF_SIZE];
						done_flag = 0;

						unsigned char user_add_web[BUFF_SIZE];
						unsigned char user_add_uname[BUFF_SIZE];
						unsigned char user_add_pword[BUFF_SIZE];
						unsigned char encrypted_user_cred_web[BUFF_SIZE];
						unsigned char encrypted_user_cred_uname[BUFF_SIZE];
						unsigned char encrypted_user_cred_pword[BUFF_SIZE];
						if(vault.num_accounts < MAX_ACCOUNTS)
							add_credentials(user_add_web, user_add_uname, user_add_pword);						/******** User input ********/
						
//------>				/********** Add Credentials **********/
						//begin time here
						clock_t begin = clock();
						success = k2_add_cred_encrypt_write_vault(user_add_web, user_add_uname,
							user_add_pword, encrypted_user_cred_web, encrypted_user_cred_uname,
							encrypted_user_cred_pword, size, &vault, &done_flag);
						clock_t end = clock();
						//end time here

						printf("exec_time_add: %f\n", ((double)end-begin)/CLOCKS_PER_SEC);

						if(success)
							printf("success\n");
						else
							printf("error\n");


						/*
						if(success)
							printf("Successfully added credentials\n");
						else
							printf("Max number of credentials (%d) already added.\n", MAX_ACCOUNTS);
						*/
						

						unsigned char user_ret_web[BUFF_SIZE];
						unsigned char user_ret_uname[BUFF_SIZE];
						unsigned char user_ret_pword[BUFF_SIZE];
						get_credentials(user_cred_get);						/******** User input ********/
						
//------>				/********** Access Credentials **********/
						//begin time here
						begin = clock();
						success = k1_get_cred_check(user_cred_get, 
							user_ret_web, user_ret_uname, user_ret_pword, 
							size, &done_flag);
						end = clock();
						//end time here
						
						printf("exec_time_get: %f\n", ((double)end-begin)/CLOCKS_PER_SEC);

						if(success) 
							printf("found\n");
						else 
							printf("not_present\n");
						

						/*if(success) {
							printf("Credentials found for \"%s\".\nUsername: %s\nPassword: %s\n",
								user_ret_web, user_ret_uname, user_ret_pword);
						}
						else {
							printf("No credentials for the website (%s) found!\n", user_cred_get);
						}*/
						
					
					}
				}
				else {
					printf("Invalid username or password\n");
				}
				break;
			}
			default:
				printf("Bad input. try again\n");
				break;
		}
	}
}