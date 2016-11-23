/*
 * Sz1v5QuotHandle.cpp
 *
 *  Created on: Oct 26, 2016
 *      Author: level2
 */

#include "Sz1v5QuotHandle.h"
#include <ctime>
#include "../../public/szstock/SZv5Snap.h"
#include "../../public/protocol/SZV5Struct.h"
#include <arpa/inet.h>

const char Sz1v5QuotHandle::beginInteger[] = "312403";

Sz1v5QuotHandle::Sz1v5QuotHandle(): snap0(NULL), sequence(0), tag10072(0), tag10142(0) {
	// TODO Auto-generated constructor stub

}

Sz1v5QuotHandle::~Sz1v5QuotHandle() {
	// TODO Auto-generated destructor stub
	for (SzSnapList::iterator it = szSnpLst.begin(); it != szSnpLst.end(); ++ it)
		delete it->second;
}

void Sz1v5QuotHandle::handle(Message* msg)
{
	for (uint32_t i = 0; i < msg->vectorDecoderMessage.size(); ++ i)
	{
		int id = msg->vectorDecoderMessage[i][1].getID();
		if (id == 8)
		{
			char beginString[16];
			memset(beginString, 0, 16);
			msg->vectorDecoderMessage[i][1].getString(beginString);
			if (strncmp(beginString, beginInteger, sizeof(beginInteger)) == 0)
				handleSTEP(msg->vectorDecoderMessage[i]);
		}
	}
	if (msg->bf != NULL)
		handleSZ1V5(msg->bf);
}

void Sz1v5QuotHandle::handleSTEP(FastCodecs::DecoderMessage& dm)
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

void Sz1v5QuotHandle::handleSZ1V5(char* data)
{
	time_t recvTime = ::time(NULL);
	struct tm *tt=localtime(&recvTime);
	if (snap0 == NULL)
		return ;
	snap0->bid[1].price=(tt->tm_hour*10000+tt->tm_min*100+tt->tm_sec)*1000;
	snap0->bid[2].price = snap0->time;

	int32_t headLength = sizeof(MSG_HEAD);
	int32_t bodyLength = ntohl(reinterpret_cast <MSG_HEAD*>(data)->bodyLength);
	switch (ntohl(reinterpret_cast <MSG_HEAD*>(data)->msgType))
	{
	case 1:
		handleLogon(data + headLength, bodyLength);
		break;
	case 2:
		handleLogout(data + headLength, bodyLength);
		break;
	case 3:
		break;
	case 390095:
		break;
	case 390090:
		handleStatistic(data + headLength, bodyLength);
		break;
	case 390012:
		break;
	case 390013:
		handleStatus(data + headLength, bodyLength);
		break;
	case 300111:
		handleAuction(data + headLength, bodyLength);
		break;
	case 309011:
		handleIndex(data + headLength, bodyLength);
		break;
	case 309111:
		handleTradeVolume(data + headLength, bodyLength);
		break;
	default:
		break;
	}
}

void Sz1v5QuotHandle::handleLogon(char* buf, uint32_t bodyLength)
{
	INFO("senderCompID: %s, ", reinterpret_cast <LOGON_BODY*>(buf)->senderCompID);
	INFO("targetCompID: %s, ", reinterpret_cast <LOGON_BODY*>(buf)->targetCompID);
	int32_t heartBtInt = static_cast<int32_t>(ntohl(reinterpret_cast <LOGON_BODY*>(buf)->heartBtInt));
	INFO("heartBtInt: %d, ", heartBtInt);
	INFO("password: %s, ", reinterpret_cast <LOGON_BODY*>(buf)->password);
	INFO("defaultApplVerID: %s", reinterpret_cast <LOGON_BODY*>(buf)->defaultApplVerID);
}

void Sz1v5QuotHandle::handleLogout(char* buf, uint32_t bodyLength)
{
	INFO("sessionStatus: %d, ", static_cast<int32_t>(ntohl(reinterpret_cast <LOGOUT_BODY*>(buf)->sessionStatus)));
	INFO("text: %s", reinterpret_cast <LOGOUT_BODY*>(buf)->text);
	INFO("log out!");
//	in->swrite(buf - headLength, bodyLength + headLength + sizeof(checkSum));
//	sleep(10);
//	in->sclose();
	exit(0);
}

void Sz1v5QuotHandle::handleStatistic(char* buf, uint32_t bodyLength)
{
	if (bodyLength != sizeof(MARKET_DATA_STATISTIC_BODY) + sizeof(MARKET_DATA_CATEGORY) *
			ntohl(reinterpret_cast <MARKET_DATA_STATISTIC_BODY*>(buf)->noMDStreamID))
	{
		ERROR("statistic body length is not matched!");
		exit(1);
	}
}

void Sz1v5QuotHandle::handleStatus(char* buf, uint32_t bodyLength)
{
	if (bodyLength != sizeof(REAL_TIME_STATUS_BODY) + sizeof(SWITCH) * ntohl(reinterpret_cast <REAL_TIME_STATUS_BODY*>(buf)->noSwitch))
	{
		ERROR("statistic body length is not matched!");
		exit(1);
	}
}

