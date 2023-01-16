#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

// #include <netinet/in.h>
// #include <netinet/ip.h>

#define PORT_NUMBER 65000

static int sockfd;
static struct sockaddr_in server_info;
static socklen_t server_info_len = sizeof(server_info);
static int clientfd;
static struct sockaddr_in client_info = {0};
static socklen_t client_info_len = sizeof(client_info);

void server_exit(void) {
	//Close socket
	if ( close(sockfd) == -1 )
		perror("Can't close socket");
}

int main(int argc, char **argv) {

	atexit(server_exit);

	server_info.sin_family = AF_INET;
	server_info.sin_port = htons(PORT_NUMBER);
	server_info.sin_addr.s_addr = htonl(INADDR_ANY);

	//Open socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		perror("Can't open socket");
		return -1;
	}

	//Bind socket
	if ( bind(sockfd, (struct sockaddr *)&server_info, server_info_len) == -1) {
		perror("Can't bind socket");
		return -1;
	}

	//Listen on socket
	if ( listen(sockfd, 0) == -1 ) {
		perror("Can't listen on socket");
		return -1;
	}

	char *msg = "Hello world\n";
	size_t msg_len = strlen(msg);
	while(1) {
		clientfd = accept(sockfd, (struct sockaddr *)&client_info, &client_info_len);
		if (clientfd == -1) {
			perror("Can't accept connection");
			return -1;
		}
		ssize_t send_len = send(clientfd, (void *)msg, msg_len, 0);
		if ((send_len == -1) || (send_len != msg_len)) {
			perror("Can't send message");
			close(clientfd);
			return -1;
		}
		close(clientfd);
	}

	return 0;
}