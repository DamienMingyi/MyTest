/*
 * FixIncomeTrd2Reply.cpp
 *
 *  Created on: Aug 31, 2015
 *      Author: level2
 */

#include "FixIncomeTrd2Reply.h"
#include "../../public/market/Market2.h"
#include "../../public/fixincome/FixIncomeSnap.h"

FixIncomeTrd2Reply::FixIncomeTrd2Reply() {
	// TODO Auto-generated constructor stub

}

FixIncomeTrd2Reply::~FixIncomeTrd2Reply() {
	// TODO Auto-generated destructor stub
}

int FixIncomeTrd2Reply::request(const char* script,const char* query,char* buffer,int& len)
{
	if (!Trd2Reply::parse(script,query))
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);
	trd2.clear();
	SNAP_INDEX* si=(SNAP_INDEX*)(market)->getSnapIndex(code.c_str());
	if (!si)
		return HttpReply::replyError(HttpStatus::NotFound,buffer,len);

	SHFISNAP* snap=(SHFISNAP*)si->snap_item;
	time=snap->time;
	CShmVector* tradeVector=si->tradeVector;
	map<int,long>::iterator it;
	unsigned int i=0;
	for (i=0;i<tradeVector->size();++i)
	{
	    SHFITRADE* trade=(SHFITRADE*)&tradeVector->getItem(i);
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
