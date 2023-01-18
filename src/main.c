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
#include "db.h"

// #include <netinet/in.h>
// #include <netinet/ip.h>

#define RCV_BUF_SIZE 4096
#define SAMPLE_BUF_SIZE 256

int main(int argc, char **argv) {

	uint8_t prevHash = 0;

	if ( socketInit() == -1 ) {
		return -1;
	}

	if ( dbInit() == -1 ) {
		socketClose();
		return -1;
	}

	while(1) {
		uint8_t recvBuf[RCV_BUF_SIZE];
		uint64_t calcHash[4];
		uint32_t packetHash;
		int8_t packetHeader;
		struct packetInfo_t packetInfo;

		//Wait for incoming packet
		int packetSize = socketListen((void *)recvBuf, RCV_BUF_SIZE);
		if ( packetSize == -1 ) {
			dbClose();
			socketClose();
			return -1;
		}

		//Calculate hash
		if ( getHash((void *)recvBuf, packetSize-4, calcHash) == -1 ) {
			closeCon();
			dbClose();
			socketClose();
			return -1;
		}

		//Extract hash from packet
		packetHash = readHash((void *)recvBuf, packetSize);
		
		//Compare the last 4 bytes of the hashes
		if ( packetHash != (uint32_t)calcHash[3] ) {
			fprintf(stderr, "Incorrect hash\n");
			closeCon();
			dbClose();
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
			dbClose();
			socketClose();
			return -1;
		}

		switch(packetHeader) {
		case -1:
			closeCon();
			dbClose();
			socketClose();
			return -1;

		case DATA_HEADER: {
			struct dataSample_t sampleBuffer[SAMPLE_BUF_SIZE];
			uint8_t n;
			n = getData(sampleBuffer);

			if ( dbStore(sampleBuffer, packetInfo.ecgID, n) == -1 ) {
				closeCon();
				dbClose();
				socketClose();
				return -1;
			}
			break;
		}

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

	dbClose();

	if ( socketClose() == -1 ) {
		return -1;
	}

	return 0;
}