#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include "socket.h"

// #include <netinet/in.h>
// #include <netinet/ip.h>

#define BUF_SIZE 64

int main(int argc, char **argv) {

	// uint8_t recvBuf[BUF_SIZE];
	// memset((void *)recvBuf, '9', BUF_SIZE);
	// // strcpy(recvBuf, "Hello World flabingus\n");
	// uint64_t hashKey[4];
	// // memset(recvBuf, (int)'1', 32);

	// int i;
	// for (i = 0; i < 32; i++) {
	// 	printf("%02x", recvBuf[i]);
	// }
	// printf("\n");
	// for (i = 0; i < 32; i++) {
	// 	printf("%u", recvBuf[i]);
	// }
	// printf("\n");

	// if ( getHash((void *)recvBuf, 32, hashKey) == -1 ) {
	// 	socketClose();
	// 	return -1;
	// }

	// for (i = 0; i < 4; i++) {
	// 	printf("%lx ", hashKey[i]);
	// }
	// printf("\n");
	// for (i = 0; i < 4; i++) {
	// 	printf("%lu ", hashKey[i]);
	// }
	// printf("\n");


	// !Debuging for hex2u256i function
	const char *string = "2d178a96cc95f1ed70d8ee2621e3f1bbf05f5f725c04a353788d3dce8064b31a";
	uint64_t key[4];
	printf("%s\n", string);
	hex2u256i(string, 64, key);
	int i;
	for (i = 0; i < 4; i++) {
		printf("%lx ", key[i]);
	}
	printf("\n");

	// if ( socketInit() == -1 ) {
	// 	return -1;
	// }

	// int packetSize = socketListen((void *)recvBuf, BUF_SIZE);
	// if ( packetSize == -1 ) {
	// 	socketClose();
	// 	return -1;
	// }

	// if ( getHash((void *)recvBuf, packetSize-4, hashKey) == -1 ) {
	// 	socketClose();
	// 	return -1;
	// }

	// if ( closeCon() == -1 ) {
	// 	socketClose();
	// 	return -1;
	// }

	// if ( socketClose() == -1 ) {
	// 	return -1;
	// }

	return 0;
}