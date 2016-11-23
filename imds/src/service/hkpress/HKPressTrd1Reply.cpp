/*
 * HKPressTrd1Reply.cpp
 *
 *  Created on: Sep 24, 2015
 *      Author: level2
 */

#include "HKPressTrd1Reply.h"
#include "../../public/market/Market.h"
#include "../../public/hkpress/HKPressSnap.h"

HKPressTrd1Reply::HKPressTrd1Reply() {
	// TODO Auto-generated constructor stub

}

HKPressTrd1Reply::~HKPressTrd1Reply() {
	// TODO Auto-generated destructor stub
}

int HKPressTrd1Reply::request(const char* script,const char* query,char* buffer,int& len)
{
	if (!Trd1Reply::parse(script,query))
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	SNAP_INDEX* si=market->getSnapIndex(code.c_str());
	if (!si)
		return HttpReply::replyError(HttpStatus::NotFound,buffer,len);

	trd1.clear();
	HKPSNAP* snap=(HKPSNAP*)si->snap_item;
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

int HKPressTrd1Reply::getTradeString(char* buffer,char* trd)
{
	HKPTRADE* t=(HKPTRADE*)trd;
	return sprintf(buffer,"[%d,%s,%ld,%d]",\
			t->time,getHKFloat(t->price),t->volume,t->sign);
}

