#ifndef USERCLASS_H
#define USERCLASS_H
#include "userclass.h"
#endif

#ifndef MBSIDE_H
#define MBSIDE_H
#include "mbside.h"
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

//function for create account
//function for login


int main(int argc, char** argv) {
	uint32_t a = BUFF_SIZE;
	uint32_t *size = &a;
	printf("Create account or login? (C/L): ");
	char c;
	scanf(" %c", &c);
	getchar();
	if(c != 'C' && c != 'c' && c != 'L' && c != 'l') {
		printf("Bad input, run again\n");
		exit(1);
	}
	else if(c == 'C' || c == 'c') {
		user_account user_create;
		printf("Create account!\n");
		printf("Enter username: ");
		scanf("%s",user_create.m_uname);
		getchar();
		printf("Enter password: ");
		scanf("%s",user_create.m_pword);
		getchar();
		//printf("user_create: %s, %s\n", user_create.uname, user_create.pword);
		user_account user_store;
		create_user(&user_create, size, &user_store);
		store_user(&user_store);
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
		if(get_user(&login, &user_from_db)) {
			uint32_t found = false;
			check_user(&user_from_db, size, &login, &found);
			if(found)
				printf("User found!\n");
			else
				printf("Invalid username or password\n");
		}
		else {
			printf("Invalid username or password\n");
		}
	}
	return 0;
}