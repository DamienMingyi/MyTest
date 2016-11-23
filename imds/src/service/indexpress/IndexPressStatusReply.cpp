/*
 * IndexPressStatusReply.cpp
 *
 *  Created on: Nov 4, 2015
 *      Author: level2
 */

#include "IndexPressStatusReply.h"
#include "../../public/market/Market.h"
#include "../../public/indexpress/IndexPressSnap.h"

IndexPressStatusReply::IndexPressStatusReply() {
	// TODO Auto-generated constructor stub

}

IndexPressStatusReply::~IndexPressStatusReply() {
	// TODO Auto-generated destructor stub
}

int IndexPressStatusReply::request(const char* script,const char* query,char* buffer,int& len)
{
	if (!StatusReply::parse(script,query))
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	snap=market->getSnap(0);
	if (!snap)
		return HttpReply::replyError(HttpStatus::NotFound,buffer,len);
	INDEXSNAP* s=(INDEXSNAP*)snap;
	date=s->date;

	HttpReply::parseSelect("time,total");

	if (paramSelect.empty())
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	return StatusReply::reply(buffer,len);
}

int IndexPressStatusReply::getSnapString(char* buffer,const char* select)
{
	INDEXSNAP* s=(INDEXSNAP*)snap;
	if (strcmp(select, "time") == 0)
		return sprintf(buffer, "[%d]", s->lowPx);
	if (strcmp(select,"total")==0)
		return sprintf(buffer,"[%ld]", s->amount - 1);
	return 0;
}
