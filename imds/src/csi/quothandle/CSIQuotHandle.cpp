/*
 * CSIQuotHandle.cpp
 *
 *  Created on: Nov 15, 2016
 *      Author: level2
 */

#include "CSIQuotHandle.h"

const char CSIQuotHandle::stepBeginString[] = "STEP.1.0.0";
const char CSIQuotHandle::ezeiBeginString[] = "FIXT.1.1";
const char CSIQuotHandle::beginInteger[] = "312403";
const char CSIQuotHandle::indexInfo[] = "01";

CSIQuotHandle::CSIQuotHandle(): snap0(NULL), sequence(0), tag10072(0), tag10142(0), date(0), time(0) {
	// TODO Auto-generated constructor stub
	memset(securityType, 0, sizeof(securityType));
}

CSIQuotHandle::~CSIQuotHandle() {
	// TODO Auto-generated destructor stub
	for (IndexSnapList::iterator it = indexSnpLst.begin(); it != indexSnpLst.end(); ++ it)
		delete it->second;
}

void CSIQuotHandle::handle(Message* msg)
{
	for (uint32_t i = 0; i < msg->vectorDecoderMessage.size(); ++ i)
	{
		int id = msg->vectorDecoderMessage[i][1].getID();
		if (id == 8)
		{
			char beginString[16];
			memset(beginString, 0, 16);
			msg->vectorDecoderMessage[i][1].getString(beginString);
			if (strncmp(beginString, stepBeginString, sizeof(stepBeginString)) == 0 ||
					strncmp(beginString, beginInteger, sizeof(beginInteger)) == 0)
				handleSTEP(msg->vectorDecoderMessage[i]);
			else if (strncmp(beginString, ezeiBeginString, sizeof(ezeiBeginString)) == 0)
				handleEzEI(msg->vectorDecoderMessage[i]);
			else
				WARN("begin string inconsistent!");
		}
		else
			handleFAST(msg->vectorDecoderMessage[i]);
	}
}

void CSIQuotHandle::handleSTEP(FastCodecs::DecoderMessage& dm)
{
	int tagCount = 0;
	if (!dm[0].getInt(tagCount))
		return ;
	for (int i = 1; i <= tagCount; ++ i)
	{
		switch(dm[i].getID())
		{
		case 8:
			break;
		case 35:
			break;
		case 10072:
			dm[i].getInt(tag10072);
			break;
		case 10142:
			dm[i].getInt(tag10142);
			break;
		default:
			break;
		}
	}
}

void CSIQuotHandle::handleEzEI(FastCodecs::DecoderMessage& dm)
{
	int tagCount = 0;
	int seq = 0;
	if (!dm[0].getInt(tagCount))
		return ;
	for (int i = 1; i <= tagCount; ++ i)
	{
		switch(dm[i].getID())
		{
		case 8:
			break;
		case 34:
			dm[i].getInt(seq);
			break;
		case 35:
			break;
		case 167:
			dm[i].getString(this->securityType);
			break;
		case 339:
			break;
		case 1180:
			break;
		case 1181:
			break;
		case 75:
			dm[i].getInt(date);
			break;
		case 779:
			dm[i].getInt(time);
			break;
		case 265:
			break;
		case 5468:
			break;
		case 393:

			break;
		default:
			break;
		}
	}
}

