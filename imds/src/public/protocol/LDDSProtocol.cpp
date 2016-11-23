/*
 * LDDSProtocol.cpp
 *
 *  Created on: Sep 8, 2016
 *      Author: level2
 */

#include "LDDSProtocol.h"

LDDSProtocol::LDDSProtocol(char* parameter): parameter(parameter) {
	// TODO Auto-generated constructor stub
	memset(msgType, 0, sizeof(msgType));
	memset(beginInteger, 0, sizeof(beginInteger));
}

LDDSProtocol::~LDDSProtocol() {
	// TODO Auto-generated destructor stub
}

bool LDDSProtocol::handleProtocol(char* buf, Message* msg)
{
	if (!this->decode(buf, msg))
		return false;
	std::string t35(msgType);
	std::string para(parameter);
	if (para.find(t35) == std::string::npos || t35 == "A")
	{
//		msg->vectorDecoderMessage.clear();
		msg->clear();
		return true;
	}

	if (this->successorProtocol != NULL)
	{
		this->successorProtocol->getTotalLength(buf + sizeof(LDDS_HEAD));
		return this->successorProtocol->handleProtocol(buf + sizeof(LDDS_HEAD), msg);
	}
	return true;
}

int LDDSProtocol::getTotalLength(char* buf)
{
	LDDS_HEAD* head = reinterpret_cast<LDDS_HEAD*>(buf);
	return htonl(head->length) + sizeof(LDDS_HEAD);
}

int LDDSProtocol::getLogOn(char* buf)
{
	return sizeof(LDDS_HEAD);
}

bool LDDSProtocol::decode(char* buf, Message* msg)
{
	LDDS_HEAD* head = reinterpret_cast<LDDS_HEAD*>(buf);
	if (htonl(head->begin) != 312403 && htonl(head->length) != 40)
	{
		WARN("this is message is not ldds message!");
		return false;
	}
	sprintf(msgType, "%d", htonl(head->msgType));
	sprintf(beginInteger, "%d", htonl(head->begin));
	FastCodecs::DecoderMessage dm;
	dm[0].setValue(3);

	dm[1].setID(8);
	dm[1].setValue(beginInteger);

	dm[2].setID(10142);
	dm[2].setValue(htonl(head->category));
	dm[3].setID(35);
	dm[3].setValue(htonl(head->msgType));
	dm[4].setID(10072);
	dm[4].setValue(htonl(head->sequence));
	msg->vectorDecoderMessage.push_back(dm);
	return true;
}
