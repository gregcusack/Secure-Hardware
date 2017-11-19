# Password Manager

## What it does:
* Encrypts and maintains a user's passwords for multiple websites

## Use
* Run the program and create an account
* Login and add website credentials (i.e. website, username, password)
* Access those credentials whenever you forget your credentials for a website!

## How it works:
* Create an account by entering master username and password.
	* Password is encrypted by enclave and stored in unstrusted memory in vault
* Add website credentials (website/web_uname/web_pword)
	* Website credentials are encrypted by enclave and stored in user account in vault
* Access your stored credentials
	* Login using your master username and password
	* Enter the website name for the credentials you want
	* Encvlave decrypts vault and returns the web_uname and web_pword back to you

## To Do:
* Implement user web account credentials as linked list
	* Currently stored as an array, but linked list would allow easy add and remove credential functionality
		* Currently no method to remove credentials
* Check if website credentials already exist
* Implement update master password
* Implement update website credentials
* Encrypt master username in vault
	* Must send entire vault to enclave in order to validate login
	* Currently, untrusted side looks up login username in vault and if it finds it, sends the found user structure to the enclave and the enclave decrypts the master password and compares decryted password to login password.

* To compile:
	* `gcc -w arm_side.c mb_side.c -o test`
* To run:
	* `./test`
	* Then follow on screen instructions for user
