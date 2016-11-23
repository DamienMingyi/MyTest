/*
 * LDDSProtocol.h
 *
 *  Created on: Sep 8, 2016
 *      Author: level2
 */

#ifndef LDDSPROTOCOL_H_
#define LDDSPROTOCOL_H_

#include "IProtocol.h"
#include <arpa/inet.h>

#pragma pack(1)

struct LDDS_HEAD
{
	int begin;
	int length;
	//10142
	int category;
	//35
	int msgType;
	//10172
	int sequence;
	int timeStamp;
	int recvTime;
	int sendTime;
	int indication;
	int sourceID;
};

#pragma pack()

class LDDSProtocol: public IProtocol {
public:
	LDDSProtocol(char* parameter);
	virtual ~LDDSProtocol();

	virtual bool handleProtocol(char* buf, Message* msg);
	virtual int getLogOn(char* buf);
	virtual int getTotalLength(char* buf);
	virtual int getHeadLength()
	{
		return sizeof(LDDS_HEAD);
	}
protected:
	virtual bool decode(char* buf, Message* msg);
private:
	char* parameter;
	char msgType[8];
	char beginInteger[8];
};

#endif /* LDDSPROTOCOL_H_ */
