/*
 * StockTrd2Reply.cpp
 *
 *  Created on: Mar 17, 2014
 *      Author: level2
 */

#include "StockTrd2Reply.h"
#include "../../public/market/Market.h"
#include "../../public/stock/StockSnap.h"

StockTrd2Reply::StockTrd2Reply()
{

}

StockTrd2Reply::~StockTrd2Reply() {

}

int StockTrd2Reply::request(const char* script,const char* query,char* buffer,int& len)
{
	if (!Trd2Reply::parse(script,query))
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);
	trd2.clear();
	SNAP_INDEX* si=market->getSnapIndex(code.c_str());
	if (!si)
		return HttpReply::replyError(HttpStatus::NotFound,buffer,len);

	STOCKSNAP* snap=(STOCKSNAP*)si->snap_item;
	time=snap->time;
	CShmVector* tradeVector=si->tradeVector;
	map<int,long>::iterator it;
	unsigned int i=0;
	for (i=0;i<tradeVector->size();++i)
	{
		STOCKTRADE* trade=(STOCKTRADE*)&tradeVector->getItem(i);
		if (trade!=NULL)
		{
			it=trd2.find(trade->price);
			if (it!=trd2.end())
			{
				it->second+=trade->volume;
			}
			else
				trd2[trade->price]=trade->volume;
		}
	}
	return Trd2Reply::reply(buffer,len);
}


