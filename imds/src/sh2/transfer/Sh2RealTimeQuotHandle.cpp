/*
 * Sh2RealTimeQuotHandle.cpp
 *
 *  Created on: Nov 8, 2016
 *      Author: level2
 */

#include "Sh2RealTimeQuotHandle.h"

Sh2RealTimeQuotHandle::Sh2RealTimeQuotHandle(): market(NULL)  {
	// TODO Auto-generated constructor stub

}

Sh2RealTimeQuotHandle::~Sh2RealTimeQuotHandle() {
	// TODO Auto-generated destructor stub
	if (market != NULL)
		delete market;
}

bool Sh2RealTimeQuotHandle::load(const char* shm_name)
{
	if (shmEnv.openShmEnv(shm_name) != SUCCEED)
	{
		ERROR("openShmEnv(%s) error\r\n", shm_name);
		return false;
	}
	if (market != NULL)
		delete market;
	market = new Market("sh2", &shmEnv, &snap);
	if (!market->load(sizeof(STOCKSNAP2), sizeof(STOCKLINE2), sizeof(STOCKTRADE2), sizeof(STOCK2TRADE2)))
	{
		ERROR("market(sh2) load error");
		return false;
	}
	snap0 = reinterpret_cast<STOCKSNAP2*>(market->getSnap(0));
	return true;
}

void Sh2RealTimeQuotHandle::update(char* data, QuotType qt)
{
	STOCKSNAP2* dt = reinterpret_cast<STOCKSNAP2*>(data);
	STOCKSNAP2* pData = (STOCKSNAP2*)market->getSnap(dt->securityID);
	if (pData)
	{
		pData->date = dt->date;
		pData->time = dt->time / 100;
		strncpy(pData->securityID, dt->securityID, sizeof(pData->securityID));
		switch(qt)
		{
		case shanghaiLevel2Snap:
			pData->openPx = dt->openPx;
			pData->highPx = dt->highPx;
			pData->lowPx = dt->lowPx;
			pData->lastPx = dt->lastPx;
			strncpy(pData->instrumentStatus, dt->instrumentStatus, sizeof(dt->instrumentStatus));
			strncpy(pData->tradingPhaseCode, dt->tradingPhaseCode, sizeof(dt->tradingPhaseCode));
			pData->numTrades = dt->numTrades;
			pData->volume = dt->volume;
			pData->amount = dt->amount;

			pData->totalBidQty = dt->totalBidQty;
			pData->weightedAvgBidPx = dt->weightedAvgBidPx;
			pData->altWeightedAvgBidPx = dt->altWeightedAvgBidPx;

			pData->totalOfferQty = dt->totalOfferQty;
			pData->weightAvgOfferPx = dt->weightAvgOfferPx;
			pData->altWeightedAvgOfferPx = dt->altWeightedAvgOfferPx;

			pData->iopv = dt->iopv;
			pData->ETFBuyNumber = dt->ETFBuyNumber;
			pData->ETFBuyAmount = dt->ETFBuyAmount;
			pData->ETFBuyMoney = dt->ETFBuyMoney;

			pData->ETFSellNumber = dt->ETFSellNumber;
			pData->ETFSellAmount = dt->ETFSellAmount;
			pData->ETFSellMoney = dt->ETFSellMoney;
			pData->yield2Maturity = dt->yield2Maturity;
			pData->totalWarrantExecQty = dt->totalWarrantExecQty;
			pData->warLowerPx = dt->warLowerPx;
			pData->warUpperPx = dt->warUpperPx;

			pData->withdrawBuyNumber = dt->withdrawBuyNumber;
			pData->withdrawBuyAmount = dt->withdrawBuyAmount;
			pData->withdrawBuyMoney = dt->withdrawBuyMoney;
			pData->withdrawSellNumber = dt->withdrawSellNumber;
			pData->withdrawSellAmount = dt->withdrawSellAmount;
			pData->withdrawSellMoney = dt->withdrawSellMoney;
			pData->totalBidNumber = dt->totalBidNumber;
			pData->totalOfferNumber = dt->totalOfferNumber;
			pData->bidTradeMaxDuration = dt->bidTradeMaxDuration;
			pData->offerTradeMaxDuration = dt->offerTradeMaxDuration;
			pData->numBidOrders = dt->numBidOrders;
			pData->numOfferOrders = dt->numOfferOrders;
			for (int i = 0; i < 10; ++ i)
			{
				pData->bid[i].price = dt->bid[i].price;
				pData->bid[i].orderQty = dt->bid[i].orderQty;
				pData->bid[i].numOrders = dt->bid[i].numOrders;
				pData->offer[i].price = dt->offer[i].price;
				pData->offer[i].orderQty = dt->offer[i].orderQty;
				pData->offer[i].numOrders = dt->offer[i].numOrders;
			}
			for (int i = 0; i < 50; ++ i)
			{
				pData->bid[0].order[i] = dt->bid[0].order[i];
				pData->offer[0].order[i] = dt->offer[0].order[i];
			}
			//update
			snap.attach(reinterpret_cast<char*>(pData));
			snap.update();
			market->update(reinterpret_cast<char*>(pData));
			break;
		case shanghaiLevel2Tick:
			pData->tradeTime = dt->tradeTime;
			pData->tradePrice = dt->tradePrice;
			pData->tradeQty = dt->tradeQty;
			pData->tradeMoney = dt->tradeMoney;
			pData->tradeBuyNo = dt->tradeBuyNo;
			pData->tradeSellNo = dt->tradeSellNo;
			strncpy(pData->tradeBSFlag, dt->tradeBSFlag, sizeof(pData->tradeBSFlag));
			//update
			snap.attach(reinterpret_cast<char*>(pData));
			snap.tradeUpdate2();
			market->updateTick(reinterpret_cast<char*>(pData));
			break;
		case shanghaiLevel2Auction:
			if (strcmp(dt->instrumentStatus, "BETW") != 0)
			{
				pData->bid[0].price = pData->offer[0].price = dt->vap.price;
				pData->bid[0].orderQty = pData->offer[0].orderQty = dt->vap.virtualAuctionQty;
				switch(atoi(dt->vap.side))
				{
				case 0:
					pData->bid[1].orderQty = pData->offer[1].orderQty = 0;
					break;
				case 1:
					pData->bid[1].orderQty = dt->vap.leaveQty / 1000;
					pData->offer[1].orderQty = 0;
					break;
				case 2:
					pData->bid[1].orderQty = 0;
					pData->offer[1].orderQty = dt->vap.leaveQty / 1000;
					break;
				default:
					ERROR("obtaining virutal auction price failed!");
					break;
				}
			}
			//update
			snap.attach(reinterpret_cast<char*>(pData));
			snap.update();
			market->update(reinterpret_cast<char*>(pData));
			break;
		case shanghaiLevel2Index:
			pData->openPx = dt->openPx;
			pData->highPx = dt->highPx;
			pData->lowPx = dt->lowPx;
			pData->lastPx = dt->lastPx;
			pData->amount = dt->amount;
			pData->volume = dt->volume;
			//update
			snap.attach(reinterpret_cast<char*>(pData));
			snap.update();
			market->update(reinterpret_cast<char*>(pData));
			break;
		default:
			break;
		}
	}
}