void Sz1v5QuotHandle::handleAuction(char* buf, uint32_t bodyLength)
{
	SZSNAP* pData = NULL;
	uint32_t noMDEntries = handleSnap(buf, &pData);
	if (pData)
	{
//		AGGREGATION_AUCTION auction[noMDEntries];
		for (uint32_t i = 0; i < noMDEntries; ++ i)
		{
//			memcpy(&(auction[i]), buf + sizeof(SNAP) + sizeof(AGGREGATION_AUCTION) * i, sizeof(AGGREGATION_AUCTION));
			AGGREGATION_AUCTION* auction = reinterpret_cast<AGGREGATION_AUCTION*>(buf + sizeof(SNP) + sizeof(AGGREGATION_AUCTION) * i);
			if (auction->MDEntryType[0] == '0')
			{
				if (ntohs(auction->MDPriceLevel) == 1)
				{
					if (pData->lastPx <= pData->bid[0].price)
						pData->pin = 1;
					else
						pData->pin = 0;
				}
				pData->bid[ntohs(auction->MDPriceLevel) - 1].price = static_cast<int64_t>(ntohll(auction->MDEntryPx)) / 1000;
				pData->bid[ntohs(auction->MDPriceLevel) - 1].size = static_cast<int64_t>(ntohll(auction->MDEntrySize)) / 100;
			}
			else if (auction->MDEntryType[0] == '1')
			{
				pData->offer[ntohs(auction->MDPriceLevel) - 1].price = static_cast<int64_t>(ntohll(auction->MDEntryPx)) / 1000;
				pData->offer[ntohs(auction->MDPriceLevel) - 1].size = static_cast<int64_t>(ntohll(auction->MDEntrySize)) / 100;
			}
			else if (auction->MDEntryType[0] == '2')
			{
				pData->lastPx = static_cast<int64_t>(ntohll(auction->MDEntryPx));
			}
			else if (auction->MDEntryType[0] == '4')
			{
				pData->openPx = static_cast<int64_t>(ntohll(auction->MDEntryPx));
			}
			else if (auction->MDEntryType[0] == '7')
			{
				pData->highPx = static_cast<int64_t>(ntohll(auction->MDEntryPx));
			}
			else if (auction->MDEntryType[0] == '8')
			{
				pData->lowPx = static_cast<int64_t>(ntohll(auction->MDEntryPx));
			}
			else if (strncmp(auction->MDEntryType, "x1", 2) == 0)
			{
				pData->change = static_cast<int64_t>(ntohll(auction->MDEntryPx));
			}
			else if (strncmp(auction->MDEntryType, "x2", 2) == 0)
			{
				pData->change2 = static_cast<int64_t>(ntohll(auction->MDEntryPx));
			}
			else if (strncmp(auction->MDEntryType, "x3", 2) == 0)
			{
				pData->buySum = static_cast<int64_t>(ntohll(auction->MDEntryPx));
				pData->buySumVol = static_cast<int64_t>(ntohll(auction->MDEntrySize));
			}
			else if (strncmp(auction->MDEntryType, "x4", 2) == 0)
			{
				pData->sellSum = static_cast<int64_t>(ntohll(auction->MDEntryPx));
				pData->sellSumVol = static_cast<int64_t>(ntohll(auction->MDEntrySize));
			}
			else if (strncmp(auction->MDEntryType, "x5", 2) == 0)
			{
				pData->per1 = static_cast<int64_t>(ntohll(auction->MDEntryPx));
			}
			else if (strncmp(auction->MDEntryType, "x6", 2) == 0)
			{
				pData->per2 = static_cast<int64_t>(ntohll(auction->MDEntryPx));
			}
			else if (strncmp(auction->MDEntryType, "x7", 2) == 0)
			{
				if (pData->nav == 0)
					pData->nav = static_cast<int64_t>(ntohll(auction->MDEntryPx)) / 1000;
			}
			else if (strncmp(auction->MDEntryType, "x8", 2) == 0)
			{
				pData->iopv = static_cast<int64_t>(ntohll(auction->MDEntryPx));
			}
			else if (strncmp(auction->MDEntryType, "x9", 2) == 0)
			{
				pData->premiumRate = static_cast<int64_t>(ntohll(auction->MDEntryPx));
			}
			else if (strncmp(auction->MDEntryType, "xe", 2) == 0)
			{
				pData->upLimit = static_cast<int64_t>(ntohll(auction->MDEntryPx));
			}
			else if (strncmp(auction->MDEntryType, "xf", 2) == 0)
			{
				pData->downLimit = static_cast<int64_t>(ntohll(auction->MDEntryPx));
			}
			else if (strncmp(auction->MDEntryType, "xg", 2) == 0)
			{
				pData->position = static_cast<int64_t>(ntohll(auction->MDEntrySize));
			}
		}
		if (bodyLength != sizeof(SNP) + noMDEntries * sizeof(AGGREGATION_AUCTION))
		{
			ERROR("auction body length is not matched!");
			exit(1);
		}
	}
	update(reinterpret_cast<char*>(pData), shenzhenLevel1Snap);
}

