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

#ifndef PTHREAD_H
#define PTHREAD_H
#include <pthread.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

uint32_t done = 0;
pthread_mutex_t m ;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;
pthread_cond_t w = PTHREAD_COND_INITIALIZER;

void print_vault(vault *vault) {
	printf("Vault: num_users: %d\n", vault->num_users);
	for(int i=0; i < MAX_USERS; i++) {
		printf("Username: %s\n", vault->user_store[i].m_uname);
		printf("Password: %s\n", vault->user_store[i].m_pword);
		printf("Num accounts: %d\n", vault->user_store[i].num_accounts);
		for(int j=0; j < MAX_ACCOUNTS; j++) {
			printf("Website: %s\n", vault->user_store[i].accounts[j].web_name);
			printf("Web username: %s\n", vault->user_store[i].accounts[j].credentials.a_uname);
			printf("Web password: %s\n", vault->user_store[i].accounts[j].credentials.a_pword);
		}
	}
}

bool write_vault(vault *vault) {
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
	return true;
}

bool vault_store_user(vault *vault, user_account *store) {
	if(vault->num_users >= MAX_USERS) {
		return false;
	}
	//printf("num users in vault: %d\n", vault->num_users);
	vault->user_store[vault->num_users] = *store;
	//printf("vault_store_user: uname in vault: %s\n", vault->user_store[vault->num_users].m_uname);
	//printf("vault_store_user: pword in vault: %s\n", vault->user_store[vault->num_users].m_pword);
	vault->num_users++;
	//printf("num users in vault after store: %d\n", vault->num_users);
	return true;
}

int32_t search_vault(vault *vault, login_struct *login_attempt) {
	//printf("search vault: Num vault users: %d\n", vault->num_users);
	if(vault->num_users == 0) {
		//printf("no vault users rn.\n");
		return -1;
	}
	for(uint32_t i = 0; i < vault->num_users; i++) {
		//printf("login attempt: %s\n", login_attempt->l_uname);
		//printf("vault_store_user: %s\n", vault->user_store[i].m_uname);
		if(!strcmp(login_attempt->l_uname, vault->user_store[i].m_uname)) {
			return i;
		}
	}
	return -1;
}

