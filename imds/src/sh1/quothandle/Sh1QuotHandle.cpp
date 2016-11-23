/*
 * Sh1QuotHandle.cpp
 *
 *  Created on: Oct 14, 2016
 *      Author: level2
 */

#include "Sh1QuotHandle.h"

const char Sh1QuotHandle::stepBeginString[] = "STEP.1.0.0";
const char Sh1QuotHandle::ezeiBeginString[] = "FIXT.1.1";
const char Sh1QuotHandle::beginInteger[] = "312403";

Sh1QuotHandle::Sh1QuotHandle(): snap0(NULL), sequence(0), tag10072(0), tag10142(0), date(0), time(0)  {
	// TODO Auto-generated constructor stub
	memset(tradingSessionID, 0, sizeof(tradingSessionID));
	memset(securityType, 0, sizeof(securityType));
}

Sh1QuotHandle::~Sh1QuotHandle() {
	// TODO Auto-generated destructor stub
	for (StockSnapList::iterator it = stockSnpLst.begin(); it != stockSnpLst.end(); ++ it)
		delete it->second;
}

void Sh1QuotHandle::handle(Message* msg)
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

void Sh1QuotHandle::handleSTEP(FastCodecs::DecoderMessage& dm)
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

void Sh1QuotHandle::handleEzEI(FastCodecs::DecoderMessage& dm)
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
		case 336:
			dm[i].getString(this->tradingSessionID);
			break;
		case 393:
			break;
		default:
			break;
		}
	}
}

void Sh1QuotHandle::handleFAST(FastCodecs::DecoderMessage& dm)
{
    time_t recvTime = ::time(NULL);
	struct tm *tt=localtime(&recvTime);
	if (snap0 == NULL)
		return ;
	snap0->bid[1].price=(tt->tm_hour*10000+tt->tm_min*100+tt->tm_sec)*1000;
	snap0->bid[2].price = snap0->time;

	int tagCount = 0;
	if (!dm[0].getInt(tagCount))
		return ;
	char securityID[16];
	int noMDEntries = 0;
	char mdEntryType[1];
	FastCodecs::int64_t price = 0L;
	FastCodecs::int64_t amount = 0L;
	int i = 1;
	long volume = 0;
	STOCKSNAP* stocksnap = NULL;
	while (i <= tagCount)
	{
		switch (dm[i].getID())
		{
		case 1500:
			break;
		case 48:
			memset(securityID, 0, 16);
			dm[i].getString(securityID);
			if (stockSnpLst.find(securityID) == stockSnpLst.end())
			{
				stockSnpLst[securityID] = new STOCKSNAP();
				strncpy(stockSnpLst[securityID]->securityID, securityID, sizeof(stockSnpLst[securityID]->securityID));
			}
			stocksnap = stockSnpLst[securityID];
			strncpy(stocksnap->tradingSessionID, this->tradingSessionID, sizeof(this->tradingSessionID));
			if (snap0 == NULL)
				return ;
			snap0->date = stocksnap->date = date;
			snap0->time = stocksnap->time = time;
			break;
		case 55:
			break;
		case 8503:
			dm[i].getInt(stocksnap->numTrades);
			break;
		case 1020:
			dm[i].getInt(stocksnap->volume);
			break;
		case 8504:
			dm[i].getDecimalInt(amount);
			stocksnap->amount = amount;
			break;
		case 140:
			dm[i].getDecimalInt(price, 3);
			stocksnap->prevClosePx = price;
			break;
		case 268:
			if (!dm[i].getInt(noMDEntries))
			{
				ERROR("get MDEntries failed");
				return ;
			}
			break;
		case 269:
			if (!dm[i].getString(mdEntryType))
			{
				ERROR("get security id failed!");
				return ;
			}
			price = 0L;
			break;
		case 270:
			if(!dm[i].getDecimalInt(price, 3))
			{
				ERROR("get price error!");
				return ;
			}
			switch(mdEntryType[0])
			{
			case '0':
				break;
			case '1':
				break;
			case '2':
				stocksnap->lastPx = price;
				if (stocksnap->lastPx <= stocksnap->bid[0].price)
					stocksnap->pin = 1;
				else
					stocksnap->pin = 0;
				break;
			case '3':
				stocksnap->lastPx = price;
				break;
			case '4':
				stocksnap->openPx = price;
				break;
			case '5':
				stocksnap->lastPx = price;
				if (stocksnap->lastPx <= stocksnap->bid[0].price)
					stocksnap->pin = 1;
				else
					stocksnap->pin = 0;
				break;
			case '6':
				break;
			case '7':
				stocksnap->highPx = price;
				break;
			case '8':
				stocksnap->lowPx = price;
				break;
			case 'v':
				stocksnap->iopv = price;
				break;
			case 'w':
				stocksnap->nav = price;
				break;
			case 'x':
				break;
			}
			break;
		case 271:
			if (!dm[i].getInt(volume))
			{
				ERROR("get volume error!");
				return ;
			}
			break;
		case 273:
			break;
		case 290:
			{
				int level = 0;
				if (!dm[i].getInt(level))
				{
					ERROR("get level error!");
					return ;
				}
				switch(mdEntryType[0])
				{
				case '0':
					stocksnap->bid[level].price = price;
					stocksnap->bid[level].size = volume;
					break;
				case '1':
					stocksnap->offer[level].price = price;
					stocksnap->offer[level].size = volume;
					break;
				}
			}
			break;
		case 8538:
			dm[i].getString(stocksnap->tradingPhaseCode);
			break;
		}
		++i;
	}
	update(reinterpret_cast<char*>(stocksnap), shanghaiLevel1Snap);
}

bool Sh1QuotHandle::checkSequence(int seq, int sequence)
{
	return true;
}
