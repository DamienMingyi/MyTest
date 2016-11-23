/*
 * OptionTrd1Reply.cpp
 *
 *  Created on: Mar 17, 2014
 *      Author: level2
 */

#include "OptionTrd1Reply.h"

#include "../../public/market/Market.h"
#include "../../public/option/OptionSnap.h"

OptionTrd1Reply::OptionTrd1Reply()
{
	dot=4;
}

OptionTrd1Reply::~OptionTrd1Reply()
{
}

int OptionTrd1Reply::request(const char* script,const char* query,char* buffer,int& len)
{
	if (!Trd1Reply::parse(script,query))
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	SNAP_INDEX* si=market->getSnapIndex(code.c_str());
	if (!si)
		return HttpReply::replyError(HttpStatus::NotFound,buffer,len);

	trd1.clear();
	SHOSNAP* snap=(SHOSNAP*)si->snap_item;
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

int OptionTrd1Reply::getTradeString(char* buffer,char* trd)
{
    SHOTRADE* t=(SHOTRADE*)trd;
	return sprintf(buffer,"[%d,%s,%ld,%d]",\
			t->time,getFloat(t->price),t->volume,t->sign);
}