void CSIQuotHandle::handleFAST(FastCodecs::DecoderMessage& dm)
{
    time_t recvTime = ::time(NULL);
	struct tm *tt=localtime(&recvTime);
	if (snap0 == NULL)
		return ;
	snap0->lowPx =(tt->tm_hour*10000+tt->tm_min*100+tt->tm_sec)*1000;
	snap0->lastPx = snap0->time;

	int tagCount = 0;
	if (!dm[0].getInt(tagCount))
		return ;
	char securityID[16];
	memset(securityID, 0, 16);
	int noMDEntries = 0;
	FastCodecs::int64_t price = 0L;
	FastCodecs::int64_t amount = 0L;
	INDEXSNAP* indexsnap = NULL;
	char MDStreamID[8];
	memset(MDStreamID, 0, 8);

	char mdEntryType[2];
	memset(mdEntryType, 0, 2);
	char mdEntryTime[8];
	memset(mdEntryTime, 0, 8);
	for (int i = 1; i <= tagCount; ++ i)
	{
		switch (dm[i].getID())
		{
		case 1500:
			dm[i].getString(MDStreamID);
			break;
		case 49:
			if (strncmp(MDStreamID, indexInfo, 2) == 0)
			{
				dm[i].getString(securityID);
				if (indexSnpLst.find(securityID) == indexSnpLst.end())
				{
					indexSnpLst[securityID] = new INDEXSNAP();
					strncpy(indexSnpLst[securityID]->securityID, securityID, sizeof(indexSnpLst[securityID]->securityID));
				}
				indexsnap = indexSnpLst[securityID];
				snap0->date = indexsnap->date = date;
				snap0->time = indexsnap->time = time;
			}
			break;
		case 56:
			break;
		case 48:
			break;
		case 55:
			break;
		case 8503:
			break;
		case 1020:
			if (strncmp(MDStreamID, indexInfo, 2) == 0)
				dm[i].getInt(indexsnap->volume);
			break;
		case 8504:
			if (strncmp(MDStreamID, indexInfo, 2) == 0)
			{
				dm[i].getDecimalInt(amount, 5);
				indexsnap->amount = amount;
			}
			break;
		case 140:
			if (strncmp(MDStreamID, indexInfo, 2) == 0)
			{
				dm[i].getDecimalInt(price, 4);
				indexsnap->prevClosePx = price;
			}
			break;
		case 734:
			break;
		case 8506:
			break;
		case 268:
			if (strncmp(MDStreamID, indexInfo, 2) == 0)
			{
				if (!dm[i].getInt(noMDEntries))
				{
					ERROR("get MDEntries failed");
					return ;
				}
			}
			break;
		case 269:
			if (strncmp(MDStreamID, indexInfo, 2) == 0)
			{
				if (!dm[i].getString(mdEntryType))
				{
					ERROR("get security id failed!");
					return ;
				}
				price = 0L;
			}
			break;
		case 270:
			if (strncmp(MDStreamID, indexInfo, 2) == 0)
			{
				if(!dm[i].getDecimalInt(price, 4))
				{
					ERROR("get price error!");
					return ;
				}
				//trade price
				if (mdEntryType[0] == '2')
					indexsnap->lastPx = price;
				//open price
				else if (mdEntryType[0] == '4')
					indexsnap->openPx = price;
				//high price
				else if (mdEntryType[0] == '7')
					indexsnap->highPx = price;
				//low price
				else if (mdEntryType[0] == '8')
					indexsnap->lowPx = price;
				//close
				else if (strncmp(mdEntryType, "5", 1) == 0)
					indexsnap->lastPx = price;
				//change
				else if (strncmp(mdEntryType, "x3", 2) == 0)
					indexsnap->change = price;
				else if (strncmp(mdEntryType, "x5", 2) == 0)
					indexsnap->exchangeRate = price * 10000;
				//close price 2
				else if (strncmp(mdEntryType, "5b", 2) == 0)
					indexsnap->lastIndex2 = price;
				//close price 3
				else if (strncmp(mdEntryType, "5c", 2) == 0)
					indexsnap->lastIndex3 = price;
			}
			break;
		case 271:
			break;
		case 273:
			if (strncmp(MDStreamID, indexInfo, 2) == 0)
			{
				if(!dm[i].getString(mdEntryTime))
				{
					ERROR("get price error!");
					return ;
				}
				//change rate
				if (strncmp(mdEntryType, "x4", 2) == 0)
				{
					parse.parse(mdEntryTime);
					indexsnap->amountOfIncrease =  parse.getMantissa() * pow(10.0L, 4 + parse.getExponent());;
				}
			}
			break;
		case 290:
			break;
		case 8538:
			break;
		}
	}
	if (strncmp(MDStreamID, indexInfo, 2) == 0)
		update(reinterpret_cast<char*>(indexsnap), chinaSecurityIndexSnap);
}
