/*
 * OptionStatusReply.cpp
 *
 *  Created on: Nov 4, 2015
 *      Author: level2
 */

#include "OptionStatusReply.h"
#include "../../public/market/Market.h"
#include "../../public/option/OptionSnap.h"

OptionStatusReply::OptionStatusReply() {
	// TODO Auto-generated constructor stub

}

OptionStatusReply::~OptionStatusReply() {
	// TODO Auto-generated destructor stub
}

int OptionStatusReply::request(const char* script,const char* query,char* buffer,int& len)
{
	if (!StatusReply::parse(script,query))
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	snap=market->getSnap(0);
	if (!snap)
		return HttpReply::replyError(HttpStatus::NotFound,buffer,len);
	SHOSNAP* s=(SHOSNAP*)snap;
	date=s->date;

	HttpReply::parseSelect("time,total,status");

	if (paramSelect.empty())
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	return StatusReply::reply(buffer,len);
}

int OptionStatusReply::getSnapString(char* buffer,const char* select)
{
    SHOSNAP* s=(SHOSNAP*)snap;
	if (strcmp(select, "time") == 0)
		return sprintf(buffer, "[%d]", s->bid[2].price);
	if (strcmp(select,"total")==0)
		return sprintf(buffer,"[%ld]", s->amount - 1);
	if (strcmp(select, "status") == 0)
		return sprintf(buffer, "[\"%s\"]", s->tradingPhaseCode);
	return 0;
}
