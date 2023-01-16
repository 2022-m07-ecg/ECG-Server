#include "socket.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define SERVER_PORT 65000
#define HASH_PATH "tmp/SHA256_Buf"
#define HASH_LEN 64

static int servfd, clientfd;
static struct sockaddr_in servAddr, clientAddr;
static socklen_t servAddrLen, clientAddrLen;


int socketInit(void) {

	//Initialize variables
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(SERVER_PORT);
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddrLen = sizeof(servAddr);

	//Open socket
	servfd = socket(AF_INET, SOCK_STREAM, 0);
	if (servfd == -1) {
		perror("Can't open socket");
		return -1;
	}

	//Bind socket
	if ( bind(servfd, (struct sockaddr *)&servAddr, servAddrLen) == -1) {
		perror("Can't bind socket");
		close(servfd);
		return -1;
	}

	//Listen on socket
	if ( listen(servfd, 0) == -1 ) {
		perror("Can't listen on socket");
		close(servfd);
		return -1;
	}

	return 0;
}

int socketClose(void) {
	//Close socket
	if ( close(servfd) == -1 ) {
		perror("Can't close socket");
		return -1;
	}

	return 0;
}

int socketListen(void *buf, size_t bufSize) {
	
	ssize_t bytesRead;
	uint16_t packetLen;

	clientfd = accept(servfd, (struct sockaddr *)&clientAddr, &clientAddrLen);
	if (clientfd == -1) {
		perror("Can't accept connection");
		return -1;
	}

	//Initial packet read
	bytesRead = read(clientfd, buf, bufSize);
	if (bytesRead == -1) {
		perror("Can't read from client");
		close(clientfd);
		return -1;
	}
	else if (((char *)buf)[0] != '1') {	//Check version ID
		fprintf(stderr, "Incorrect version ID\n");
		close(clientfd);
		return -1;
	}
	
	//Get packet length
	packetLen = *(uint16_t *)(buf+1);
	if (bufSize < packetLen) {
		fprintf(stderr, "Buffer too small to read packet\n");
		close(clientfd);
		return -1;
	}
	//Keep reading until complete message is received
	int i;
	for (i = 0; i < 3; i++) {
		bytesRead += read(clientfd, buf+bytesRead, packetLen-bytesRead);
		if(bytesRead == packetLen) {
			break;
		}
	}
	if(bytesRead != packetLen) {
		fprintf(stderr, "Incomplete packet\n");
		return -1;
	}
	
	return bytesRead;
}

int closeCon(void) {
	if ( close(clientfd) == -1 ) {
		perror("Can't close connection");
		return -1;
	}

	return 0;
}

int getHash(const void *msg, size_t msgLen, uint64_t *key) {
	FILE *msgFile;
	FILE *hashFile;
	char hashCmd[50] = "sha256sum ";
	char buffer[HASH_LEN];

	//Open hash buffer
	msgFile = fopen(HASH_PATH, "w+");
	if (msgFile == NULL) {
		perror("Can't open hash file");
		return -1;
	}

	//Write msg to msgFile
	if ( fwrite(msg, 1, msgLen, msgFile) != msgLen ) {
		fprintf(stderr, "Can't write msg to hash buffer\n");
		return -1;
	}

	//Close hash buffer
	if ( fclose(msgFile) == EOF) {
		perror("Can't close hash file");
		return -1;
	}

	//Open SHA256 pipe
	strcat(hashCmd, HASH_PATH);
	hashFile = popen(hashCmd, "r");
	if (hashFile == NULL) {
		perror("Can't open SHA256 pipe");
		return -1;
	}

	//Read SHA256 key
	if ( fread((void *)buffer, 1, HASH_LEN, hashFile) != HASH_LEN ) {
		fprintf(stderr, "Can't read hash key\n");
		return -1;
	}

	//Close SHA256 pipe
	if ( pclose(hashFile) == -1) {
		perror("Can't close SHA256 pipe");
		return -1;
	}

	//Convert key to integer
	hex2u256i(buffer, HASH_LEN, key);

	return 0;
}

void hex2u256i(const char *string, size_t strLen, uint64_t *value) {
	int i;
	memset(value, 0, strLen/16);
	for (i = 0; i < strLen; i++) {
		char curSym = string[i];
		uint8_t curValue;
		//0-9
		if (curSym >= '0' && curSym <= '9')
			curValue = curSym - 48;
		//A-F
		else if (curSym >= 'A' && curSym <= 'F')
			curValue = curSym - 65 + 10;
		//a-f
		else if (curSym >= 'a' && curSym <= 'f')
			curValue = curSym - 97 + 10;

		uint64_t sigValue = curValue * 16^(i % 16);
		value[i/16] += sigValue;
	}
}