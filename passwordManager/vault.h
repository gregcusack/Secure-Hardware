#ifndef USERCLASS_H
#define USERCLASS_H
#include "userclass.h"
#endif

typedef struct vault {  //size 1384 bytes
	unsigned char m_pword[BUFF_SIZE];				 //16 bytes
	website accounts[MAX_ACCOUNTS];		//240 bytes				 
	uint32_t num_accounts;// = 0;		//4 bytes
	uint32_t full;
} vault;