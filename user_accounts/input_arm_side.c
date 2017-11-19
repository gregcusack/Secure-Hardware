#ifndef USERCLASS_H
#define USERCLASS_H
#include "userclass.h"
#endif

#ifndef MBSIDE_H
#define MBSIDE_H
#include "mbside.h"
#endif

#ifndef VAULT_H
#define VAULT_H
#include "vault.h"
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

bool store_user(user_account *input) {
	FILE *f = fopen("test.dat", "ab+"); //will need to be changed to w+
	if(f == NULL) {
		fprintf(stderr, "\nError in file open (write)\n");
		exit(1);
	}
	fwrite(input, sizeof(struct user_account), 1, f);
	if(fwrite != 0) {
		printf("Account added\n");
	}
	else {
		printf("Error writing account\n");
		exit(1);
	}
	fclose(f);
	return true;
}

bool get_user(user_account *input, user_account *output) {
	FILE *infile;
	infile = fopen("test.dat","r");
	if(infile == NULL) {
		fprintf(stderr, "\nError in file open (read)\n");
		exit(1);
	}
	bool found_flag = false;
	while(fread(output, sizeof(struct user_account), 1, infile)) {
		if(!(strcmp(input->m_uname,output->m_uname))) {
			found_flag = true;
			break;
		}
	}
	if(found_flag)
		return true;
	return false;
}

uint32_t get_user_from_vault(vault *m_vault, user_account *login) {
	for(int i=0; i < m_vault->num_users; i++) {
		if(!strcmp(m_vault->user_store[i].m_uname,login->m_uname)) {
			return i;
		}
	}
	return -1;
}

void create_web_login(user_account *user) {
	printf("Create login\n");
	website new_web_login;
	printf("Enter website name: ");
	scanf("%s",new_web_login.web_name);
	getchar();
	printf("Enter website username: ");
	scanf("%s", new_web_login.credentials.a_uname);
	getchar();
	printf("Enter website password: ");
	scanf("%s", new_web_login.credentials.a_pword);
	getchar();
	user->accounts[user->num_accounts] = new_web_login;
	user->num_accounts++;
	//send to MB to encrypt
	//return encrypted from MB
	//Store in Vault

}


int main(int argc, char** argv) {
	vault vault;
	vault.num_users = 0;
	uint32_t a = BUFF_SIZE;
	uint32_t *size = &a;
	while (1) {
		printf("Create account or login? (C/L): ");
		char c;
		scanf(" %c", &c);
		getchar();
		if(c != 'C' && c != 'c' && c != 'L' && c != 'l') {
			printf("Bad input, run again\n");
			exit(1);
		}
		else if(c == 'C' || c == 'c') {
			user_account new_user;  //may need to malloc this bad boy
			printf("Create account!\n");
			printf("Enter username: ");
			scanf("%s",new_user.m_uname);
			getchar();
			printf("Enter password: ");
			scanf("%s",new_user.m_pword);
			getchar();
			new_user.num_accounts = 0;
			user_account user_store;
			create_user(&new_user, size, &user_store);
			vault.user_store[vault.num_users] = user_store;		//store user in vault (may need to be own function)
			vault.num_users++;
			//store_user(&user_store);
			return 0;
		}
		else if(c == 'L' || c == 'l') {
			user_account login, user_from_db;
			printf("Login to account.\n");
			printf("Enter Username: ");
			scanf("%s",login.m_uname);
			getchar();
			printf("Enter password: ");
			scanf("%s", login.m_pword);
			getchar();
			uint32_t user_index = get_user_from_vault(&vault, &login);
			if(user_index >= 0) {
				uint32_t found = false;
				check_user(&vault.user_store[user_index], size, &login, &found);
				if(found) {
					printf("User found!\n");
					printf("Add login\n"); //will need to change to add or view
					create_web_login(&vault.user_store[user_index]);
					//add_login(user_account *user_data, uint32_t *pword_len,
					//user_account *cipher_account);
					//add_login()
					//option here to add uname/pword for website login
				}
				else {
					printf("Invalid username or password\n");
				}
			}
			else {
				printf("Invalid username or password\n");
			}
		}
	}
	return 0;
}