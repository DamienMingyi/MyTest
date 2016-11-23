/*
 * CSIRealTimeQuotHandle.cpp
 *
 *  Created on: Nov 15, 2016
 *      Author: level2
 */

#include "CSIRealTimeQuotHandle.h"

CSIRealTimeQuotHandle::CSIRealTimeQuotHandle(): market(NULL) {
	// TODO Auto-generated constructor stub

}

CSIRealTimeQuotHandle::~CSIRealTimeQuotHandle() {
	// TODO Auto-generated destructor stub
	if (market != NULL)
		delete market;
}

bool CSIRealTimeQuotHandle::load(const char* shm_name)
{
	if (shmEnv.openShmEnv(shm_name) != SUCCEED)
	{
		ERROR("openShmEnv(%s) error\r\n", shm_name);
		return false;
	}
	if (market != NULL)
		delete market;
	market = new Market("csi", &shmEnv, &snap);
	if (!market->load(sizeof(INDEXSNAP), sizeof(INDEXLINE), sizeof(INDEXTRADE)))
	{
		ERROR("market(csi) load error");
		return false;
	}
	snap0 = reinterpret_cast<INDEXSNAP*>(market->getSnap(0));
	return true;
}

void CSIRealTimeQuotHandle::update(char* data, QuotType qt)
{
	INDEXSNAP* dt = reinterpret_cast<INDEXSNAP*>(data);
	INDEXSNAP* pData = (INDEXSNAP*)market->getSnap(dt->securityID);

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

		pData->volume = dt->volume;
		pData->amount = dt->amount;

		pData->change = dt->change;
		pData->amountOfIncrease = dt->amountOfIncrease;
		pData->exchangeRate = dt->exchangeRate;
		pData->lastIndex2 = dt->lastIndex2;
		pData->lastIndex3 = dt->lastIndex3;
		snap.attach(reinterpret_cast<char*>(pData));
		snap.update();
		market->update(reinterpret_cast<char*>(pData));
	}
}
