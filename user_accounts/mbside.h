#include <stdint.h>
#include <stdbool.h>
#ifndef USERCLASS_H
#define USERCLASS_H
#include "userclass.h"
#endif

void create_user(user_account *user_data, uint32_t *pword_len, 
	user_account *user_cipher);

void check_user(user_account *cipher_data, uint32_t *pword_len, 
	user_account *login_attempt, uint32_t *found);

/* have option to add login once check_user called */
void add_login(user_account *user_data, uint32_t *pword_len,
	user_account *cipher_account);
