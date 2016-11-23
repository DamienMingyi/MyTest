/*
 * Stock2Trd2Reply.cpp
 *
 *  Created on: Apr 14, 2014
 *      Author: level2
 */

#include "Stock2Trd2Reply.h"
#include "../../public/stock2/StockSnap2.h"

Stock2Trd2Reply::Stock2Trd2Reply() {
	// TODO Auto-generated constructor stub

}

Stock2Trd2Reply::~Stock2Trd2Reply() {
	// TODO Auto-generated destructor stub
}

int Stock2Trd2Reply::request(const char* script,const char* query,char* buffer,int& len)
{
	if (!Trd2Reply::parse(script,query))
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);
	trd2.clear();
	SNAP_INDEX* si=(SNAP_INDEX*)(market)->getSnapIndex(code.c_str());
	if (!si)
		return HttpReply::replyError(HttpStatus::NotFound,buffer,len);

	STOCKSNAP2* snap=(STOCKSNAP2*)si->snap_item;
	time=snap->time;
	CShmVector* tradeVector=si->tradeVector;
	map<int,long>::iterator it;
	unsigned int i=0;
	for (i=0;i<tradeVector->size();++i)
	{
		STOCKTRADE2* trade=(STOCKTRADE2*)&tradeVector->getItem(i);
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
