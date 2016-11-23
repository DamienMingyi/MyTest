/*
 * Sh2QuotHandle.cpp
 *
 *  Created on: Nov 8, 2016
 *      Author: level2
 */

#include "Sh2QuotHandle.h"

const char Sh2QuotHandle::stepBeginString[] = "STEP.1.0.0";
const char Sh2QuotHandle::beginInteger[] = "312403";

Sh2QuotHandle::Sh2QuotHandle(): snap0(NULL), sequence(0), tag10072(0), tag10142(0), date(0), time(0) {
	// TODO Auto-generated constructor stub
	memset(tag35, 0, sizeof(tag35));
	channelIndex[SET_NUM] = {0};
}

Sh2QuotHandle::~Sh2QuotHandle() {
	// TODO Auto-generated destructor stub
	for (StockSnap2List::iterator it = stockSnap2List.begin(); it != stockSnap2List.end(); ++ it)
		delete it->second;
}

void Sh2QuotHandle::handle(Message* msg)
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
			else
				WARN("begin string inconsistent!");
		}
		else
			handleFAST(msg->vectorDecoderMessage[i]);
	}
}

void Sh2QuotHandle::handleFAST(FastCodecs::DecoderMessage& dm)
{
    time_t recvTime = ::time(NULL);
	struct tm *tt=localtime(&recvTime);
	if (snap0 == NULL)
		return ;
	snap0->bid[1].price=(tt->tm_hour*10000+tt->tm_min*100+tt->tm_sec)*1000;
	snap0->bid[2].price = snap0->time;

	int msgType = atoi(tag35 + 2);
	int tagCount = 0;
	if (!dm[0].getInt(tagCount))
		return ;
	switch(msgType)
	{
	case 3201:
		handleTick(dm, tagCount);
		break;
	case 3202:
		handleMarketData(dm, tagCount);
		break;
	case 3113:
		handleIndex(dm, tagCount);
		break;
	case 3115:
		handleMarketOverview(dm, tagCount);
		break;
	case 3107:
		handleVirtualAuctionPrice(dm, tagCount);
		break;
	default:
		ERROR("unknown message type!");
		break;
	}
}

void Sh2QuotHandle::handleSTEP(FastCodecs::DecoderMessage& dm)
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
			dm[i].getString(tag35);
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

void Sh2QuotHandle::handleTick(FastCodecs::DecoderMessage& dm, int tagCount)
{
	int tradeChannel = 0;
	int tradeIndex = 0;
	char code[8];
	memset(code, 0, 8);
	STOCKSNAP2* stocksnap2 = NULL;
	for (int i = 1; i <= tagCount; ++ i)
	{
		switch(dm[i].getID())
		{
		case 10011:
			dm[i].getInt(tradeIndex);
			break;
		case 10115:
			dm[i].getInt(tradeChannel);
//			channelIndex[tradeChannel] = tradeIndex;
			break;
		case 48:
			dm[i].getString(code);
			if (stockSnap2List.find(code) == stockSnap2List.end())
			{
				stockSnap2List[code] = new STOCKSNAP2();
				strncpy(stockSnap2List[code]->securityID, code, sizeof(stockSnap2List[code]->securityID));
			}
			stocksnap2 = stockSnap2List[code];
			break;
		case 10013:
			dm[i].getInt(stocksnap2->tradeTime);
			break;
		case 10014:
			dm[i].getInt(stocksnap2->tradePrice);
			break;
		case 10015:
			dm[i].getInt(stocksnap2->tradeQty);
			break;
		case 10016:
			dm[i].getInt(stocksnap2->tradeMoney);
			break;
		case 10179:
			dm[i].getInt(stocksnap2->tradeBuyNo);
			break;
		case 10180:
			dm[i].getInt(stocksnap2->tradeSellNo);
			break;
		case 10192:
			dm[i].getString(stocksnap2->tradeBSFlag);
			break;
		default:
			break;
		}
	}
	update(reinterpret_cast<char*>(stocksnap2), shanghaiLevel2Tick);
}

