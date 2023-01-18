#ifndef _PARSE_H
#define _PARSE_H 1

#include <stdint.h>

#define DATA_HEADER 1
#define ERROR_HEADER 2
#define NOTIF_HEADER 3
#define ROT_HEADER 4

struct packetInfo_t {
	uint8_t packetID;
	uint32_t hashPrev;
	uint32_t deviceID;
	uint32_t ecgID;
};

struct dataSample_t {
	uint16_t data;
	uint64_t timeStamp;
};

uint32_t readHash(void *packet, int packetSize);

int parse(void *packet, int packetSize);

uint8_t readByte(void *packet, int ofst);

uint16_t readWord(void *packet, int ofst);

uint32_t readDWord(void *packet, int ofst);

uint64_t readQWord(void *packet, int ofst);

struct packetInfo_t getPacketInfo(void);

uint8_t getData(struct dataSample_t *buf);

#endif //_PARSE_H