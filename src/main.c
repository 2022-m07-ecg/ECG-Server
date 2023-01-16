#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

// #include <netinet/in.h>
// #include <netinet/ip.h>

#define SERVER_PORT 65000
#define BUF_SIZE 4096

static int servfd, clientfd;
static struct sockaddr_in servAddr;

void server_exit(void) {
	//Close socket
	if ( close(servfd) == -1 )
		perror("Can't close socket");
}

int main(int argc, char **argv) {

	uint8_t recvBuff[BUF_SIZE];

	atexit(server_exit);

	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(SERVER_PORT);
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	//Open socket
	servfd = socket(AF_INET, SOCK_STREAM, 0);
	if (servfd == -1) {
		perror("Can't open socket");
		return -1;
	}

	//Bind socket
	if ( bind(servfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) == -1) {
		perror("Can't bind socket");
		return -1;
	}

	//Listen on socket
	if ( listen(servfd, 0) == -1 ) {
		perror("Can't listen on socket");
		return -1;
	}

	// char *msg = "Hello world\n";
	// size_t msg_len = strlen(msg);
	//Clear buffer
	memset((void *)recvBuff, 0, BUF_SIZE);
	while(1) {
		struct sockaddr_in clientAddr = {0};
		socklen_t clientAddrLen = sizeof(clientAddr);
		ssize_t bytesRead;
		
		clientfd = accept(servfd, (struct sockaddr *)&clientAddr, &clientAddrLen);
		if (clientfd == -1) {
			perror("Can't accept connection");
			return -1;
		}

		bytesRead = read(clientfd, (void *)recvBuff, BUF_SIZE);
		if (bytesRead == -1) {
			perror("Can't read from client");
			close(clientfd);
			return -1;
		}

		// ssize_t send_len = send(clientfd, (void *)msg, msg_len, 0);
		// if ((send_len == -1) || (send_len != msg_len)) {
		// 	perror("Can't send message");
		// 	close(clientfd);
		// 	return -1;
		// }
		close(clientfd);
	}

	return 0;
}