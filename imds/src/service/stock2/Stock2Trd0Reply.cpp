/*
 * Stock2TrdReply.cpp
 *
 *  Created on: Jun 13, 2014
 *      Author: level2
 */

#include "Stock2Trd0Reply.h"
#include "../../public/stock2/StockSnap2.h"

Stock2Trd0Reply::Stock2Trd0Reply() {
	// TODO Auto-generated constructor stub

}

Stock2Trd0Reply::~Stock2Trd0Reply() {
	// TODO Auto-generated destructor stub
}

int Stock2Trd0Reply::request(const char* script,const char* query,char* buffer,int& len)
{
	if (!Trd0Reply::parse(script,query))
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	SNAP_INDEX* si=(SNAP_INDEX*)(market)->getSnapIndex(code.c_str());
	if (!si)
		return HttpReply::replyError(HttpStatus::NotFound,buffer,len);

	trade0.clear();
	STOCKSNAP2* snap=(STOCKSNAP2*)si->snap_item;
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

int Stock2Trd0Reply::getTradeString(char* buffer,char* trd)
{
	STOCK2TRADE2* t=(STOCK2TRADE2*)trd;
	return sprintf(buffer,"[%d,%s,%ld, %ld, %ld, %ld, %d]",\
			t->time,getFloat(t->price),t->volume, t->money, t->buyNo, t->sellNo, t->sign);
}

