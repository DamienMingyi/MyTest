/*
 * SzStockTrd1Reply.cpp
 *
 *  Created on: Jun 24, 2016
 *      Author: level2
 */

#include "SzStockTrd1Reply.h"
#include "../../public/market/Market.h"
#include "../../public/szstock/SZSnap.h"

SzStockTrd1Reply::SzStockTrd1Reply() {
	// TODO Auto-generated constructor stub

}

SzStockTrd1Reply::~SzStockTrd1Reply() {
	// TODO Auto-generated destructor stub
}

int SzStockTrd1Reply::request(const char* script, const char* query, char* buffer, int& len)
{
	if (!Trd1Reply::parse(script, query))
		return HttpReply::replyError(HttpStatus::BadRequest, buffer, len);

	SNAP_INDEX* si = (SNAP_INDEX*)market->getSnapIndex(code.c_str());
	if (!si)
		return HttpReply::replyError(HttpStatus::NotFound, buffer, len);

	trd1.clear();
	SZSNAP* snap = (SZSNAP*)si->snap_item;
	time = snap->time;

	CShmVector* tradeVector = si->tradeVector;
	total = tradeVector->size();
	if (!Trd1Reply::parseBeginEnd(-11, -1))
		return HttpReply::replyError(HttpStatus::BadRequest, buffer, len);
	if (total > 0)
	{
		for (int i = begin; i < end; ++ i)
		{
			char* trd = &tradeVector->getItem(i);
			if (trd != NULL)
			{
				trd1.push_back(trd);
			}
			else
			{
				begin = 0;
				end = 0;
				total = 0;
			}
		}
	}
	return Trd1Reply::reply(buffer, len);
}

int SzStockTrd1Reply::getTradeString(char* buffer, char* trd)
{
    SZ1TRADE * t = (SZ1TRADE*)trd;
	return sprintf(buffer, "[%d, %s, %ld, %d]", t->time, getFloat(t->price), t->volume, t->sign);
}
