#define BUFF_SIZE 16
#define MAX_ACCOUNTS 5
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

typedef struct user_account {	//276
	unsigned char m_uname[BUFF_SIZE]; //master uname //16 bytes
	unsigned char m_pword[BUFF_SIZE];				 //16 bytes
	website accounts[MAX_ACCOUNTS];		//240 bytes
	uint32_t num_accounts;// = 0;		//4 bytes
} user_account;

typedef struct login_struct {
	unsigned char l_uname[BUFF_SIZE];
	unsigned char l_pword[BUFF_SIZE];
} login_struct;

typedef struct locks { //100 bytes
	uint32_t done;									//4 bytes
	pthread_mutex_t m;								//40 bytes		
	pthread_cond_t w;	//56 bytes
} locks;


/*
typedef struct thead_args {
	user_account u_acct_t;
	uint32_t size_th;
	user_account u_store_t;
} thread_args;
*/

/* User struct
 * username
 * password for master account
 * Then you have an array of tuples
 * 		[(facebook.com, (fb_uname, fb_pword)), (instagram.com, (insta_uname, fb_pword)), etc]
 */

/* Need getter for data
 * If login successful, you are prompted with what account info you want
 * You type in facebook, and you are given username and password
 */
