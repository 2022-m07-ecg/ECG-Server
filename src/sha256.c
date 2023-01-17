#include "sha256.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>


#define HASH_PATH "tmp/SHA256_Buf"
#define HASH_LEN 64

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
	memset(value, 0, strLen/2);
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

		uint8_t exp = (63-i) % 16;
		uint64_t sigValue = (uint64_t)1 << (exp*4);	//Avoid using pow(), effectively 16^exp
		sigValue *= curValue;
		value[i/16] += sigValue;
	}
}