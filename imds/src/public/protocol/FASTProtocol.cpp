/*
 * FASTProtocol.cpp
 *
 *  Created on: Sep 9, 2016
 *      Author: level2
 */

#include "FASTProtocol.h"
#include <stdio.h>

FASTProtocol::FASTProtocol(char* tpName): iCount(0) {
	// TODO Auto-generated constructor stub
	if (!this->loadTemplate(tpName))
	{
		ERROR("loading template file %s falied!", tpName);
		exit(1);
	}
}

FASTProtocol::~FASTProtocol() {
	// TODO Auto-generated destructor stub
}

bool FASTProtocol::handleProtocol(char* buf, Message* msg)
{
	return this->decode(buf, msg);
}

bool FASTProtocol::loadTemplate(char* tpName)
{
	return decoder.loadTemplate(tpName);
}

bool FASTProtocol::decode(char* buf, Message* msg)
{
	int i = 0;
	int tag95 = 0;
	int tagCount = 0;
	msg->vectorDecoderMessage.back()[0].getInt(tagCount);
	while (i <= tagCount)
	{
		switch(msg->vectorDecoderMessage.back()[i].getID())
		{
		case 95:
			msg->vectorDecoderMessage.back()[i].getInt(tag95);
			break;
		}
		++ i;
	}

	vector<FastCodecs::DecoderMessage> vecDm;
	//try-throw-catch
	decoder.decode((FastCodecs::uchar_t*)buf, tag95, vecDm);
	msg->vectorDecoderMessage.insert(msg->vectorDecoderMessage.end(), vecDm.begin(), vecDm.end());
	return true;
}
