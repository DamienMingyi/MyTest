/*
 * CrossPlatSort.cpp
 *
 *  Created on: 2016年10月24日
 *      Author: mds
 */

#include "cnPlatSort.h"

cnPlatSort::cnPlatSort() {
	// TODO Auto-generated constructor stub
	sh1market = NULL;
	sz1market = NULL;
	cnmarket = NULL;
}

cnPlatSort::~cnPlatSort() {
	// TODO Auto-generated destructor stub
}

bool cnPlatSort::load(const char* cross_shm_path,const char* sh1_shm_path, const char* sz1_shm_path)
{
	if(cnShmEnv.openShmEnv(cross_shm_path) != SUCCEED)
	{
		ERROR("openShmEnv(%s)  error\r\n", cross_shm_path);
		return false;
	}
	if(sh1ShmEnv.openShmEnv(sh1_shm_path) != SUCCEED)
	{
		ERROR("openShmEnv(%s)  error\r\n", sh1_shm_path);
		return false;
	}
	if(sz1ShmEnv.openShmEnv(sz1_shm_path) != SUCCEED)
	{
		ERROR("openShmEnv(%s)  error\r\n", sz1_shm_path);
		return false;
	}

	if(cnmarket != NULL)
		delete cnmarket;
	if(sh1market != NULL)
		delete sh1market;
	if(sz1market != NULL)
		delete sz1market;

	cnmarket = new Market("cross", &cnShmEnv, &cnsnap);
	if (!cnmarket->load(sizeof(SNAP), 1, 1))
	{
		ERROR("market(sh1) load error");
		return false;
	}
	sh1market = new Market("sh1", &sh1ShmEnv, &sh1snap);
	if (!sh1market->load(sizeof(STOCKSNAP), sizeof(STOCKLINE), sizeof(STOCKTRADE)))
	{
		ERROR("market(sh1) load error");
		return false;
	}
	sz1market = new Market("sz1", &sz1ShmEnv, &sz1snap);
	if (!sz1market->load(sizeof(SZSNAP), sizeof(SZ1LINE), sizeof(SZ1TRADE)))
	{
		ERROR("market(sz1) load error");
		return false;
	}
	return true;
}
void cnPlatSort::update()
{
	int i = 0;
	STOCKSNAP* pstocksnap;
	SZSNAP* pszsnap;
	CROSNAP* pcnsnap;
	map<string, STOCKSNAP> msh1Snap;
	map<string, SZSNAP> msz1Snap;
	map<string, CROSNAP> mcnSnap;

	while((pstocksnap = (STOCKSNAP*)sh1market->getSnap(++i))!=NULL)
	{
		if(pstocksnap->sectiontype == '\0')
		{
			if(strlen(pstocksnap->basicinfo.regionCode) != 0)
			{
				msh1Snap[pstocksnap->basicinfo.regionCode].interestRate++;
				msh1Snap[pstocksnap->basicinfo.regionCode].prevClosePx += pstocksnap->prevClosePx;
				msh1Snap[pstocksnap->basicinfo.regionCode].openPx += pstocksnap->openPx;
				msh1Snap[pstocksnap->basicinfo.regionCode].highPx += pstocksnap->highPx;
				msh1Snap[pstocksnap->basicinfo.regionCode].lowPx += pstocksnap->lowPx;
				msh1Snap[pstocksnap->basicinfo.regionCode].lastPx += pstocksnap->lastPx;
				msh1Snap[pstocksnap->basicinfo.regionCode].change += pstocksnap->change;
				msh1Snap[pstocksnap->basicinfo.regionCode].volume += pstocksnap->volume;
				msh1Snap[pstocksnap->basicinfo.regionCode].amount += pstocksnap->amount;
				msh1Snap[pstocksnap->basicinfo.regionCode].bid[0].price += pstocksnap->bid[0].price;
				msh1Snap[pstocksnap->basicinfo.regionCode].offer[0].price += pstocksnap->offer[0].price;
				msh1Snap[pstocksnap->basicinfo.regionCode].hand = pstocksnap->hand;
				msh1Snap[pstocksnap->basicinfo.regionCode].out = pstocksnap->out;
				msh1Snap[pstocksnap->basicinfo.regionCode].in = pstocksnap->in;
			}
			if(strlen(pstocksnap->basicinfo.industyCode) !=0 )
			{
				msh1Snap[pstocksnap->basicinfo.industyCode].interestRate++;
				msh1Snap[pstocksnap->basicinfo.industyCode].prevClosePx += pstocksnap->prevClosePx;
				msh1Snap[pstocksnap->basicinfo.industyCode].openPx += pstocksnap->openPx;
				msh1Snap[pstocksnap->basicinfo.industyCode].highPx += pstocksnap->highPx;
				msh1Snap[pstocksnap->basicinfo.industyCode].lowPx += pstocksnap->lowPx;
				msh1Snap[pstocksnap->basicinfo.industyCode].lastPx += pstocksnap->lastPx;
				msh1Snap[pstocksnap->basicinfo.industyCode].change += pstocksnap->change;
				msh1Snap[pstocksnap->basicinfo.industyCode].volume += pstocksnap->volume;
				msh1Snap[pstocksnap->basicinfo.industyCode].amount += pstocksnap->amount;
				msh1Snap[pstocksnap->basicinfo.industyCode].bid[0].price += pstocksnap->bid[0].price;
				msh1Snap[pstocksnap->basicinfo.industyCode].offer[0].price += pstocksnap->offer[0].price;
				msh1Snap[pstocksnap->basicinfo.industyCode].hand = pstocksnap->hand;
				msh1Snap[pstocksnap->basicinfo.industyCode].out = pstocksnap->out;
				msh1Snap[pstocksnap->basicinfo.industyCode].in = pstocksnap->in;
			}
			if(strlen(pstocksnap->basicinfo.conceptCode) !=0 )
			{
				char *ptr = strtok(pstocksnap->basicinfo.conceptCode,",");
				while(ptr != NULL)
				{
					msh1Snap[ptr].interestRate++;
					msh1Snap[ptr].prevClosePx += pstocksnap->prevClosePx;
					msh1Snap[ptr].openPx += pstocksnap->openPx;
					msh1Snap[ptr].highPx += pstocksnap->highPx;
					msh1Snap[ptr].lowPx += pstocksnap->lowPx;
					msh1Snap[ptr].lastPx += pstocksnap->lastPx;
					msh1Snap[ptr].change += pstocksnap->change;
					msh1Snap[ptr].volume += pstocksnap->volume;
					msh1Snap[ptr].amount += pstocksnap->amount;
					msh1Snap[ptr].bid[0].price += pstocksnap->bid[0].price;
					msh1Snap[ptr].offer[0].price += pstocksnap->offer[0].price;
					msh1Snap[ptr].hand = pstocksnap->hand;
					msh1Snap[ptr].out = pstocksnap->out;
					msh1Snap[ptr].in = pstocksnap->in;
					ptr = strtok(NULL, ",");
				}
			}
		}
		else
			break;
	}

	i=0;
	while((pszsnap = (SZSNAP*)sz1market->getSnap(++i))!=NULL)
	{
		if(pszsnap->sectiontype == '\0')
		{
			if(strlen(pszsnap->basicinfo.regionCode) != 0)
			{
				msz1Snap[pszsnap->basicinfo.regionCode].interestRate++;
				msz1Snap[pszsnap->basicinfo.regionCode].prevClosePx += pszsnap->prevClosePx;
				msz1Snap[pszsnap->basicinfo.regionCode].openPx += pszsnap->openPx;
				msz1Snap[pszsnap->basicinfo.regionCode].highPx += pszsnap->highPx;
				msz1Snap[pszsnap->basicinfo.regionCode].lowPx += pszsnap->lowPx;
				msz1Snap[pszsnap->basicinfo.regionCode].lastPx += pszsnap->lastPx;
				msz1Snap[pszsnap->basicinfo.regionCode].change += pszsnap->change;
				msz1Snap[pszsnap->basicinfo.regionCode].volume += pszsnap->volume;
				msz1Snap[pszsnap->basicinfo.regionCode].amount += pszsnap->amount;
				msz1Snap[pszsnap->basicinfo.regionCode].bid[0].price += pszsnap->bid[0].price;
				msz1Snap[pszsnap->basicinfo.regionCode].offer[0].price += pszsnap->offer[0].price;
				msz1Snap[pszsnap->basicinfo.regionCode].hand = pszsnap->hand;
				msz1Snap[pszsnap->basicinfo.regionCode].out = pszsnap->out;
				msz1Snap[pszsnap->basicinfo.regionCode].in = pszsnap->in;
			}
			if(strlen(pszsnap->basicinfo.industyCode) !=0 )
			{
				msz1Snap[pszsnap->basicinfo.industyCode].interestRate++;
				msz1Snap[pszsnap->basicinfo.industyCode].prevClosePx += pszsnap->prevClosePx;
				msz1Snap[pszsnap->basicinfo.industyCode].openPx += pszsnap->openPx;
				msz1Snap[pszsnap->basicinfo.industyCode].highPx += pszsnap->highPx;
				msz1Snap[pszsnap->basicinfo.industyCode].lowPx += pszsnap->lowPx;
				msz1Snap[pszsnap->basicinfo.industyCode].lastPx += pszsnap->lastPx;
				msz1Snap[pszsnap->basicinfo.industyCode].change += pszsnap->change;
				msz1Snap[pszsnap->basicinfo.industyCode].volume += pszsnap->volume;
				msz1Snap[pszsnap->basicinfo.industyCode].amount += pszsnap->amount;
				msz1Snap[pszsnap->basicinfo.industyCode].bid[0].price += pszsnap->bid[0].price;
				msz1Snap[pszsnap->basicinfo.industyCode].offer[0].price += pszsnap->offer[0].price;
				msz1Snap[pszsnap->basicinfo.industyCode].hand = pszsnap->hand;
				msz1Snap[pszsnap->basicinfo.industyCode].out = pszsnap->out;
				msz1Snap[pszsnap->basicinfo.industyCode].in = pszsnap->in;
			}
			if(strlen(pszsnap->basicinfo.industyCode) !=0 )
			{
				char *ptr = strtok(pszsnap->basicinfo.industyCode,",");
				while(ptr != NULL)
				{
					msz1Snap[ptr].interestRate++;
					msz1Snap[ptr].prevClosePx += pszsnap->prevClosePx;
					msz1Snap[ptr].openPx += pszsnap->openPx;
					msz1Snap[ptr].highPx += pszsnap->highPx;
					msz1Snap[ptr].lowPx += pszsnap->lowPx;
					msz1Snap[ptr].lastPx += pszsnap->lastPx;
					msz1Snap[ptr].change += pszsnap->change;
					msz1Snap[ptr].volume += pszsnap->volume;
					msz1Snap[ptr].amount += pszsnap->amount;
					msz1Snap[ptr].bid[0].price += pszsnap->bid[0].price;
					msz1Snap[ptr].offer[0].price += pszsnap->offer[0].price;
					msz1Snap[ptr].hand = pszsnap->hand;
					msz1Snap[ptr].out = pszsnap->out;
					msz1Snap[ptr].in = pszsnap->in;
					ptr = strtok(NULL,",");
				}
			}
		}
		else
			break;
	}

	i=0;
	while((pcnsnap = (CROSNAP*)cnmarket->getSnap(++i))!=NULL)
	{
		if(msh1Snap.find(pcnsnap->securityID) != msh1Snap.end())
		{
			mcnSnap[pcnsnap->securityID].interestRate += msh1Snap[pcnsnap->securityID].interestRate;
			mcnSnap[pcnsnap->securityID].prevClosePx += msh1Snap[pcnsnap->securityID].prevClosePx;
			mcnSnap[pcnsnap->securityID].openPx += msh1Snap[pcnsnap->securityID].openPx;
			mcnSnap[pcnsnap->securityID].highPx += msh1Snap[pcnsnap->securityID].highPx;
			mcnSnap[pcnsnap->securityID].lowPx += msh1Snap[pcnsnap->securityID].lowPx;
			mcnSnap[pcnsnap->securityID].lastPx += msh1Snap[pcnsnap->securityID].lastPx;
			mcnSnap[pcnsnap->securityID].change += msh1Snap[pcnsnap->securityID].change;
			mcnSnap[pcnsnap->securityID].volume += msh1Snap[pcnsnap->securityID].volume;
			mcnSnap[pcnsnap->securityID].amount += msh1Snap[pcnsnap->securityID].amount;
			mcnSnap[pcnsnap->securityID].bid[0].price += msh1Snap[pcnsnap->securityID].bid[0].price;
			mcnSnap[pcnsnap->securityID].offer[0].price += msh1Snap[pcnsnap->securityID].offer[0].price;
			mcnSnap[pcnsnap->securityID].hand = msh1Snap[pcnsnap->securityID].hand;
			mcnSnap[pcnsnap->securityID].out = msh1Snap[pcnsnap->securityID].out;
			mcnSnap[pcnsnap->securityID].in = msh1Snap[pcnsnap->securityID].in;
		}
		if(msz1Snap.find(pcnsnap->securityID) != msz1Snap.end())
		{
			mcnSnap[pcnsnap->securityID].interestRate += msz1Snap[pcnsnap->securityID].interestRate;
			mcnSnap[pcnsnap->securityID].prevClosePx += msz1Snap[pcnsnap->securityID].prevClosePx;
			mcnSnap[pcnsnap->securityID].openPx += msz1Snap[pcnsnap->securityID].openPx;
			mcnSnap[pcnsnap->securityID].highPx += msz1Snap[pcnsnap->securityID].highPx;
			mcnSnap[pcnsnap->securityID].lowPx += msz1Snap[pcnsnap->securityID].lowPx;
			mcnSnap[pcnsnap->securityID].lastPx += msz1Snap[pcnsnap->securityID].lastPx;
			mcnSnap[pcnsnap->securityID].change += msz1Snap[pcnsnap->securityID].change;
			mcnSnap[pcnsnap->securityID].volume += msz1Snap[pcnsnap->securityID].volume;
			mcnSnap[pcnsnap->securityID].amount += msz1Snap[pcnsnap->securityID].amount;
			mcnSnap[pcnsnap->securityID].bid[0].price += msz1Snap[pcnsnap->securityID].bid[0].price;
			mcnSnap[pcnsnap->securityID].offer[0].price += msz1Snap[pcnsnap->securityID].offer[0].price;
			mcnSnap[pcnsnap->securityID].hand = msz1Snap[pcnsnap->securityID].hand;
			mcnSnap[pcnsnap->securityID].out = msz1Snap[pcnsnap->securityID].out;
			mcnSnap[pcnsnap->securityID].in = msz1Snap[pcnsnap->securityID].in;
		}
		if(mcnSnap[pcnsnap->securityID].interestRate > 0)
		{
			INFO("%s - %d",pcnsnap->securityID,mcnSnap[pcnsnap->securityID].interestRate);
			pcnsnap->prevClosePx = mcnSnap[pcnsnap->securityID].prevClosePx/mcnSnap[pcnsnap->securityID].interestRate;
			pcnsnap->openPx = mcnSnap[pcnsnap->securityID].openPx/mcnSnap[pcnsnap->securityID].interestRate;
			pcnsnap->highPx = mcnSnap[pcnsnap->securityID].highPx/mcnSnap[pcnsnap->securityID].interestRate;
			pcnsnap->lowPx = mcnSnap[pcnsnap->securityID].lowPx/mcnSnap[pcnsnap->securityID].interestRate;
			pcnsnap->lastPx = mcnSnap[pcnsnap->securityID].lastPx/mcnSnap[pcnsnap->securityID].interestRate;
			pcnsnap->change = mcnSnap[pcnsnap->securityID].change/mcnSnap[pcnsnap->securityID].interestRate;
			pcnsnap->volume = mcnSnap[pcnsnap->securityID].volume/mcnSnap[pcnsnap->securityID].interestRate;
			pcnsnap->amount = mcnSnap[pcnsnap->securityID].amount/mcnSnap[pcnsnap->securityID].interestRate;
			pcnsnap->bid[0].price = mcnSnap[pcnsnap->securityID].bid[0].price/mcnSnap[pcnsnap->securityID].interestRate;
			pcnsnap->offer[0].price = mcnSnap[pcnsnap->securityID].offer[0].price/mcnSnap[pcnsnap->securityID].interestRate;
			pcnsnap->hand = mcnSnap[pcnsnap->securityID].hand/mcnSnap[pcnsnap->securityID].interestRate;
			pcnsnap->out = mcnSnap[pcnsnap->securityID].out/mcnSnap[pcnsnap->securityID].interestRate;
			pcnsnap->in = mcnSnap[pcnsnap->securityID].in/mcnSnap[pcnsnap->securityID].interestRate;
		}
	}
}
