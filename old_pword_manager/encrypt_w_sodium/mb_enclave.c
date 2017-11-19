#include <sodium.h>

void encrypt_password(unsigned char* p, uint32_t* p_len, 
	unsigned char* ciphertext, uint32_t* cp_len) {
	//printf("p: %s\n", p);
	//printf("p_len: %u\n", *p_len);

	uint32_t x = crypto_secretbox_MACBYTES + *p_len;
	cp_len = &x;

	unsigned char nonce[crypto_secretbox_NONCEBYTES];
	unsigned char key[crypto_secretbox_KEYBYTES];
	//printf("herex\n");
	//unsigned char ciphertext[*cp_len];
	unsigned char decrypted[*p_len];

	//printf("cipher length: %d\n", *cp_len);

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

//another idea, use srand to generate a key and then XOR that key with your string

//need to repalce p with the cipher text!  can't have arm function having password still