#include <stdint.h>
#include <stdbool.h>
#ifndef USERCLASS_H
#define USERCLASS_H
#include "userclass.h"
#endif

void create_user(user_account *user_data, uint16_t *sruct_size, 
	user_account *user_cipher);

void check_user(user_account *cipher_data, uint16_t *struct_size, 
	user_account *login_attempt, uint8_t *found);
