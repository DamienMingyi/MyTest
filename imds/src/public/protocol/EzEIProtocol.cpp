/*
 * EzEIProtocol.cpp
 *
 *  Created on: Sep 8, 2016
 *      Author: level2
 */

#include "EzEIProtocol.h"

EzEIProtocol::EzEIProtocol(char* parameter, int mode): STEPProtocol(parameter), bodyPos(0), mode(mode) {
	// TODO Auto-generated constructor stub
	headLength = sprintf(msgHead, "8=FIXT.1.1");
	msgHead[headLength ++] = 0x01;
	headLength += sprintf(msgHead + headLength, "9=");
}

EzEIProtocol::~EzEIProtocol() {
	// TODO Auto-generated destructor stub
}

int EzEIProtocol::getTotalLength(char* buf)
{
	char tagLen[12];
	int i;
	int bodyLen = 0;
	int tailLen = 7;//10=xxx|

	//get body length
	for (i = 0;i < 8; i ++)
	{
		tagLen[i] = buf[headLength + i];
		if (tagLen[i] == 0x01)
		{
			tagLen[i] = 0;
			bodyLen = atoi(tagLen);
			break;
		}
	}
	if (bodyLen == 0)
		return 0;
	length = bodyLen + headLength + i + 1 + tailLen;
	return length;
}

bool EzEIProtocol::handleProtocol(char* buf, Message* msg)
{
	decode(buf, msg);
	int tagCount = 0;
	msg->vectorDecoderMessage.back()[0].getInt(tagCount);
	int i = 1;
	char tag35[8];
	memset(tag35, 0, 8);
	char tag167[8];
	memset(tag167, 0, 8);
	int tag339 = 0;
	while (i <= tagCount)
	{
		switch(msg->vectorDecoderMessage.back()[i].getID())
		{
		case 35:
			msg->vectorDecoderMessage.back()[i].getString(tag35);
			break;
		case 167:
			msg->vectorDecoderMessage.back()[i].getString(tag167);
			break;
		case 339:
			msg->vectorDecoderMessage.back()[i].getInt(tag339);
			break;
		}
		++ i;
	}

	std::string para(parameter);
	std::string t167(tag167);
	if (para.find(t167) == std::string::npos)
	{
//		msg->vectorDecoderMessage.clear();
		msg->clear();
		return true;
	}
	if (this->successorProtocol != NULL && tag339 == mode  && tag35[0] != 'h')
	{
		this->successorProtocol->getTotalLength(buf + bodyPos);
		return this->successorProtocol->handleProtocol(buf + bodyPos, msg);
	}
	else if (tag339 != mode)
	{
//		msg->vectorDecoderMessage.clear();
		msg->clear();
		return false;
	}
	return true;
}

bool EzEIProtocol::decode(char* buf, Message* msg)
{
	int index = 1;
	FastCodecs::DecoderMessage dm;
	int tag95 = 0;
//	char tag35[8];
	int i, j, valuePos;
	bool findEqual;

	char szNumber[64];
	char szValue[64];
	int tagNumber;
	i = j = findEqual = 0;
	while(i < length)
	{
		if (!findEqual)
		{
			szNumber[j] = buf[i];
			if (szNumber[j] == '=')
			{
				szNumber[j] = 0;
				tagNumber = atoi(szNumber);
				j = 0;
				valuePos = i+1;
				findEqual = true;
			}
			else
				j ++;
		}
		else
		{
			if (tagNumber == 96)
			{
				bodyPos = valuePos;
				i += tag95 + 1;
				dm[index].setID(96);
				dm[index ++].setValue(buf + bodyPos, tag95);
				findEqual = false;
				break;
			}
			else
			{
				if (buf[i] == 0x01)
				{
					memcpy(szValue, buf + valuePos, i - valuePos);
					szValue[i - valuePos] = 0;
					findEqual = false;

					switch(tagNumber)
					{
					case 95:
						dm[index].setID(tagNumber);
						tag95 = atoi(szValue);
						dm[index ++].setValue(tag95);
						break;
					case 8:
					case 35:
					case 167:
					case 265:
					case 336:
						dm[index].setID(tagNumber);
						dm[index ++].setValue(szValue);
						break;
					case 34:
					case 339:
					case 1180:
					case 1181:
					case 75:
					case 779:
					case 5468:
					case 393:
						dm[index].setID(tagNumber);
						dm[index ++].setValue(atoi(szValue));
						break;
					}
				}
			}
		}
		i++;
	}
	dm[0].setValue(index);
	msg->vectorDecoderMessage.push_back(dm);
	return true;
}
