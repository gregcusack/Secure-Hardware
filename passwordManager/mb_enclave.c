#include <time.h>
#include <sodium.h>

//#define MESSAGE ((const unsigned char *) "test")
//#define MESSAGE_LEN 4
//#define CIPHERTEXT_LEN (crypto_secretbox_MACBYTES + MESSAGE_LEN)

/*
void encrypt_account_password(unsigned char db_key[16], unsigned char *password_pt, 
	unsigned long password_len, unsigned char *password_ct, int seed, 
	unsigned char tag[16], unsigned int flags) {


}
*/

void encrypt_password(unsigned char* p, uint32_t* p_len, 
	unsigned char* ciphertext, uint32_t* cp_len) {
	printf("p: %s\n", p);
	printf("p_len: %u\n", *p_len);

	uint32_t x = crypto_secretbox_MACBYTES + *p_len;
	cp_len = &x;

	unsigned char nonce[crypto_secretbox_NONCEBYTES];
	unsigned char key[crypto_secretbox_KEYBYTES];
	printf("herex\n");
	//unsigned char ciphertext[*cp_len];
	unsigned char decrypted[*p_len];

	printf("cipher length: %d\n", *cp_len);

	randombytes_buf(nonce, sizeof nonce);
	randombytes_buf(key, sizeof key);
	crypto_secretbox_easy(ciphertext, p, *p_len, nonce, key);

	if (crypto_secretbox_open_easy(decrypted, ciphertext, *cp_len, nonce, key) != 0) {
	    printf("Bad things happened!  message was forged!");
	    return;
	}
	else {
		printf("Cipher text: %s\n", ciphertext);
		printf("Decrypted ciphertext: %s\n", decrypted);
	}
}

int main(int argc, char** argv) {
	unsigned char* p = (unsigned char*)"Greg";
	uint32_t a = 4;
	uint32_t* p_len = &a;
	printf("here2");
	unsigned char cipher_p[*p_len];
	uint32_t* cp_len;
	printf("size p_len: %u\n", *p_len);
	encrypt_password(p, p_len, cipher_p, cp_len);
	printf("cipher: %s\n", );

	return 0;
}

/*


// Set password 
// May need to change these input types as pointers 
int ew_accounts_set_password(uint32_t idx, const unsigned char *mbname, 
	unsigned char* mbpass, uint16_t mbpass_len) {
	//char nonce[12];
	unsigned char* epass[mbpass_len];

	srand(time(0));
	int seed = rand();

	encrypt_account

	if(mbpass_len) {

	}

}

*/