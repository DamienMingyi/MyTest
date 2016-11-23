/*
 * FixIncomeTrd0Reply.cpp
 *
 *  Created on: Aug 31, 2015
 *      Author: level2
 */

#include "FixIncomeTrd0Reply.h"
#include "../../public/market/Market2.h"
#include "../../public/fixincome/FixIncomeSnap.h"

FixIncomeTrd0Reply::FixIncomeTrd0Reply() {
	// TODO Auto-generated constructor stub

}

FixIncomeTrd0Reply::~FixIncomeTrd0Reply() {
	// TODO Auto-generated destructor stub
}

int FixIncomeTrd0Reply::request(const char* script,const char* query,char* buffer,int& len)
{
	if (!Trd0Reply::parse(script,query))
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	SNAP_INDEX* si=(SNAP_INDEX*)(market)->getSnapIndex(code.c_str());
	if (!si)
		return HttpReply::replyError(HttpStatus::NotFound,buffer,len);

	trade0.clear();
	SHFISNAP* snap=(SHFISNAP*)si->snap_item;
	time=snap->time;
	CShmVector* trade2Vector=si->trade2Vector;
	total=si->trade2Vector->size();
	if (!Trd0Reply::parseBeginEnd(-11,-1))
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	if (total>0)
	{
		for (int i=begin;i<end;++i)
		{
			char* trd=&trade2Vector->getItem(i);
			if (trd!=NULL)
			{
				trade0.push_back(trd);
			}
			else
			{
				total=0;
				begin=0;
				end=0;
			}
		}
	}
	return Trd0Reply::reply(buffer,len);
}

int FixIncomeTrd0Reply::getTradeString(char* buffer,char* trd)
{
    TICK* t=(TICK*)trd;
	char tradeNetPrice[36];
	memset(tradeNetPrice, 0, 36);
	memset(ftValue, 0, 36);
	strncpy(tradeNetPrice, getFloat(t->tradeNetPrice), 36);
	char accurateInterestAmt[36];
	memset(accurateInterestAmt, 0, 36);
	memset(ftValue, 0, 36);
	strncpy(accurateInterestAmt, getFloat(t->accurateInterestAmt, 4), 36);
	char tradeFullPrice[36];
	memset(tradeFullPrice, 0, 36);
	memset(ftValue, 0, 36);
	strncpy(tradeFullPrice, getFloat(t->tradeFullPrice), 36);
	char yield2Maturity[36];
	memset(yield2Maturity, 0, 36);
	memset(ftValue, 0, 36);
	strncpy(yield2Maturity, getFloat(t->yield2Marturity, 4), 36);
	return sprintf(buffer,"[%d,%s,%s,%s,%s,%ld,%ld,%s]",\
			t->time, tradeNetPrice, accurateInterestAmt, tradeFullPrice, yield2Maturity,
			t->tradeQty, t->tradeMoney, t->tradeMethod);
}

