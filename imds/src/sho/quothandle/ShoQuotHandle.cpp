/*
 * ShoQuotHandle.cpp
 *
 *  Created on: Sep 14, 2016
 *      Author: level2
 */

#include "ShoQuotHandle.h"

const char ShoQuotHandle::stepBeginString[] = "STEP.1.0.0";
const char ShoQuotHandle::ezeiBeginString[] = "FIXT.1.1";
const char ShoQuotHandle::beginInteger[] = "312403";

ShoQuotHandle::ShoQuotHandle(): snap0(NULL), sequence(0), tag10072(0), tag10142(0), date(0), time(0) {
	// TODO Auto-generated constructor stub
	memset(this->securityType, 0, sizeof(this->securityType));
	memset(this->tradingSessionID, 0, sizeof(this->tradingSessionID));
}

ShoQuotHandle::~ShoQuotHandle() {
	// TODO Auto-generated destructor stub
	for (OptionSnapList::iterator it = optionSnpLst.begin(); it != optionSnpLst.end(); ++ it)
		delete it->second;
}

void ShoQuotHandle::handle(Message* msg)
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

void ShoQuotHandle::handleSTEP(FastCodecs::DecoderMessage& dm)
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

void ShoQuotHandle::handleEzEI(FastCodecs::DecoderMessage& dm)
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


void ShoQuotHandle::handleFAST(FastCodecs::DecoderMessage& dm)
{
	if (strncmp(this->securityType, "02", 2) == 0)
	{
		if (snap0 == NULL)
			return ;
        time_t recvTime = ::time(NULL);
		struct tm *tt=localtime(&recvTime);
		snap0->offer[1].price=(tt->tm_hour*10000+tt->tm_min*100+tt->tm_sec)*1000;
		snap0->offer[2].price = snap0->time;

		handleOptionFAST(dm);

	}
	else if (strncmp(this->securityType, "01", 2) == 0)
	{
		if (snap0 == NULL)
			return ;
        time_t recvTime = ::time(NULL);
		struct tm *tt=localtime(&recvTime);
		snap0->bid[1].price=(tt->tm_hour*10000+tt->tm_min*100+tt->tm_sec)*1000;
		snap0->bid[2].price = snap0->time;
		handleStockFAST(dm);
	}
}

void ShoQuotHandle::handleOptionFAST(FastCodecs::DecoderMessage& dm)
{
	int tagCount = 0;
	if (!dm[0].getInt(tagCount))
		return ;
	char securityID[16];
	int noMDEntries = 0;
	char mdEntryType[1];
	FastCodecs::int64_t price = 0L;
	FastCodecs::int64_t amount = 0L;
	int64_t volume = 0;
	int64_t tm = 0;
	SHOSNAP* shosnap = NULL;
	for (int i = 1; i <= tagCount; ++ i)
	{
		switch(dm[i].getID())
		{
		case 1500:
			break;
		case 48:
			memset(securityID, 0, 16);
			dm[i].getString(securityID);
			if (optionSnpLst.find(securityID) == optionSnpLst.end())
			{
				optionSnpLst[securityID] = new SHOSNAP();
				strncpy(optionSnpLst[securityID]->securityID, securityID, sizeof(optionSnpLst[securityID]->securityID));
			}
			shosnap = optionSnpLst[securityID];
			strncpy(shosnap->tradingSessionID, this->tradingSessionID, sizeof(this->tradingSessionID));
			if (snap0 == NULL)
				return ;
			snap0->date = shosnap->date = date;
			snap0->time = shosnap->time = time;

			break;
		case 55:
			break;
		case 8503:
			dm[i].getInt(shosnap->numTrades);
			break;
		case 1020:
			dm[i].getInt(shosnap->volume);
			break;
		case 8504:
			dm[i].getDecimalInt(amount);
			shosnap->amount = amount;
			break;
		case 140:
			dm[i].getDecimalInt(price, 3);
			shosnap->prevClosePx = price;
			break;
		case 734:
			dm[i].getDecimalInt(price, 3);
			shosnap->prevSettlePrice = price;
			break;
		case 8506:
			dm[i].getInt(shosnap->openInterest);
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
				shosnap->lastPx = price;
				if (shosnap->lastPx <= shosnap->bid[0].price)
					shosnap->pin = 1;
				else
					shosnap->pin = 0;
				break;
			case '3':
				break;
			case '4':
				shosnap->openPx = price;
				break;
			case '5':
				shosnap->lastPx = price;
				break;
			case '6':
				break;
			case '7':
				shosnap->highPx = price;
				break;
			case '8':
				shosnap->lowPx = price;
				break;
			case 'q':
				break;
			case 'v':
				break;
			case 'w':
				break;
			case 'x':
				shosnap->auctionPx = price;
				break;
			}
			break;
		case 271:
			dm[i].getInt(volume);
			switch(mdEntryType[0])
			{
			case '2':
				shosnap->lastVolume = volume;
				break;
			case 'x':
				shosnap->auctionPx = volume;
				break;
			}
			break;
		case 273:
			dm[i].getInt(tm);
			if (mdEntryType[0] == 'q')
				shosnap->lastestEnquiryTime = tm;
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
					shosnap->bid[level].price = price;
					shosnap->bid[level].size = volume;
					break;
				case '1':
					shosnap->offer[level].price = price;
					shosnap->offer[level].size = volume;
					break;
				}
			}
			break;
		case 8538:
			dm[i].getString(shosnap->tradingPhaseCode);
			break;
		default:
			break;
		}
	}
	StockPx::iterator it = stockpx.find(shosnap->underlyingSecurityID);
	if (it != stockpx.end())
		shosnap->stockLastPx = it->second;
	update(reinterpret_cast<char*>(shosnap), shanghaiOptionSnap);
}

void ShoQuotHandle::handleStockFAST(FastCodecs::DecoderMessage& dm)
{
	int tagCount = 0;
	if (!dm[0].getInt(tagCount))
		return ;
	char securityID[8];
	int noMDEntries = 0;
	char mdEntryType[1];
	FastCodecs::int64_t price = 0;
	for (int i = 1; i <= tagCount; ++ i)
	{
		switch(dm[i].getID())
		{
		case 1500:
			break;
		case 48:
			memset(securityID, 0, 8);
			if (!dm[i].getString(securityID))
			{
				ERROR("get security id failed!");
				return ;
			}
			break;
		case 55:
			break;
		case 8503:
			break;
		case 1020:
			break;
		case 8504:
			break;
		case 140:
			break;
		case 734:
			break;
		case 8506:
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
				stockpx[securityID] = price * 10;
				break;
			case '3':
				stockpx[securityID] = price * 10;
				break;
			case '4':
				break;
			case '5':
				stockpx[securityID] = price * 10;
				break;
			case '6':
				break;
			case '7':
				break;
			case '8':
				break;
			case 'v':
				break;
			case 'w':
				break;
			case 'x':
				break;
			}
			break;
		case 271:
			break;
		case 273:
			break;
		case 290:
			break;
		case 8538:
			break;
		default:
			break;
		}
	}
}

bool ShoQuotHandle::checkSequence(int seq, int sequence)
{
	if (sequence == 0)
		sequence = seq;
	else
	{
		++ sequence;
		if (sequence < seq)
		{
			WARN("message lost!");
			return false;
		}
		else if (sequence > seq)
		{
			if (seq == 0)
				sequence = seq;
			else
			{
				ERROR("message sequence error!");
				return false;
			}
		}
	}
	return true;
}
