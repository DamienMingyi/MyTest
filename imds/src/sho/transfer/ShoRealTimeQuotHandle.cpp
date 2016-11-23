/*
 * ShoRealTimeQuotHandle.cpp
 *
 *  Created on: Oct 24, 2016
 *      Author: level2
 */

#include "ShoRealTimeQuotHandle.h"


ShoRealTimeQuotHandle::ShoRealTimeQuotHandle(): market(NULL) {
	// TODO Auto-generated constructor stub

}

ShoRealTimeQuotHandle::~ShoRealTimeQuotHandle() {
	// TODO Auto-generated destructor stub
	if (market != NULL)
		delete market;
}

bool ShoRealTimeQuotHandle::load(const char* shm_name)
{
	if (shmEnv.openShmEnv(shm_name) != SUCCEED)
	{
		ERROR("openShmEnv(%s) error\r\n", shm_name);
		return false;
	}
	if (market != NULL)
		delete market;
	market = new Market("sho", &shmEnv, &snap);
	if (!market->load(sizeof(SHOSNAP), sizeof(SHOLINE), sizeof(SHOTRADE)))
	{
		ERROR("market(sho) load error");
		return false;
	}
	snap0 = reinterpret_cast<SHOSNAP*>(market->getSnap(0));
	return true;
}

void ShoRealTimeQuotHandle::update(char* data, QuotType qt)
{
	SHOSNAP* dt = reinterpret_cast<SHOSNAP*>(data);
	SHOSNAP* pData = reinterpret_cast<SHOSNAP*>(market->getSnap(dt->securityID));

	if (pData)
	{
		pData->date = dt->date;
		pData->time = dt->time / 100;
		strncpy(pData->securityID, dt->securityID, sizeof(pData->securityID));

		pData->prevVolume = pData->volume;
		pData->prevAmount = pData->amount;
		pData->prevSnapHigh = pData->highPx;
		pData->prevSnapLow = pData->lowPx;

		pData->stockLastPx = dt->stockLastPx;
		pData->openPx = dt->openPx;
		pData->highPx = dt->highPx;
		pData->lowPx = dt->lowPx;
		pData->lastPx = dt->lastPx;
		pData->auctionPx = dt->auctionPx;
		pData->auctionQty = dt->auctionQty;
		pData->openInterest = dt->openInterest;
		pData->pin = dt->pin;
		for (int i = 0; i < 5; ++ i)
		{
			pData->bid[i].price = dt->bid[i].price;
			pData->bid[i].size = dt->bid[i].size;
			pData->offer[i].price = dt->offer[i].price;
			pData->offer[i].size = dt->offer[i].size;
		}
		pData->volume = dt->volume;
		pData->amount = dt->amount;
		strncpy(pData->tradingPhaseCode, dt->tradingPhaseCode, sizeof(pData->tradingPhaseCode));
		strncpy(pData->tradingSessionID, dt->tradingSessionID, sizeof(pData->tradingSessionID));
		strncpy(snap0->tradingSessionID, dt->tradingSessionID, sizeof(pData->tradingSessionID));
		pData->lastestEnquiryTime = dt->lastestEnquiryTime;
		snap.attach(reinterpret_cast<char*>(pData));
		snap.update();
		market->update(reinterpret_cast<char*>(pData));
	}
}
