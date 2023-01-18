#include "parse.h"
#include <stdint.h>
#include <arpa/inet.h>
#include <stdio.h>

#define VERSION_OFST 0
#define SIZE_OFST 1
#define HEADER_OFST 3
#define INFO_BASE 4
#define INFO_PACKET_OFST INFO_BASE + 0
#define INFO_HASH_OFST INFO_BASE + 1
#define INFO_DEVICE_OFST INFO_BASE + 5
#define INFO_ECG_OFST INFO_BASE + 11
#define BODY_OFST 21

#define PACKET_OVRHD 25
#define DATA_BUF_SIZE 256

static struct packetInfo_t packetInfo;
static struct dataSample_t dataBuffer[DATA_BUF_SIZE];
static uint8_t dataLen;
static uint8_t errorCode;
static uint8_t nextID;

uint32_t readHash(void *packet, int packetSize) {
	uint32_t hash;
	hash = *(uint32_t *)(packet + packetSize - 4);
	hash = htonl(hash);
	return hash;
}

int parse(void *packet, int packetSize) {
	uint8_t header;

	//Check packet header
	header = readByte(packet, HEADER_OFST);

	//Get packet info
	packetInfo.packetID = readByte(packet, INFO_PACKET_OFST);
	packetInfo.hashPrev = readDWord(packet, INFO_HASH_OFST);
	packetInfo.deviceID = readDWord(packet, INFO_DEVICE_OFST);
	packetInfo.ecgID = readDWord(packet, INFO_ECG_OFST);

	switch(header)
	case DATA_HEADER: {
		dataLen = (packetSize-PACKET_OVRHD)/10;
		int i;
		for (i = 0; i < dataLen; i++) {
			//Only read data bits 11:0
			dataBuffer[i].data = (uint16_t)0x0FFF & readWord(packet, BODY_OFST + i*10);
			dataBuffer[i].timeStamp = readQWord(packet, BODY_OFST + i*10 + 2);
		}
		break;

	case ERROR_HEADER:
		errorCode = readByte(packet, BODY_OFST);
		break;
	
	//Notification not implemented yet
	case NOTIF_HEADER:
		fprintf(stderr, "Notification header not implemented yet\n");
		return -1;
	
	case ROT_HEADER:
		nextID = readByte(packet, BODY_OFST);
		break;
	
	default:
		fprintf(stderr, "Incorrect packet header\n");
		return -1;
	}

	return header;
}

uint8_t readByte(void *packet, int ofst) {
	uint8_t value;
	value = *(uint8_t *)(packet + ofst);
	return value;
}

uint16_t readWord(void *packet, int ofst) {
	uint16_t value;
	value = *(uint16_t *)(packet + ofst);
	value = htons(value);
	return value;
}

uint32_t readDWord(void *packet, int ofst) {
	uint32_t value;
	value = *(uint32_t *)(packet + ofst);
	value = htonl(value);
	return value;
}

//Custom implementation of htons for 64-bit
uint64_t readQWord(void *packet, int ofst) {
	uint64_t value;
	
	//Network and system byte order is the same
	if ((uint16_t)1 == htons(1)) {
		value = *(uint64_t *)(packet + ofst);
	} else {	//Convert from big endian to little endian
		uint64_t valueH = readDWord(packet, ofst);
		uint64_t valueL = readDWord(packet, ofst+4);
		value = valueL;
		value += valueH << 32;
	}
	return value;
}

struct packetInfo_t getPacketInfo(void) {
	return packetInfo;
}

uint8_t getData(struct dataSample_t *buf) {
	buf = dataBuffer;
	return dataLen;
}