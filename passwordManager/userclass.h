#define BUFF_SIZE 257
#define MAX_ACCOUNTS 1650
#include <stdint.h>

#ifndef PTHREAD_H
#define PTHREAD_H
#include <pthread.h>
#endif

typedef struct web_login { //32 bytes
	unsigned char a_uname[BUFF_SIZE]; //acount uname
	unsigned char a_pword[BUFF_SIZE];
} web_login;

typedef struct website { // 48 bytes
	unsigned char web_name[BUFF_SIZE]; //website name  //16 bytes
	web_login credentials;	//32 bytes  //this will be stored as copy, change in future?
} website;

typedef struct user_account {	//276				 //16 bytes
	website accounts[MAX_ACCOUNTS];		//240 bytes
} user_account;