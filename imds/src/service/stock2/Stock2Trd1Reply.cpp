/*
 * Stock2Trd1Reply.cpp
 *
 *  Created on: Apr 14, 2014
 *      Author: level2
 */

#include "Stock2Trd1Reply.h"
#include "../../public/stock2/StockSnap2.h"

Stock2Trd1Reply::Stock2Trd1Reply() {
	// TODO Auto-generated constructor stub

}

Stock2Trd1Reply::~Stock2Trd1Reply() {
	// TODO Auto-generated destructor stub
}

int Stock2Trd1Reply::request(const char* script,const char* query,char* buffer,int& len)
{
	if (!Trd1Reply::parse(script,query))
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	SNAP_INDEX* si=(SNAP_INDEX*)(market)->getSnapIndex(code.c_str());
	if (!si)
		return HttpReply::replyError(HttpStatus::NotFound,buffer,len);

	trd1.clear();
	STOCKSNAP2* snap=(STOCKSNAP2*)si->snap_item;
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

int Stock2Trd1Reply::getTradeString(char* buffer,char* trd)
{
	STOCKTRADE2* t=(STOCKTRADE2*)trd;
	return sprintf(buffer,"[%d,%s,%ld,%d]",\
			t->time,getFloat(t->price),t->volume,t->sign);
}

