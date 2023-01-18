#ifndef _SHA256_H
#define _SHA256_H 1

#include <stdint.h>
#include <stdlib.h>

/**
 * @brief Calculate 32-Byte SHA256 hash for message
 * 
 * @param msg buffer containing message to be used for hash
 * @param msgLen length of message
 * @param key array of 4 64-bit unsigned ints containing hash
 * Highest order value indexed first, so to get complete message:
 * key[0] & key[1] & key[2] & key[3]
 * 
 * @return -1 if failed and print error message, else 0  
 */
int getHash(const void *msg, size_t msgLen, uint64_t *key);

/**
 * @brief Convert 64 character hexadecimal string into array of ints
 * 
 * @param string contains hexadecimal number, can only contain 
 * 0-9, a-f and A-F, so no 0x prefixes or other characters
 * @param strLen length of string, can be maximum of 64
 * @param value array of 4 64-bit unsigned ints containing hash
 * Highest order value indexed first, so to get complete message:
 * key[0] & key[1] & key[2] & key[3]
 */
void hex2u256i(const char *string, size_t strLen, uint64_t *value);

#endif //_SHA256_H