#include "socket.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdint.h>

#define SERVER_PORT 65000

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
	packetLen = htons(packetLen);
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
		close(clientfd);
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