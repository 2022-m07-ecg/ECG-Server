#ifndef _SOCKET_H
#define _SOCKET_H 1

#include <stdlib.h>

int socketInit(void);

int socketClose(void);

int socketListen(void *buf, size_t bufSize);

int closeCon(void);

#endif //_SOCKET_H