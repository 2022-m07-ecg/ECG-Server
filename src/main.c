#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include "socket.h"
#include "sha256.h"
#include "parse.h"

// #include <netinet/in.h>
// #include <netinet/ip.h>

#define BUF_SIZE 4096

int main(int argc, char **argv) {

	uint8_t prevHash = 0;

	if ( socketInit() == -1 ) {
		return -1;
	}

	while(1) {
		uint8_t recvBuf[BUF_SIZE];
		uint64_t calcHash[4];
		uint32_t packetHash;
		int8_t packetHeader;
		struct packetInfo_t packetInfo;

		//Wait for incoming packet
		int packetSize = socketListen((void *)recvBuf, BUF_SIZE);
		if ( packetSize == -1 ) {
			socketClose();
			return -1;
		}

		//Calculate hash
		if ( getHash((void *)recvBuf, packetSize-4, calcHash) == -1 ) {
			closeCon();
			socketClose();
			return -1;
		}

		//Extract hash from packet
		packetHash = readHash((void *)recvBuf, packetSize);
		
		//Compare the last 4 bytes of the hashes
		if ( packetHash != (uint32_t)calcHash[3] ) {
			fprintf(stderr, "Incorrect hash\n");
			closeCon();
			socketClose();
			return -1;
		}

		//Parse packet completely
		packetHeader = parse((void *)recvBuf, packetSize);
		packetInfo = getPacketInfo();

		//Check previous hash
		if (packetInfo.hashPrev != prevHash) {
			fprintf(stderr, "Security breach\n");
			closeCon();
			socketClose();
			return -1;
		}

		switch(packetHeader) {
		case -1:
			closeCon();
			socketClose();
			return -1;

		case DATA_HEADER:
			break;

		case ERROR_HEADER:
			break;

		case NOTIF_HEADER:
			break;

		case ROT_HEADER:
			break;
		}

		//Close connection
		if ( closeCon() == -1 ) {
			socketClose();
			return -1;
		}
	}

	if ( socketClose() == -1 ) {
		return -1;
	}

	return 0;
}