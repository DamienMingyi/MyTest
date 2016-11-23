/*
 * FixIncomeTrd1Reply.cpp
 *
 *  Created on: Aug 31, 2015
 *      Author: level2
 */

#include "FixIncomeTrd1Reply.h"
#include "../../public/market/Market2.h"
#include "../../public/fixincome/FixIncomeSnap.h"


FixIncomeTrd1Reply::FixIncomeTrd1Reply() {
	// TODO Auto-generated constructor stub

}

FixIncomeTrd1Reply::~FixIncomeTrd1Reply() {
	// TODO Auto-generated destructor stub
}

int FixIncomeTrd1Reply::request(const char* script,const char* query,char* buffer,int& len)
{
	if (!Trd1Reply::parse(script,query))
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	SNAP_INDEX* si=(SNAP_INDEX*)(market)->getSnapIndex(code.c_str());
	if (!si)
		return HttpReply::replyError(HttpStatus::NotFound,buffer,len);

	trd1.clear();
	SHFITRADE* snap=(SHFITRADE*)si->snap_item;
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

int FixIncomeTrd1Reply::getTradeString(char* buffer,char* trd)
{
    SHFITRADE* t=(SHFITRADE*)trd;
	return sprintf(buffer,"[%d,%s,%ld,%d]",\
			t->time,getFloat(t->price),t->volume,t->sign);
}

