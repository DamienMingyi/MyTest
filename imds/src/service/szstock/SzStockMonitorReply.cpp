/*
 * SzStockMonitorReply.cpp
 *
 *  Created on: Jun 24, 2016
 *      Author: level2
 */

#include "SzStockMonitorReply.h"
#include "../../public/szstock/SZSnap.h"
#include "../../public/market/Market.h"


SzStockMonitorReply::SzStockMonitorReply() {
	// TODO Auto-generated constructor stub

}

SzStockMonitorReply::~SzStockMonitorReply() {
	// TODO Auto-generated destructor stub
}

int SzStockMonitorReply::request(const char* script, const char* query, char* buffer, int& len)
{
	if (!MonitorReply::parse(script, query))
		return HttpReply::replyError(HttpStatus::BadRequest, buffer, len);

	snap = market->getSnap(0);
	if (!snap)
		return HttpReply::replyError(HttpStatus::NotFound, buffer, len);

	SZSNAP* s = (SZSNAP*)snap;
	date = s->date;
	time = s->time;

	HttpReply::parseSelect("mem_usage,objects_num,save_time,save_date,recv_time,snap_time,total,status");

	if (paramSelect.empty())
		return HttpReply::replyError(HttpStatus::BadRequest, buffer, len);

	return MonitorReply::reply(buffer, len);

}

int SzStockMonitorReply::getSnapString(char* buffer, const char* select)
{
	SZSNAP* s = (SZSNAP*)snap;
    if (strcmp(select, "mem_usage") == 0)
        return sprintf(buffer, "%ld", s->hand);
    if (strcmp(select, "objects_num") == 0)
        return sprintf(buffer, "%ld", s->out);
    if (strcmp(select, "save_time") == 0)
        return sprintf(buffer, "%ld", s->offer[0].price);
    if (strcmp(select, "save_date") == 0)
        return sprintf(buffer, "%ld", s->offer[0].size);
    if (strcmp(select, "recv_time") == 0)
        return sprintf(buffer, "[%ld]", s->bid[1].price);
    if (strcmp(select, "snap_time") == 0)
        return sprintf(buffer, "[%ld]", s->bid[2].price);
    if (strcmp(select, "total") == 0)
        return sprintf(buffer, "[%ld]", s->amount - 1);

    if (strcmp(select, "packet_dropout") == 0)
         return sprintf(buffer, "[%ld]", s->bid[0].price / 1000);
     if (strcmp(select, "seq") == 0)
         return sprintf(buffer, "[%ld]", s->bid[0].size);
     if (strcmp(select, "latest_seq") == 0)
         return sprintf(buffer, "[%ld]", s->bid[1].size);
     if (strcmp(select, "status") == 0)
             return sprintf(buffer, "[\"%s\"]", s->tradingPhaseCode);
	return 0;
}