void Sh2QuotHandle::handleMarketData(FastCodecs::DecoderMessage& dm, int tagCount)
{
	char code[8];
	memset(code, 0, 8);
	STOCKSNAP2* stocksnap2 = NULL;
	int64_t lastPx = 0;
	int imageStatus = 0;
	//bid 0 ,offer 1
	ORDER order[2][LEVEL_NUM];
	memset(order, 0, sizeof(ORDER) * 2 * LEVEL_NUM);
	int32_t noLevel[2];
	memset(noLevel, 0, sizeof(int32_t) * 2);
	//bid or offer
	int j = 0;
	//the level
	int l = -1;
	//the queue
	int q = -1;
	for (int i = 1; i <= tagCount; ++ i)
	{
		switch(dm[i].getID())
		{
		case 48:
			dm[i].getString(code);
			if (stockSnap2List.find(code) == stockSnap2List.end())
			{
				stockSnap2List[code] = new STOCKSNAP2();
				strncpy(stockSnap2List[code]->securityID, code, sizeof(stockSnap2List[code]->securityID));
				stockSnap2List[code]->corrupted = true;
			}
			stocksnap2 = stockSnap2List[code];
			snap0->date = stocksnap2->date = date;
			snap0->time = stocksnap2->time = time;
			break;
		case 10146:
			dm[i].getInt(imageStatus);
			if (imageStatus == FULL_IMAGE)
				stocksnap2->corrupted = false;
			break;
		case 140:
			dm[i].getInt(stocksnap2->prevClosePx);
			break;
		case 10018:
			dm[i].getInt(stocksnap2->openPx);
			break;
		case 332:
			dm[i].getInt(stocksnap2->highPx);
			break;
		case 333:
			dm[i].getInt(stocksnap2->lowPx);
			break;
		case 31:
			dm[i].getInt(stocksnap2->lastPx);
			break;
		case 10204:
			dm[i].getInt(lastPx);
			if (lastPx > 0)
				stocksnap2->lastPx = lastPx;
			break;
		case 10135:
			dm[i].getString(stocksnap2->instrumentStatus);
			break;
		case 8538:
			dm[i].getString(stocksnap2->tradingPhaseCode);
			break;
		case 8503:
			dm[i].getInt(stocksnap2->numTrades);
			break;
		case 387:
			dm[i].getInt(stocksnap2->volume);
			break;
		case 8504:
			dm[i].getInt(stocksnap2->amount);
			break;
		case 10043:
			dm[i].getInt(stocksnap2->totalBidQty);
			break;
		case 10039:
			dm[i].getInt(stocksnap2->weightedAvgBidPx);
			break;
		case 10116:
			dm[i].getInt(stocksnap2->altWeightedAvgBidPx);
			break;
		case 10044:
			dm[i].getInt(stocksnap2->totalOfferQty);
			break;
		case 10040:
			dm[i].getInt(stocksnap2->weightAvgOfferPx);
			break;
		case 10117:
			dm[i].getInt(stocksnap2->altWeightedAvgOfferPx);
			break;
		case 10057:
			dm[i].getInt(stocksnap2->iopv);
			break;
		case 10193:
			dm[i].getInt(stocksnap2->ETFBuyNumber);
			break;
		case 10194:
			dm[i].getInt(stocksnap2->ETFBuyAmount);
			break;
		case 10195:
			dm[i].getInt(stocksnap2->ETFBuyMoney);
			break;
		case 10196:
			dm[i].getInt(stocksnap2->ETFSellNumber);
			break;
		case 10197:
			dm[i].getInt(stocksnap2->ETFSellAmount);
			break;
		case 10198:
			dm[i].getInt(stocksnap2->ETFSellMoney);
			break;
		case 10060:
			dm[i].getInt(stocksnap2->yield2Maturity);
			break;
		case 10138:
			dm[i].getInt(stocksnap2->totalWarrantExecQty);
			break;
		case 10139:
			dm[i].getInt(stocksnap2->warLowerPx);
			break;
		case 10140:
			dm[i].getInt(stocksnap2->warUpperPx);
			break;
		case 10184:
			dm[i].getInt(stocksnap2->withdrawBuyNumber);
			break;
		case 10185:
			dm[i].getInt(stocksnap2->withdrawBuyAmount);
			break;
		case 10186:
			dm[i].getInt(stocksnap2->withdrawBuyMoney);
			break;
		case 10187:
			dm[i].getInt(stocksnap2->withdrawSellNumber);
			break;
		case 10188:
			dm[i].getInt(stocksnap2->withdrawSellAmount);
			break;
		case 10189:
			dm[i].getInt(stocksnap2->withdrawSellMoney);
			break;
		case 10190:
			dm[i].getInt(stocksnap2->totalBidNumber);
			break;
		case 10191:
			dm[i].getInt(stocksnap2->totalOfferNumber);
			break;
		case 10203:
			dm[i].getInt(stocksnap2->bidTradeMaxDuration);
			break;
		case 10202:
			dm[i].getInt(stocksnap2->offerTradeMaxDuration);
			break;
		case 10070:
			dm[i].getInt(stocksnap2->numBidOrders);
			break;
		case 10071:
			dm[i].getInt(stocksnap2->numOfferOrders);
			break;
		case 10068:
			if (stocksnap2->lastPx <= stocksnap2->bid[0].price)
				stocksnap2->pin = 1;
			else
				stocksnap2->pin = 0;
			j = BID;
			dm[i].getInt(noLevel[j]);
			break;
		case 10069:
			j = OFFER;
			dm[i].getInt(noLevel[j]);
			l = -1;
			break;
		case 10147:
			if (imageStatus == UPDATED_IMAGE)	//update image
			{
				if (++ l >= LEVEL_NUM)
				{
					stocksnap2->corrupted = true;
					return ;
				}
				dm[i].getInt(order[j][l].priceLevelOperator);
			}
			break;
		case 44:
			if (imageStatus == FULL_IMAGE)	//full image
			{
				if (++ l >= LEVEL_NUM)
				{
					stocksnap2->corrupted = true;
					return ;
				}
			}
			dm[i].getInt(order[j][l].price);
			break;
		case 39:
			dm[i].getInt(order[j][l].orderQty);
			break;
		case 10067:
			dm[i].getInt(order[j][l].numOrders);
			break;
		case 73:
			if (order[j][l].priceLevelOperator != DELETE)
			{
				dm[i].getInt(order[j][l].noOrders);
				q = -1;
			}
			break;
		case 10148:
			dm[i].getInt(order[j][l].queue[++ q].orderQueueOperator);
			break;
		case 10149:
			dm[i].getInt(order[j][l].queue[q].orderQueueOperatorEntryID);
			break;
		case 38:
			if (imageStatus == 1 || (imageStatus == 2 && order[j][l].priceLevelOperator == 1))
			{
				if (++q >= Q_LENGTH)
				{
					stocksnap2->corrupted = true;
					return ;
				}
			}
			dm[i].getInt(order[j][l].queue[q].orderQty);
			break;
		}
	}
	if (stocksnap2->corrupted == false)
	{
		if (!booq.sort(stocksnap2, order, noLevel, imageStatus))
		{
			stocksnap2->corrupted = true;
			return ;
		}
		update(reinterpret_cast<char*>(stocksnap2), shanghaiLevel2Snap);
	}
}