void Sz1v5QuotHandle::handleIndex(char* buf, uint32_t bodyLength)
{
	SZSNAP* pData = NULL;
	uint32_t noMDEntries = handleSnap(buf, &pData);
	if (pData)
	{
//		INDEX index[noMDEntries];
		if (bodyLength != sizeof(SNP) + noMDEntries * sizeof(INDEX))
		{
			ERROR("index body length is not matched!\n");
			exit(1);
		}
//		memcpy(index, buf + sizeof(SNP), noMDEntries * sizeof(INDEX));
		for (uint32_t i = 0; i < noMDEntries; ++ i)
		{
			INDEX* index = reinterpret_cast<INDEX*>(buf + sizeof(SNP) + i * sizeof(INDEX));
			if (index->MDEntryType[0] == '3')
			{
				pData->lastPx = static_cast<int64_t>(ntohll(index->MDEntryPx));
			}
			else if (strncmp(index->MDEntryType, "xb", 2) == 0)
			{
				pData->openPx = static_cast<int64_t>(ntohll(index->MDEntryPx));
			}
			else if (strncmp(index->MDEntryType, "xc", 2) == 0)
			{
				pData->highPx = static_cast<int64_t>(ntohll(index->MDEntryPx));
			}
			else if (strncmp(index->MDEntryType, "xd", 2) == 0)
			{
				pData->lowPx = static_cast<int64_t>(ntohll(index->MDEntryPx));
			}
		}
	}
	update(reinterpret_cast<char*>(pData), shenzhenLevel1Index);
}

void Sz1v5QuotHandle::handleTradeVolume(char* buf, uint32_t bodyLength)
{
//	SZSNAP* pData = NULL;
//	uint32_t stockNum = handleSnap(buf, pData);
//	if (pData)
//	{
//		pData->stockNum = stockNum;
//	}
//	snap.attach(reinterpret_cast <char*>(pData));
}

uint32_t Sz1v5QuotHandle::handleSnap(char* buf, SZSNAP** szsnap)
{
	char code[8];
	memset(code, 0, 8);
	strncpy(code, reinterpret_cast <SNP*>(buf)->securityID, 6);
	if (szSnpLst.find(code) == szSnpLst.end())
	{
		szSnpLst[code] = new SZSNAP();
		strncpy(szSnpLst[code]->securityID, code, sizeof(szSnpLst[code]->securityID));
	}
	*szsnap = szSnpLst[code];
	if (snap0 == NULL)
		return -1;
	snap0->date = (*szsnap)->date = static_cast<int64_t>(ntohll(reinterpret_cast <SNP*>(buf)->origTime)) / 1000000000;
	snap0->time = (*szsnap)->time = static_cast<int64_t>(ntohll(reinterpret_cast <SNP*>(buf)->origTime)) % 1000000000;
	strncpy((*szsnap)->mdStreamID, reinterpret_cast <SNP*>(buf)->MDStreamID, 3);
	(*szsnap)->mdStreamID[4] = '\0';

	strncpy((*szsnap)->tradingPhaseCode, reinterpret_cast <SNP*>(buf)->tradingPhaseCode, 8);
	(*szsnap)->tradingPhaseCode[7] = '\0';

	if ((*szsnap)->prevClosePx == 0)
		(*szsnap)->prevClosePx = static_cast<int64_t>(ntohll(reinterpret_cast <SNP*>(buf)->prevClosePx));

	(*szsnap)->numTrades = static_cast<int64_t>(ntohll(reinterpret_cast <SNP*>(buf)->numTrades));
	(*szsnap)->prevVolume = (*szsnap)->volume;
	(*szsnap)->prevAmount = (*szsnap)->amount;
	(*szsnap)->volume = static_cast<int64_t>(ntohll(reinterpret_cast <SNP*>(buf)->totalVolumeTrade));
	(*szsnap)->amount = static_cast<int64_t>(ntohll(reinterpret_cast <SNP*>(buf)->totalValueTrade));
	return ntohl(reinterpret_cast <SNP*>(buf)->noMDEntries);
}

#ifndef swap64
/* Byte swap a 64-bit number. */
static inline uint64_t swap64(uint64_t in)
{
#ifndef WORDS_BIGENDIAN
  /* Little endian, flip the bytes around until someone makes a faster/better
   * way to do this. */
	uint64_t rv= 0;
	uint8_t x= 0;
	for(x= 0; x < 8; x++)
	{
		rv= (rv << 8) | (in & 0xff);
		in >>= 8;
	}
	return rv;

#else
  /* big-endian machines don't need byte swapping */
	return in;
#endif
}
#endif

uint64_t ntohll(uint64_t value)
{
	return swap64(value);
}

uint64_t htonll(uint64_t value)
{
	return swap64(value);
}

