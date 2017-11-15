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
	if(found_flag) {
		printf("Account found! Now, need to call, check password\n");
		printf("Username found: %s\n", output->uname);
		printf("Username found (login): %s\n", input->uname);
		return true;
	}
	printf("Incorrect username/password\n");
	printf("Username not found: %s\n", output->uname);
	printf("Username not found (login): %s\n", input->uname);
	return false;
}


int main(int argc, char** argv) {

	//user_account userArray[10];
	//user_account *next_avail = &userArray[0];

	user_account user1 = {"greg", "password1"};

	user_account login_user = {"greg", "password1"};

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
	printf("username (should : %s\n", user1.uname);
	printf("password (should be empty): %s\n", user1.pword);

	printf("ret_uname: %s\n", user_store.uname);
	printf("ret_pword: %s\n", user_store.pword);

	if(store_user(&user_store)) {
		printf("Stored use successfully!\n");
	}
	else {
		printf("ERROR: Storing user failed!\n");
		exit(1);
	}
	user_account from_db;
	if(get_user(&login_user, &from_db)) {
		//these should not exist in real implementation, should just call MB
		printf("Username found, now need to send to MB for password validation\n");
	}
	else {
		printf("Invalid username or password\n");
	}

	uint8_t found = false;
	check_user(&from_db, size, &login_user, &found);  //if account fund
	if(found) {
		printf("Username/password combination correct\n");
	}
	else {
		printf("Invalid username or password\n");
	}

	return 0;

}