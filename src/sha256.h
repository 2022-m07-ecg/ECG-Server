#ifndef _SHA256_H
#define _SHA256_H 1

#include <stdint.h>
#include <stdlib.h>

int getHash(const void *msg, size_t msgLen, uint64_t *key);

void hex2u256i(const char *string, size_t strLen, uint64_t *value);

#endif //_SHA256_H