void Sh2QuotHandle::handleIndex(FastCodecs::DecoderMessage& dm, int tagCount)
{
	char code[8];
	memset(code, 0, 8);
	STOCKSNAP2* stocksnap2 = NULL;
	int64_t lastPx = 0;
	for (int i = 1; i <= tagCount; ++ i)
	{
		switch(dm[i].getID())
		{
		case 48:
			dm[i].getString(code);
			if (stockSnap2List.find(code) == stockSnap2List.end())
			{
				stockSnap2List[code] = new STOCKSNAP2();
				strncpy(stockSnap2List[code]->securityID, code, sizeof(stockSnap2List[code]->securityID));
			}
			stocksnap2 = stockSnap2List[code];
			break;
		case 10007:
			dm[i].getInt(stocksnap2->prevClosePx);
			break;
		case 10006:
			dm[i].getInt(stocksnap2->openPx);
			break;
		case 10118:
			dm[i].getInt(stocksnap2->amount);
			break;
		case 10009:
			dm[i].getInt(stocksnap2->highPx);
			break;
		case 10010:
			dm[i].getInt(stocksnap2->lowPx);
			break;
		case 10008:
			dm[i].getInt(stocksnap2->lastPx);
			break;
		case 10013:
			dm[i].getInt(stocksnap2->time);
			break;
		case 387:
			dm[i].getInt(stocksnap2->volume);
			break;
		case 10205:
			dm[i].getInt(lastPx);
			if (lastPx > 0)
				stocksnap2->lastPx = lastPx;
			break;
		}
	}
	update(reinterpret_cast<char*>(stocksnap2), shanghaiLevel2Index);
}

void Sh2QuotHandle::handleVirtualAuctionPrice(FastCodecs::DecoderMessage& dm, int tagCount)
{
	char code[8];
	memset(code, 0, 8);
	STOCKSNAP2* stocksnap2 = NULL;
	for (int i = 1; i <= tagCount; ++ i)
	{
		switch(dm[i].getID())
		{
		case 48:
			dm[i].getString(code);
			if (stockSnap2List.find(code) == stockSnap2List.end())
			{
				stockSnap2List[code] = new STOCKSNAP2();
				strncpy(stockSnap2List[code]->securityID, code, sizeof(stockSnap2List[code]->securityID));
			}
			stocksnap2 = stockSnap2List[code];
			snap0->date = stocksnap2->date = date;
			snap0->time = stocksnap2->time = time;
			break;
		case 44:
			dm[i].getInt(stocksnap2->vap.price);
			break;
		case 10127:
			dm[i].getInt(stocksnap2->vap.virtualAuctionQty);
			break;
		case 151:
			dm[i].getInt(stocksnap2->vap.leaveQty);
			break;
		case 54:
			dm[i].getString(stocksnap2->vap.side);
			break;
		default:
			break;
		}
	}
	update(reinterpret_cast<char*>(stocksnap2), shanghaiLevel2Auction);
}

void Sh2QuotHandle::handleMarketOverview(FastCodecs::DecoderMessage& dm, int tagCount)
{
	char code[8];
	memset(code, 0, 8);
	for (int i = 1; i <= tagCount; ++ i)
	{
		switch(dm[i].getID())
		{
		case 48:
			dm[i].getString(code);
			break;
		case 42:
			if (strcmp("000000", code) == 0)
				dm[i].getInt(time);
			break;
		case 10003:
			if (strcmp("000000", code) == 0)
				dm[i].getInt(date);
			break;
		default:
			break;
		}
	}

}
