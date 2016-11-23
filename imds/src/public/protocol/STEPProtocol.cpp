/*
 * STEPProtocol.cpp
 *
 *  Created on: Sep 7, 2016
 *      Author: level2
 */

#include "STEPProtocol.h"

STEPProtocol::STEPProtocol(char* parameter): parameter(parameter), bodyPos(0) {
	// TODO Auto-generated constructor stub
	headLength = sprintf(msgHead, "8=STEP.1.0.0");
	msgHead[headLength ++] = 0x01;
	headLength += sprintf(msgHead + headLength, "9=");
	memset(tag35, 0, 8);
}

STEPProtocol::~STEPProtocol() {
	// TODO Auto-generated destructor stub
}

int STEPProtocol::getLogOn(char* buf)
{
	char split = 0x01;
	int msglen = 0;
	//52
	char dateTime[18];
	time_t tt = time(NULL);
	tm *time = localtime(&tt);
	int date = (time->tm_year + 1900)*10000+ (time->tm_mon + 1)*100+ time->tm_mday;
	int hour = time->tm_hour;
	int minute = time->tm_min;
	int second = time->tm_sec;

	int tmLength = sprintf(dateTime, "%d-%02d:%02d:%02d", date, hour, minute, second);

	msglen = sprintf(buf, "8=STEP.1.0.0%c9=56%c35=A%c49=VTF%c56=VDE%c34=0%c52=%s%c98=0%c108=0%c",
			split, split, split, split, split, split, dateTime, split, split, split);
	int chk = 0;
	for(int i = 0; i < msglen; i ++)
	{
		chk += buf[i];
	}
	msglen += sprintf(buf + msglen, "10=%03d%c", chk % 256, split);

	return msglen;
}
bool STEPProtocol::handleProtocol(char* buf, Message* msg)
{
	//step protocol
	if (!this->decode(buf, msg))
		return false;

	std::string t35(tag35);
	std::string para(parameter);
	if (para.find(t35) == std::string::npos || t35 == "A")
	{
//		msg->vectorDecoderMessage.clear();
		msg->clear();
		return true;
	}
	if (this->successorProtocol != NULL)
	{
		this->successorProtocol->getTotalLength(buf + bodyPos);
		return this->successorProtocol->handleProtocol(buf + bodyPos, msg);
	}
	return true;
}

int STEPProtocol::getTotalLength(char* buf)
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

bool STEPProtocol::decode(char* buf, Message* msg)
{
	int index = 1;
	FastCodecs::DecoderMessage dm;

	int tag95 = 0;

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
				dm[index].setID(tagNumber);
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
					case 8:
						dm[index].setID(tagNumber);
						dm[index ++].setValue(szValue);
						break;
					case 95:
						dm[index].setID(tagNumber);
						tag95 = atoi(szValue);
						dm[index ++].setValue(tag95);
						break;
					case 35:
						dm[index].setID(tagNumber);
						strncpy(tag35, szValue, sizeof(tag35));
						dm[index ++].setValue(szValue);
						break;
					case 10072:
					case 10142:
						dm[index].setID(tagNumber);
						dm[index ++].setValue(atoi(szValue));
						break;
					}
				}
			}
		}
		i++;
	}
	if (tag35[0] == '5')
	{
		INFO("tag 35 is 5");
		return false;
	}
	dm[0].setValue(index);
	msg->vectorDecoderMessage.push_back(dm);
	return true;
}
