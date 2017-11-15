#include <time.h>
#include <sodium>

void encrypt_account_password(unsigned char db_key[16], unsigned char *password_pt, unsigned long password_len,
	unsigned char *password_ct, int seed, unsigned char tag[16], unsigned int flags) {


}





/* Set password */
/* May need to change these input types as pointers */
int ew_accounts_set_password(uint32_t idx, const unsigned char *mbname, unsigned char* mbpass, uint16_t mbpass_len) {
	//char nonce[12];
	unsigned char* epass[mbpass_len];

	srand(time(0));
	int seed = rand();

	encrypt_account

	if(mbpass_len) {

	}

}