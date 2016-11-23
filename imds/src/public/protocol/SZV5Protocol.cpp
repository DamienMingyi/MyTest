/*
 * SZV5Protocol.cpp
 *
 *  Created on: Sep 8, 2016
 *      Author: level2
 */

#include "SZV5Protocol.h"

SZV5Protocol::SZV5Protocol(char* parameter, char* senderCompID, char* targetCompID,
		int heartBtInt, char* password, char* defaultApplVerID): parameter(parameter), senderCompID(senderCompID),
		targetCompID(targetCompID), heartBtInt(heartBtInt), password(password), defaultApplVerID(defaultApplVerID), bodyLength(0) {
	// TODO Auto-generated constructor stub
	memset(msgType, 0, sizeof(msgType));
}

SZV5Protocol::~SZV5Protocol() {
	// TODO Auto-generated destructor stub
}

int SZV5Protocol::getTotalLength(char* buf)
{
	bodyLength = ntohl(reinterpret_cast <MSG_HEAD*>(buf)->bodyLength);
	length = bodyLength + sizeof(uint32_t) + sizeof(MSG_HEAD);
	return length;
}

int SZV5Protocol::getLogOn(char* buf)
{
	LOGON* logon = reinterpret_cast<LOGON*>(buf);
	//head
	logon->msgHead.msgType = htonl(1);
	logon->msgHead.bodyLength = htonl(sizeof(LOGON_BODY));
	//body
	strncpy(logon->logon_body.senderCompID, this->senderCompID, 20);		//vde
	strncpy(logon->logon_body.targetCompID, this->targetCompID, 20);		//vss
	logon->logon_body.heartBtInt = htonl(this->heartBtInt);					//300
	strncpy(logon->logon_body.password, password, strlen(this->password));
	strncpy(logon->logon_body.defaultApplVerID, this->defaultApplVerID, 32);//1.00
	//tail
	logon->checksum = htonl(generateChecksum((char*)logon, sizeof(LOGON) - sizeof(uint32_t)));

	return sizeof(LOGON);
}

bool SZV5Protocol::handleProtocol(char* buf, Message* msg)
{
	if (!this->decode(buf, msg))
		return false;

	std::string t35(msgType);
	std::string para(parameter);
	if (para.find(t35) == std::string::npos)
	{
		msg->clear();
		return true;
	}
	//check sum
	uint32_t checkSum = 0;
	bodyLength = ntohl(reinterpret_cast <MSG_HEAD*>(buf)->bodyLength);
	memcpy(&checkSum, buf + sizeof(MSG_HEAD) + bodyLength, sizeof(checkSum));
	if (ntohl(checkSum) != generateChecksum(buf, bodyLength + sizeof(MSG_HEAD)))
	{
		ERROR("checksum error!");
		msg->clear();
		return true;
	}
	if (this->successorProtocol != NULL)
	{
		this->getTotalLength(buf + sizeof(MSG_HEAD));
		return this->successorProtocol->handleProtocol(buf + sizeof(MSG_HEAD), msg);
	}
	return true;
}

bool SZV5Protocol::decode(char* buf, Message* msg)
{
	MSG_HEAD* head = reinterpret_cast<MSG_HEAD*>(buf);
	sprintf(msgType, "%d", htonl(head->msgType));
	msg->bf = buf;
	return true;
}

uint32_t SZV5Protocol::generateChecksum(char* buf, uint32_t bufLen)
{
	uint32_t cks = 0;
	for (long idx = 0L; idx < bufLen; ++ idx)
		cks += (uint32_t)buf[idx];
	return cks % 256;
}

uint32_t SZV5Protocol::getHeartBeat(char* buf)
{
	HEART_BEAT* hb = reinterpret_cast<HEART_BEAT*>(buf);
	hb->msgHead.msgType = htonl(3);
	hb->msgHead.bodyLength = htonl(0);
	hb->checksum = htonl(generateChecksum(buf, sizeof(HEART_BEAT) - sizeof(uint32_t)));
	return sizeof(HEART_BEAT);
}
