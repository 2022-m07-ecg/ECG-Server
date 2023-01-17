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

#define BUF_SIZE 4096

int main(int argc, char **argv) {

	uint8_t recvBuf[BUF_SIZE];
	uint64_t hashKey[4];

	if ( socketInit() == -1 ) {
		return -1;
	}

	int packetSize = socketListen((void *)recvBuf, BUF_SIZE);
	if ( packetSize == -1 ) {
		socketClose();
		return -1;
	}

	if ( getHash((void *)recvBuf, packetSize-4, hashKey) == -1 ) {
		socketClose();
		return -1;
	}

	if ( closeCon() == -1 ) {
		socketClose();
		return -1;
	}

	if ( socketClose() == -1 ) {
		return -1;
	}

	return 0;
}