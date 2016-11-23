/*
 * SzStockTrd2Reply.cpp
 *
 *  Created on: Jun 24, 2016
 *      Author: level2
 */

#include "SzStockTrd2Reply.h"
#include "../../public/szstock/SZSnap.h"

SzStockTrd2Reply::SzStockTrd2Reply() {
	// TODO Auto-generated constructor stub

}

SzStockTrd2Reply::~SzStockTrd2Reply() {
	// TODO Auto-generated destructor stub
}

int SzStockTrd2Reply::request(const char* script, const char* query, char* buffer, int& len)
{
	if (!Trd2Reply::parse(script, query))
		return HttpReply::replyError(HttpStatus::BadRequest, buffer, len);
	trd2.clear();

	SNAP_INDEX* si = market->getSnapIndex(code.c_str());
	if (!si)
		return HttpReply::replyError(HttpStatus::NotFound, buffer, len);

	SZSNAP *snap = (SZSNAP*)si->snap_item;
	time = snap->time;
	CShmVector* tradeVector = (CShmVector*)si->tradeVector;
	map<int, long>::iterator it;
	for (int i = 0; i < tradeVector->size(); ++ i)
	{
	    SZ1TRADE* trade = (SZ1TRADE*)&tradeVector->getItem(i);
		if (trade != NULL)
		{
			it = trd2.find(trade->price);
			if (it != trd2.end())
			{
				it->second += trade->volume;
			}
			else
			{
				trd2[trade->price] = trade->volume;
			}
		}
	}
	return Trd2Reply::reply(buffer, len);
}
