* What it does:
	* Encrypts a passwords using a basic XOR key and return ciphertext to unstrusted progam
* How it works:
	* Password passed by pointer along with empty cipher text buffer to enclave
	* Enclave generates random key and XORs key with input password
	* Output cipher text stored in the passed cipher text buffer.
	* Array containing clear text buffer is zeroed so untrusted program \ 
	  no longer has access to password
	* Encrypted password can be accessed by untrusted program via the pointer
	  to the cipher text buffer

* To compile:
	* `gcc arm_user_xor.c mb_enclave_xor.c -o encrypt`
* To run:
	* `./encrypt <password to encrypt>`
