/*
 * Sz1v5PlatSort.cpp
 *
 *  Created on: 2016年10月21日
 *      Author: mds
 */

#include "Sz1v5PlatSort.h"

Sz1v5PlatSort::Sz1v5PlatSort() {
	// TODO Auto-generated constructor stub
	market = NULL;
}

Sz1v5PlatSort::~Sz1v5PlatSort() {
	// TODO Auto-generated destructor stub
}

bool Sz1v5PlatSort::load(char* shm_name)
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
		ERROR("market(sz1) load error");
		return false;
	}
	return true;
}
void Sz1v5PlatSort::update()
{
	int i=0;
	SZSNAP* pstocksnap;
	map<string, SZSNAP> mSnap;

	while((pstocksnap = (SZSNAP*)market->getSnap(++i))!=NULL)
	{
		if(pstocksnap->sectiontype == '\0')
		{
			if(strlen(pstocksnap->basicinfo.regionCode) != 0)
			{
				mSnap[pstocksnap->basicinfo.regionCode].interestRate++;
				mSnap[pstocksnap->basicinfo.regionCode].prevClosePx += pstocksnap->prevClosePx;
				mSnap[pstocksnap->basicinfo.regionCode].openPx += pstocksnap->openPx;
				mSnap[pstocksnap->basicinfo.regionCode].highPx += pstocksnap->highPx;
				mSnap[pstocksnap->basicinfo.regionCode].lowPx += pstocksnap->lowPx;
				mSnap[pstocksnap->basicinfo.regionCode].lastPx += pstocksnap->lastPx;
				mSnap[pstocksnap->basicinfo.regionCode].change += pstocksnap->change;
				mSnap[pstocksnap->basicinfo.regionCode].volume += pstocksnap->volume;
				mSnap[pstocksnap->basicinfo.regionCode].amount += pstocksnap->amount;
				mSnap[pstocksnap->basicinfo.regionCode].bid[0].price += pstocksnap->bid[0].price;
				mSnap[pstocksnap->basicinfo.regionCode].offer[0].price += pstocksnap->offer[0].price;
				mSnap[pstocksnap->basicinfo.regionCode].hand = pstocksnap->hand;
				mSnap[pstocksnap->basicinfo.regionCode].out = pstocksnap->out;
				mSnap[pstocksnap->basicinfo.regionCode].in = pstocksnap->in;
			}
			if(strlen(pstocksnap->basicinfo.industyCode) != 0)
			{
				mSnap[pstocksnap->basicinfo.industyCode].interestRate++;
				mSnap[pstocksnap->basicinfo.industyCode].prevClosePx += pstocksnap->prevClosePx;
				mSnap[pstocksnap->basicinfo.industyCode].openPx += pstocksnap->openPx;
				mSnap[pstocksnap->basicinfo.industyCode].highPx += pstocksnap->highPx;
				mSnap[pstocksnap->basicinfo.industyCode].lowPx += pstocksnap->lowPx;
				mSnap[pstocksnap->basicinfo.industyCode].lastPx += pstocksnap->lastPx;
				mSnap[pstocksnap->basicinfo.industyCode].change += pstocksnap->change;
				mSnap[pstocksnap->basicinfo.industyCode].volume += pstocksnap->volume;
				mSnap[pstocksnap->basicinfo.industyCode].amount += pstocksnap->amount;
				mSnap[pstocksnap->basicinfo.industyCode].bid[0].price += pstocksnap->bid[0].price;
				mSnap[pstocksnap->basicinfo.industyCode].offer[0].price += pstocksnap->offer[0].price;
				mSnap[pstocksnap->basicinfo.industyCode].hand = pstocksnap->hand;
				mSnap[pstocksnap->basicinfo.industyCode].out = pstocksnap->out;
				mSnap[pstocksnap->basicinfo.industyCode].in = pstocksnap->in;
			}

			if(strlen(pstocksnap->basicinfo.conceptCode) != 0)
			{
				char *ptr = strtok(pstocksnap->basicinfo.conceptCode,",");
				while(ptr != NULL)
				{
					mSnap[ptr].interestRate++;
					mSnap[ptr].prevClosePx += pstocksnap->prevClosePx;
					mSnap[ptr].openPx += pstocksnap->openPx;
					mSnap[ptr].highPx += pstocksnap->highPx;
					mSnap[ptr].lowPx += pstocksnap->lowPx;
					mSnap[ptr].lastPx += pstocksnap->lastPx;
					mSnap[ptr].change += pstocksnap->change;
					mSnap[ptr].volume += pstocksnap->volume;
					mSnap[ptr].amount += pstocksnap->amount;
					mSnap[ptr].bid[0].price += pstocksnap->bid[0].price;
					mSnap[ptr].offer[0].price += pstocksnap->offer[0].price;
					mSnap[ptr].hand = pstocksnap->hand;
					mSnap[ptr].out = pstocksnap->out;
					mSnap[ptr].in = pstocksnap->in;
					ptr = strtok(NULL,",");
				}
			}
		}
		if(pstocksnap->sectiontype != '\0')
		{
			if(mSnap.find(pstocksnap->securityID) != mSnap.end())
			{
				if(mSnap[pstocksnap->securityID].interestRate > 0)
				{
					INFO("%s -%d",pstocksnap->securityID, mSnap[pstocksnap->securityID].interestRate);
					pstocksnap->prevClosePx = mSnap[pstocksnap->securityID].prevClosePx/mSnap[pstocksnap->securityID].interestRate;
					pstocksnap->openPx = mSnap[pstocksnap->securityID].openPx/mSnap[pstocksnap->securityID].interestRate;
					pstocksnap->highPx = mSnap[pstocksnap->securityID].highPx/mSnap[pstocksnap->securityID].interestRate;
					pstocksnap->lowPx = mSnap[pstocksnap->securityID].lowPx/mSnap[pstocksnap->securityID].interestRate;
					pstocksnap->lastPx = mSnap[pstocksnap->securityID].lastPx/mSnap[pstocksnap->securityID].interestRate;
					pstocksnap->change = mSnap[pstocksnap->securityID].change/mSnap[pstocksnap->securityID].interestRate;
					pstocksnap->volume = mSnap[pstocksnap->securityID].volume/mSnap[pstocksnap->securityID].interestRate;
					pstocksnap->bid[0].price = mSnap[pstocksnap->securityID].bid[0].price/mSnap[pstocksnap->securityID].interestRate;
					pstocksnap->offer[0].price = mSnap[pstocksnap->securityID].offer[0].price/mSnap[pstocksnap->securityID].interestRate;
					pstocksnap->hand = mSnap[pstocksnap->securityID].hand/mSnap[pstocksnap->securityID].interestRate;
					pstocksnap->out = mSnap[pstocksnap->securityID].out/mSnap[pstocksnap->securityID].interestRate;
					pstocksnap->in = mSnap[pstocksnap->securityID].in/mSnap[pstocksnap->securityID].interestRate;
				}
			}
		}
	}
}

