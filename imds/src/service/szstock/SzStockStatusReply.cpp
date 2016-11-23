/*
 * SzStockStatusReply.cpp
 *
 *  Created on: Jun 24, 2016
 *      Author: level2
 */

#include "SzStockStatusReply.h"
#include "../../public/szstock/SZSnap.h"
#include "../../public/market/Market.h"

SzStockStatusReply::SzStockStatusReply() {
	// TODO Auto-generated constructor stub

}

SzStockStatusReply::~SzStockStatusReply() {
	// TODO Auto-generated destructor stub
}

int SzStockStatusReply::request(const char* script, const char* query, char* buffer, int& len)
{
	if (!StatusReply::parse(script, query))
		return HttpReply::replyError(HttpStatus::BadRequest, buffer, len);

	snap = market->getSnap(0);

	if (!snap)
		return HttpReply::replyError(HttpStatus::NotFound, buffer, len);

	SZSNAP* s = (SZSNAP*)snap;
	date = s->date;

	HttpReply::parseSelect("time,total,status");


	if (paramSelect.empty())
		return HttpReply::replyError(HttpStatus::BadRequest, buffer, len);
	return StatusReply::reply(buffer, len);
}

int SzStockStatusReply::getSnapString(char* buffer, const char* select)
{
	SZSNAP* s = (SZSNAP*)snap;
	if (strcmp(select, "time") == 0)
		return sprintf(buffer, "[%ld]", s->time);
	if (strcmp(select, "total") == 0)
		return sprintf(buffer, "[%ld]", s->amount - 1);
	if (strcmp(select, "status") == 0)
		return sprintf(buffer, "[\"%s\"]", s->tradingPhaseCode);

	return 0;
}
