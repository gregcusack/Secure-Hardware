#include <stdio.h>
#include <string.h>
#include "mb_test.h"

#define BUFF_SIZE 16

int main(int argc, char** argv) {
	unsigned char sendBuff[BUFF_SIZE] = "Hello World!";
	unsigned char recvBuff[BUFF_SIZE];

	toggle_case(sendBuff, recvBuff);

	printf("Sent string:\t\t%s\n", sendBuff);
	printf("Received string:\t%s\n", recvBuff);

	return 0;
}
