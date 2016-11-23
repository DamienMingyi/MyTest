/*
 * Sh1RealTimeQuotHandle.cpp
 *
 *  Created on: Oct 25, 2016
 *      Author: level2
 */

#include "Sh1RealTimeQuotHandle.h"

Sh1RealTimeQuotHandle::Sh1RealTimeQuotHandle(): market(NULL) {
	// TODO Auto-generated constructor stub

}

Sh1RealTimeQuotHandle::~Sh1RealTimeQuotHandle() {
	// TODO Auto-generated destructor stub
	if (market != NULL)
		delete market;
}

bool Sh1RealTimeQuotHandle::load(const char* shm_name)
{
	if (shmEnv.openShmEnv(shm_name) != SUCCEED)
	{
		ERROR("openShmEnv(%s) error\r\n", shm_name);
		return false;
	}
	if (market != NULL)
		delete market;
	market = new Market("sh1", &shmEnv, &snap);
	if (!market->load(sizeof(STOCKSNAP), sizeof(STOCKLINE), sizeof(STOCKTRADE)))
	{
		ERROR("market(sh1) load error");
		return false;
	}
	snap0 = reinterpret_cast<STOCKSNAP*>(market->getSnap(0));
	return true;
}

void Sh1RealTimeQuotHandle::update(char* data, QuotType qt)
{
	STOCKSNAP* dt = reinterpret_cast<STOCKSNAP*>(data);
	STOCKSNAP* pData = (STOCKSNAP*)market->getSnap(dt->securityID);

	if (pData)
	{
		pData->date = dt->date;
		pData->time = dt->time / 100;
		strncpy(pData->securityID, dt->securityID, sizeof(pData->securityID));

		pData->prevVolume = pData->volume;
		pData->prevAmount = pData->amount;
		pData->prevSnapHigh = pData->highPx;
		pData->prevSnapLow = pData->lowPx;

		pData->openPx = dt->openPx;
		pData->highPx = dt->highPx;
		pData->lowPx = dt->lowPx;
		pData->lastPx = dt->lastPx;
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
		snap.attach(reinterpret_cast<char*>(pData));
		snap.update();
		market->update(reinterpret_cast<char*>(pData));
	}
}
