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
	FILE *f = fopen("test.dat", "w"); //will need to be changed to w+
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
		if(!(strcmp(input->uname,output->uname))) {
			found_flag = true;
			break;
		}
	}
	if(found_flag)
		return true;
	return false;
}


int main(int argc, char** argv) {
	//user1: used to create account
	user_account user1 = {"username", "passW0rd!"};
	//login_user: used like a user trying to login to the system
	user_account login_user = {"username", "passW0rd!"};

	printf("input username: %s\n", user1.uname);
	printf("input password: %s\n", user1.pword);

	uint16_t a = BUFF_SIZE;
	uint16_t *size = &a;
	user_account user_store;

	/* Send user to MB, MB encrypt password
	 * MB remove access to pass
	 * MB returns structure with username and encrypted password
	 */
	create_user(&user1, size, &user_store);

	/* Check to see that ARM no longer has access to input uname/pword combo */
	//printf("username (should see username): %s\n", user1.uname);
	//printf("password (should be empty): %s\n", user1.pword);

	printf("returned username to store: %s\n", user_store.uname);
	printf("returned password to store: %s\n", user_store.pword);

	//store username/encrypted password in file in untrusted storage
	store_user(&user_store);

	//Get user from file (look up by username)
	user_account from_db;
	if(get_user(&login_user, &from_db)) {
		uint16_t found = false;
		/* Pass login info (uname/pword) and looked up info (uname/encryted pword)
		 * Send to MB 
		 */
		check_user(&from_db, size, &login_user, &found);
		if(found)
			printf("User found!\n");
		else
			printf("Invalid username or password\n");
	}
	else {
		printf("Invalid username or password\n");
	}
	return 0;
}