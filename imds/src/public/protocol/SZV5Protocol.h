/*
 * SZV5Protocol.h
 *
 *  Created on: Sep 8, 2016
 *      Author: level2
 */

#ifndef SZV5PROTOCOL_H_
#define SZV5PROTOCOL_H_

#include "IProtocol.h"
#include "SZV5Struct.h"
#include <cstring>
#include <arpa/inet.h>

class SZV5Protocol: public IProtocol {
public:
	SZV5Protocol(char* parameter, char* senderCompID, char* targetCompID, int heartBtInt, char* password, char* defaultApplVerID);
	virtual ~SZV5Protocol();

	virtual int getTotalLength(char* buf);
	virtual int getHeadLength()
	{
		return sizeof(MSG_HEAD);
	}
	virtual int getLogOn(char* buf);
	virtual bool handleProtocol(char* buf, Message* msg);

	uint32_t getHeartBeat(char* buf);
private:
	uint32_t generateChecksum(char* buf, uint32_t bufLen);
protected:
	virtual bool decode(char* buf, Message* msg);

private:
	char* parameter;
	char msgType[8];
	char* senderCompID;
	char* targetCompID;
	int heartBtInt;
	char* password;
	char* defaultApplVerID;

	int bodyLength;
};

#endif /* SZV5PROTOCOL_H_ */