void create_account(user_account *new_user) {
	printf("Create account\n");
	printf("Enter username: ");
	scanf("%s",new_user->m_uname);
	getchar();
	printf("Enter password: ");
	scanf("%s",new_user->m_pword);
	getchar();
	new_user->num_accounts = 0;
	//printf("user_store: uname: %s, pword: %s\n", user_store->m_uname, user_store->m_pword);
	//printf("new_user: uname: %s, pword: %s\n", new_user->m_uname, new_user->m_pword);
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

uint32_t get_add_or_edit() {
	char c;
	while(1) {
		printf("Add web account credentials or get web account credentials (a/g/e): ");
		scanf(" %c", &c);
		getchar();
		switch(c) {
			case 'g':
			case 'G':
				return 1;
			case 'a':
			case 'A':
				return 2;
			case 'e':
			case 'E':
				return 3;
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
	scanf("%s",get_cred->web_name);
	getchar();
}

void edit_uname_pword(website *ret_cred) {
	printf("Enter new username: ");
	scanf("%s",ret_cred->credentials.a_uname);
	getchar();
	printf("Enter website password: ");
	scanf("%s",ret_cred->credentials.a_pword);
	getchar();
}


void thread_join(locks *lock) {
	pthread_mutex_lock(&(lock->m));
 	while (lock->done == 0)
  		pthread_cond_wait(&(lock->w), &(lock->m));
  	pthread_mutex_unlock(&(lock->m));
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

int main(int argc, char** argv) {
	locks lock;
	pthread_mutex_t m;								//40 bytes
	pthread_cond_t w = PTHREAD_COND_INITIALIZER;
	create_vault();
	vault vault;
	uint32_t a = BUFF_SIZE;
	uint32_t *size = &a;
	while (1) {
		if(!read_vault(&vault)) {
			vault.num_users = 0;
			//printf("Vault empty\n");
		}
		user_account input, user_store;
		login_struct login_attempt;
		printf("Create account or login? (C/L): ");
		char c;
		scanf(" %c", &c);
		getchar();
		switch(c) {
			case 'c':
			case 'C': {
				//pthread_t p;
				create_account(&input);
				/*
				thread_args args;
				args.u_acct_t = input;
				args.size_th = *size;
				args.u_store_t = user_store;
				int rc;
				rc = pthread_create(&p, NULL, &create_user, (void *)&args);
				if(rc != 0) {
					fprintf(stderr, "Failed to create creare_user thread. Exiting Program.\n");
					exit(0);
				}
				*/
				lock.done = 0;
				lock.m = m;
				lock.w = w;
				create_user(&input, size, &user_store, &lock); //need lock
				thread_join(&lock);
				if(!vault_store_user(&vault, &user_store)) { //must wait for lock before calling this
					printf("Error adding acound: max number of users (%d) reached\n", MAX_USERS);
				}
				else {
					printf("Successfully added user\n");
				}
				write_vault(&vault);
				//printf("vault user: %s\n", vault.user_store[0].m_uname);
				//print_vault(&vault);
				break;
			}
			case 'l':
			case 'L': {
					login(&login_attempt);
					int32_t index = search_vault(&vault, &login_attempt);
					if(index >= 0) {
						//printf("index: %d\n", index);
						uint32_t found = false;
						lock.done = 0;
						lock.m = m;
						lock.w = w;
						check_user(&login_attempt, size, &vault.user_store[index], &found, &lock); //need lock here
						thread_join(&lock);
						if(found) {
							printf("User found!\n");
							while(1) {
								uint32_t k = get_add_or_edit();
								website user_cred;
								if(k == 1) {
									website ret_cred;
									uint32_t cred_found = false;
									printf("Get credentials\n");
									printf("Enter website name to get credentials: ");
									get_credentials(&user_cred);
									lock.done = 0;
									lock.m = m;
									lock.w = w;
									return_credentials(&vault.user_store[index], &user_cred, size, &ret_cred, &cred_found, &lock); //need lock
									thread_join(&lock);
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
									if(vault.user_store[index].num_accounts < MAX_ACCOUNTS) {
										add_credentials(&user_cred);
										lock.done = 0;
										lock.m = m;
										lock.w = w;
										encrypt_credentials(&user_cred, size, &encrypted_user_cred, &lock); //need lock
										thread_join(&lock);
										vault.user_store[index].accounts[vault.user_store[index].num_accounts] = encrypted_user_cred;
										vault.user_store[index].num_accounts++;
										write_vault(&vault);
										
										printf("user_account info. website name: %s, web uname: %s, web pword %s\n", 
											vault.user_store[index].accounts[vault.user_store[index].num_accounts-1].web_name,
											vault.user_store[index].accounts[vault.user_store[index].num_accounts-1].credentials.a_uname,
											vault.user_store[index].accounts[vault.user_store[index].num_accounts-1].credentials.a_pword);
									}
									else {
										printf("Max number of web credentials already added.  No more space.\n");
									}
								}
								else if(k==3) {
									//Edit credentials here:
									website encrypted_user_cred;
									printf("Edit credentials\n");
									website ret_cred;
									uint32_t cred_found = false;					
									printf("Enter website name to edit credentials: ");
									get_credentials(&user_cred);
									lock.done = 0;
									lock.m = m;
									lock.w = w;
									return_credentials(&vault.user_store[index], &user_cred, size, &ret_cred, &cred_found, &lock);
									thread_join(&lock);
									if(cred_found) {
										printf("Credentials found for \"%s\".\nUsername: %s\nPassword: %s\n",
											ret_cred.web_name, ret_cred.credentials.a_uname, ret_cred.credentials.a_pword);
										printf("Enter new credentials for \"%s\"\n", ret_cred.web_name);
										edit_uname_pword(&ret_cred);
										lock.done = 0;
										encrypt_credentials(&ret_cred, size, &encrypted_user_cred, &lock);
										thread_join(&lock);
											
									}
									else {
										printf("No credentials for the website (%s) found!\n", user_cred.web_name); //maybe
									}

								}
								else {
									printf("Error: Should not have made it here\n");
								}
							}
						}
						else {
							printf("Invalid username or password1\n");
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






