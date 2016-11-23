/*
 * IndexPressMonitorReply.cpp
 *
 *  Created on: Oct 27, 2015
 *      Author: level2
 */

#include "IndexPressMonitorReply.h"
#include "../../public/market/Market.h"
#include "../../public/indexpress/IndexPressSnap.h"

IndexPressMonitorReply::IndexPressMonitorReply() {
	// TODO Auto-generated constructor stub

}

IndexPressMonitorReply::~IndexPressMonitorReply() {
	// TODO Auto-generated destructor stub
}

int IndexPressMonitorReply::request(const char* script,const char* query,char* buffer,int& len)
{
	if (!MonitorReply::parse(script,query))
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	snap=market->getSnap(0);
	if (!snap)
		return HttpReply::replyError(HttpStatus::NotFound,buffer,len);
	INDEXSNAP* s=(INDEXSNAP*)snap;
	date=s->date;
	time=s->time;

	HttpReply::parseSelect("mem_usage,objects_num,save_date,save_time,packet_dropout,seq,latest_seq,recv_time,snap_time,total,status");

	if (paramSelect.empty())
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	return MonitorReply::reply(buffer,len);
}

int IndexPressMonitorReply::getSnapString(char* buffer,const char* select)
{
	INDEXSNAP* s=(INDEXSNAP*)snap;
	if (strcmp(select,"mem_usage")==0)
		return sprintf(buffer,"%ld",s->hand);
	if (strcmp(select,"objects_num")==0)
		return sprintf(buffer, "%ld", s->prevAmount);
	if (strcmp(select, "save_date") == 0)
		return sprintf(buffer, "%ld", s->exchangeRate);
	if (strcmp(select, "save_time") == 0)
		return sprintf(buffer, "%ld", s->change);
	if (strcmp(select, "packet_dropout") == 0)
		return sprintf(buffer, "[%ld]", s->openPx);
	if (strcmp(select, "seq") == 0)
		return sprintf(buffer, "[%ld]", s->prevClosePx);
	if (strcmp(select, "latest_seq") == 0)
		return sprintf(buffer, "[%ld]", s->highPx);
	if (strcmp(select, "recv_time") == 0)
		return sprintf(buffer, "[%ld]", s->lastPx);
	if (strcmp(select, "snap_time") == 0)
		return sprintf(buffer, "[%ld]", s->lowPx);
	if (strcmp(select, "total") == 0)
		return sprintf(buffer,"[%ld]", s->amount - 1);
    if (strcmp(select, "status") == 0)
        return sprintf(buffer, "[\"%s\"]", "");
	return 0;
}
