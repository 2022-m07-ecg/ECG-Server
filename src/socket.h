#ifndef _SOCKET_H
#define _SOCKET_H 1

#include <stdint.h>
#include <stdlib.h>

int socketInit(void);

int socketClose(void);

int socketListen(void *buf, size_t bufSize);

int closeCon(void);

int getHash(const void *msg, size_t msgLen, uint64_t *key);

void hex2u256i(const char *string, size_t strLen, uint64_t *value);

#endif //_SOCKET_H