/*
 * FixIncomeStatusReply.cpp
 *
 *  Created on: Nov 4, 2015
 *      Author: level2
 */

#include "FixIncomeStatusReply.h"
#include "../../public/market/Market.h"
#include "../../public/fixincome/FixIncomeSnap.h"

FixIncomeStatusReply::FixIncomeStatusReply() {
	// TODO Auto-generated constructor stub

}

FixIncomeStatusReply::~FixIncomeStatusReply() {
	// TODO Auto-generated destructor stub
}

int FixIncomeStatusReply::request(const char* script,const char* query,char* buffer,int& len)
{
	if (!StatusReply::parse(script,query))
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	snap=market->getSnap(0);
	if (!snap)
		return HttpReply::replyError(HttpStatus::NotFound,buffer,len);
	SHFISNAP* s=(SHFISNAP*)snap;
	date=s->date;

	HttpReply::parseSelect("time,total");

	if (paramSelect.empty())
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	return StatusReply::reply(buffer,len);
}

int FixIncomeStatusReply::getSnapString(char* buffer,const char* select)
{
    SHFISNAP* s=(SHFISNAP*)snap;
	if (strcmp(select, "time") == 0)
		return sprintf(buffer, "[%d]", s->bid[2].netPx);
	if (strcmp(select,"total")==0)
		return sprintf(buffer,"[%ld]",s->amount - 1);
	return 0;
}
