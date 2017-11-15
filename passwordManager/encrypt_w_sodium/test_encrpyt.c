#include <sodium.h>
#include <stdio.h>

#define MESSAGE ((const unsigned char *) "test")
#define MESSAGE_LEN 4
#define CIPHERTEXT_LEN (crypto_secretbox_MACBYTES + MESSAGE_LEN)

unsigned char nonce[crypto_secretbox_NONCEBYTES];
unsigned char key[crypto_secretbox_KEYBYTES];
unsigned char ciphertext[CIPHERTEXT_LEN];

int main(int argc, char** argv) {
/*
	if(sodium_init() == -1) {
		printf("error\n");
		return 1;
	}
*/
	/* Generate a secure random key and nonce */
	randombytes_buf(nonce, sizeof nonce);
	randombytes_buf(key, sizeof key);
	/* Encrypt the message with the given nonce and key, putting the result in ciphertext */
	crypto_secretbox_easy(ciphertext, MESSAGE, MESSAGE_LEN, nonce, key);

	unsigned char decrypted[MESSAGE_LEN];
	if (crypto_secretbox_open_easy(decrypted, ciphertext, CIPHERTEXT_LEN, nonce, key) != 0) {
	    printf("Bad things happened!  message was forged!");
	    return -1;
	    /* If we get here, the Message was a forgery. This means someone (or the network) somehow tried to tamper with the message*/
	}
	else {
		printf("Cipher text: %s\n", ciphertext);
		printf("Decrypted ciphertext: %s\n", decrypted);
	}
	printf("length: %d\n%d\n%d\n", crypto_secretbox_KEYBYTES, crypto_secretbox_NONCEBYTES, CIPHERTEXT_LEN);
}