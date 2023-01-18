#ifndef _SOCKET_H
#define _SOCKET_H 1

#include <stdlib.h>

/**
 * @brief Initialize TCP socket for connecting to the internet on
 * port 65000
 * 
 * @return -1 if failed and print error message, else 0
 */
int socketInit(void);

/**
 * @brief Close socket, if this isn't called when exiting a program,
 * socketInit() won't work
 * 
 * @return -1 if failed and print error message, else 0
 */
int socketClose(void);

/**
 * @brief Block calling program until a connection to the socket is attempted
 * Will return the complete message sent
 * 
 * @param buf buffer to store received message
 * @param bufSize size of buffer in bytes, if smaller than message size
 * en error will be returned
 * 
 * @return -1 if failed and print error message, else 
 * total amount of bytes read
 */
int socketListen(void *buf, size_t bufSize);

/**
 * @brief Close active connection to socket
 * needs to be called to accept new connections with socketListen
 * 
 * @return -1 if failed and print error message, else 0
 */
int closeCon(void);

#endif //_SOCKET_H