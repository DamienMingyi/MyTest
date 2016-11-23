/*
 * HKPressStatusReply.cpp
 *
 *  Created on: Nov 4, 2015
 *      Author: level2
 */

#include "HKPressStatusReply.h"
#include "../../public/market/Market.h"
#include "../../public/hkpress/HKPressSnap.h"

HKPressStatusReply::HKPressStatusReply() {
	// TODO Auto-generated constructor stub

}

HKPressStatusReply::~HKPressStatusReply() {
	// TODO Auto-generated destructor stub
}

int HKPressStatusReply::request(const char* script,const char* query,char* buffer,int& len)
{
	if (!StatusReply::parse(script,query))
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	snap=market->getSnap(0);
	if (!snap)
		return HttpReply::replyError(HttpStatus::NotFound,buffer,len);
	HKPSNAP* s=(HKPSNAP*)snap;
	date=s->date;

	HttpReply::parseSelect("time,total,status,threshold_amount,pos_amt,amount_status");

	if (paramSelect.empty())
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	return StatusReply::reply(buffer,len);
}

int HKPressStatusReply::getSnapString(char* buffer,const char* select)
{
	HKPSNAP* s=(HKPSNAP*)snap;
	if (strcmp(select, "time") == 0)
		return sprintf(buffer, "[%ld,%d]", s->lastPx, s->roundLot);
	if (strcmp(select,"total")==0)
		return sprintf(buffer,"[%ld,%ld]", s->in, s->pin);
	if (strcmp(select, "status") == 0)
		return sprintf(buffer, "[\"%s\",\"%s\"]", s->secTradingStatus, s->secTradingStatus1);
	if (strcmp(select, "amount_status") == 0)
		return sprintf(buffer, "\"%s\"", s->secTradingStatus2);
	if (strcmp(select, "threshold_amount") == 0)
		return sprintf(buffer, "%ld", s->openPx);
	if (strcmp(select, "pos_amt") == 0)
		return sprintf(buffer, "%ld", s->highPx);
	return 0;
}
