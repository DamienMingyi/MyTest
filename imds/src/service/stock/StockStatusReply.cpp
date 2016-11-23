/*
 * StockStatusReply.cpp
 *
 *  Created on: Nov 4, 2015
 *      Author: level2
 */

#include "StockStatusReply.h"
#include "../../public/market/Market.h"
#include "../../public/stock/StockSnap.h"

StockStatusReply::StockStatusReply() {
	// TODO Auto-generated constructor stub

}

StockStatusReply::~StockStatusReply() {
	// TODO Auto-generated destructor stub
}

int StockStatusReply::request(const char* script,const char* query,char* buffer,int& len)
{
	if (!StatusReply::parse(script,query))
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	snap=market->getSnap(0);
	if (!snap)
		return HttpReply::replyError(HttpStatus::NotFound,buffer,len);
	STOCKSNAP* s=(STOCKSNAP*)snap;
	date=s->date;

	HttpReply::parseSelect("time,total,status");

	if (paramSelect.empty())
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	return StatusReply::reply(buffer,len);
}

int StockStatusReply::getSnapString(char* buffer,const char* select)
{
	STOCKSNAP* s=(STOCKSNAP*)snap;
	if (strcmp(select, "time") == 0)
		return sprintf(buffer, "[%ld]", s->bid[2].price);
	if (strcmp(select,"total")==0)
		return sprintf(buffer,"[%ld]", s->amount - 1);
	if (strcmp(select, "status") == 0)
		return sprintf(buffer, "[\"%s\"]", s->tradingPhaseCode);
	return 0;
}
