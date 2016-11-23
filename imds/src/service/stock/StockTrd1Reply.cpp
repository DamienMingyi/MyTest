/*
 * StockTrd1Reply.cpp
 *
 *  Created on: Mar 17, 2014
 *      Author: level2
 */

#include "StockTrd1Reply.h"
#include "../../public/market/Market.h"
#include "../../public/stock/StockSnap.h"

StockTrd1Reply::StockTrd1Reply()
{
}

StockTrd1Reply::~StockTrd1Reply()
{
}

int StockTrd1Reply::request(const char* script,const char* query,char* buffer,int& len)
{
	if (!Trd1Reply::parse(script,query))
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	SNAP_INDEX* si=market->getSnapIndex(code.c_str());
	if (!si)
		return HttpReply::replyError(HttpStatus::NotFound,buffer,len);

	trd1.clear();
	STOCKSNAP* snap=(STOCKSNAP*)si->snap_item;
	time=snap->time;
	CShmVector* tradeVector=si->tradeVector;
	total=si->tradeVector->size();
	if (!Trd1Reply::parseBeginEnd(-11,-1))
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	if (total>0)
	{
		for (int i=begin;i<end;++i)
		{
			char* trd=&tradeVector->getItem(i);
			if (trd!=NULL)
			{
				trd1.push_back(trd);
			}
			else
			{
				total=0;
				begin=0;
				end=0;
			}
		}
	}
	return Trd1Reply::reply(buffer,len);
}

int StockTrd1Reply::getTradeString(char* buffer,char* trd)
{
	STOCKTRADE* t=(STOCKTRADE*)trd;
	return sprintf(buffer,"[%d,%s,%ld,%d]",\
			t->time,getFloat(t->price),t->volume,t->sign);
}

