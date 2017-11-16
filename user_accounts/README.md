* What it does:
	* Creates user account (username/password)
	* Sends account to MB, MB encrypts password with hardcoded key in MB
	* MB sends back account with encrypted password and ARM stores 
	* uname/pword combo in file.
	* User inputs username and password (currently hard coded) in attempt to login to system
	* ARM looks up input username in user file and sends to MB along with login attempt
	* ARM decrypts and compares usernames and passwords.  If same, user has logged in!

* To compile:
	* `gcc arm_side.c mb_side.c -o encrypt`
* To run:
	* `./encrypt`
