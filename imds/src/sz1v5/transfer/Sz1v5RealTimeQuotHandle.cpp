/*
 * Sz1v5RealTimeQuotHandle.cpp
 *
 *  Created on: Oct 26, 2016
 *      Author: level2
 */

#include "Sz1v5RealTimeQuotHandle.h"
#include <boost/algorithm/string.hpp>
Sz1v5RealTimeQuotHandle::Sz1v5RealTimeQuotHandle(): market(NULL) {
	// TODO Auto-generated constructor stub

}

Sz1v5RealTimeQuotHandle::~Sz1v5RealTimeQuotHandle() {
	// TODO Auto-generated destructor stub
	if (market != NULL)
		delete market;
}

bool Sz1v5RealTimeQuotHandle::load(const char* shm_name)
{
	if (shmEnv.openShmEnv(shm_name) != SUCCEED)
	{
		ERROR("openShmEnv(%s) error\r\n", shm_name);
		return false;
	}
	if (market != NULL)
		delete market;
	market = new Market("sz1", &shmEnv, &snap);
	if (!market->load(sizeof(SZSNAP), sizeof(SZ1LINE), sizeof(SZ1TRADE)))
	{
		ERROR("market(sz1v5) load error");
		return false;
	}
	snap0 = reinterpret_cast<SZSNAP*>(market->getSnap(0));
	return true;
}

void Sz1v5RealTimeQuotHandle::update(char* data, QuotType qt)
{
	SZSNAP* pData = NULL;
	SZSNAP* dt = reinterpret_cast<SZSNAP*>(data);
	pData = reinterpret_cast<SZSNAP*>(market->getSnap(dt->securityID));

	if (pData)
	{
		pData->date = dt->date;
		pData->time = dt->time / 1000;
		strncpy(pData->securityID, dt->securityID, sizeof(pData->securityID));

		pData->prevVolume = dt->prevVolume;
		pData->prevAmount = dt->prevAmount;
		pData->prevSnapHigh = dt->prevSnapHigh;
		pData->prevSnapLow = dt->prevSnapLow;

		pData->openPx = dt->openPx / 1000;
		pData->highPx = dt->highPx / 1000;
		pData->lowPx = dt->lowPx / 1000;
		pData->lastPx = dt->lastPx / 1000;

		pData->change = dt->change / 1000;
		pData->change2 = dt->change2 / 1000;
		pData->buySum = dt->buySum / 1000;
		pData->buySumVol = dt->buySumVol;
		pData->sellSum = dt->sellSum / 1000;
		pData->sellSumVol = dt->sellSumVol;
		pData->per1 = dt->per1 / 1000;
		pData->per2 = dt->per2 / 1000;
		pData->iopv = dt->iopv / 1000;
		pData->premiumRate = dt->premiumRate / 1000;
		pData->upLimit = dt->upLimit / 1000;
		pData->downLimit = dt->downLimit / 1000;
		pData->position = dt->position / 100;

		pData->pin = dt->pin;
		for (int i = 0; i < 5; ++ i)
		{
			pData->bid[i].price = dt->bid[i].price / 1000;
			pData->bid[i].size = dt->bid[i].size / 100;
			pData->offer[i].price = dt->offer[i].price / 1000;
			pData->offer[i].size = dt->offer[i].size / 100;
		}
		pData->volume = dt->volume / 100;
		pData->amount = dt->amount / 10000;
		pData->numTrades = dt->numTrades;
		strncpy(pData->tradingPhaseCode, dt->tradingPhaseCode, sizeof(pData->tradingPhaseCode));
		strncpy(pData->mdStreamID, dt->mdStreamID, sizeof(pData->mdStreamID));
		snap.attach(reinterpret_cast<char*>(pData));
		snap.update();
		market->update(reinterpret_cast<char*>(pData));
	}

}
