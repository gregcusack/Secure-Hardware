#define BUFF_SIZE 16
#define MAX_USERS 5
#ifndef USERCLASS_H
#define USERCLASS_H
#include "userclass.h"
#endif

typedef struct vault {  //size 1384 bytes
	user_account user_store[MAX_USERS];
	uint32_t num_users; // = 0
} vault;