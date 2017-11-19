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

bool vault_store_user(vault *vault, user_account *store) {
	if(vault->num_users >= MAX_USERS) {
		return false;
	}
	vault->user_store[vault->num_users] = *store;
	printf("uname in vault: %s\n", vault->user_store[vault->num_users].m_uname);
	printf("pword in vault: %s\n", vault->user_store[vault->num_users].m_pword);
	vault->num_users++;
	return true;
}

int32_t search_vault(vault *vault, login_struct *login_attempt) {
	if(vault->num_users == 0) {
		return -1;
	}
	for(uint32_t i = 0; i < vault->num_users; i++) {
		if(!strcmp(login_attempt->l_uname, vault->user_store[i].m_uname)) {
			return i;
		}
	}
	return -1;
}

void create_account(user_account *new_user, uint32_t *size, user_account *user_store) {
	printf("Create account!\n");
	printf("Enter username: ");
	scanf("%s",new_user->m_uname);
	getchar();
	printf("Enter password: ");
	scanf("%s",new_user->m_pword);
	getchar();
	new_user->num_accounts = 0;
	create_user(new_user, size, user_store); //need lock
	printf("user_store: uname: %s, pword: %s\n", user_store->m_uname, user_store->m_pword);
	printf("new_user: uname: %s, pword: %s\n", new_user->m_uname, new_user->m_pword);
}

void login(login_struct *login_attempt) {
	printf("Login to account.\n");
	printf("Enter Username: ");
	scanf("%s",login_attempt->l_uname);
	getchar();
	printf("Enter password: ");
	scanf("%s", login_attempt->l_pword);
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

void get_credentials(website *get_cred) {
	printf("Enter website name for your respective login credentials: ");
	scanf("%s",get_cred->web_name);
	getchar();
}


int main(int argc, char** argv) {
	vault vault;
	vault.num_users = 0;
	uint32_t a = BUFF_SIZE;
	uint32_t *size = &a;
	while (1) {
		user_account input, user_store;
		login_struct login_attempt;
		printf("Create account or login? (C/L): ");
		char c;
		scanf(" %c", &c);
		getchar();
		switch(c) {
			case 'c':
			case 'C':
				create_account(&input, size, &user_store);
				if(!vault_store_user(&vault, &user_store)) { //must wait for lock before calling this
					printf("Error adding acound: max number of users (%d) reached\n", MAX_USERS);
				}
				else {
					printf("Successfully added user\n");
				}
				break;
			case 'l':
			case 'L': {
					login(&login_attempt);
					int32_t index = search_vault(&vault, &login_attempt);
					if(index >= 0) {
						printf("index: %d\n", index);
						uint32_t found = false;
						check_user(&login_attempt, size, &vault.user_store[index], &found); //need lock here
						if(found) {
							printf("User found!\n");
							while(1) {
								uint32_t k = get_or_add();
								website user_cred;
								if(k == 1) {
									website ret_cred;
									uint32_t cred_found = false;
									printf("Get credentials\n");
									get_credentials(&user_cred);
									return_credentials(&vault.user_store[index], &user_cred, size, &ret_cred, &cred_found);
									if(cred_found) {
										printf("Credentials found for \"%s\".\nUsername: %s\nPassword: %s\n",
											ret_cred.web_name, ret_cred.credentials.a_uname, ret_cred.credentials.a_pword);
									}
									else {
										printf("No credentials for the website (%s) found!\n", user_cred.web_name); //maybe
									}
								}
								else if(k == 2) {
									website encrypted_user_cred;
									printf("Add credentials\n");
									add_credentials(&user_cred);
									encrypt_credentials(&user_cred, size, &encrypted_user_cred); //need lock
									vault.user_store[index].accounts[vault.user_store[index].num_accounts] = encrypted_user_cred;
									vault.user_store[index].num_accounts++;
									
									printf("user_account info. website name: %s, web uname: %s, web pword %s\n", 
										vault.user_store[index].accounts[vault.user_store[index].num_accounts-1].web_name,
										vault.user_store[index].accounts[vault.user_store[index].num_accounts-1].credentials.a_uname,
										vault.user_store[index].accounts[vault.user_store[index].num_accounts-1].credentials.a_pword);
									
								}
								else {
									printf("Error: Should not have made it here\n");
								}
							}
						}
						else {
							printf("Invalid username or password\n");
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
	printf("End program\n");
}






