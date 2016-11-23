/*
 * HKPressMonitorReply.cpp
 *
 *  Created on: Oct 27, 2015
 *      Author: level2
 */

#include "HKPressMonitorReply.h"
#include "../../public/market/Market.h"
#include "../../public/hkpress/HKPressSnap.h"

HKPressMonitorReply::HKPressMonitorReply() {
	// TODO Auto-generated constructor stub

}

HKPressMonitorReply::~HKPressMonitorReply() {
	// TODO Auto-generated destructor stub
}

int HKPressMonitorReply::request(const char* script,const char* query,char* buffer,int& len)
{
	if (!MonitorReply::parse(script,query))
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	snap=market->getSnap(0);
	if (!snap)
		return HttpReply::replyError(HttpStatus::NotFound,buffer,len);
	HKPSNAP* s=(HKPSNAP*)snap;
	date=s->date;
	time=s->time;

	HttpReply::parseSelect("mem_usage,objects_num,save_date,save_time,packet_dropout,seq,latest_seq,recv_time,snap_time,total");

	if (paramSelect.empty())
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	return MonitorReply::reply(buffer,len);
}

int HKPressMonitorReply::getSnapString(char* buffer,const char* select)
{
	HKPSNAP* s=(HKPSNAP*)snap;
	if (strcmp(select,"mem_usage")==0)
		return sprintf(buffer,"%ld",s->hand);
	if (strcmp(select,"objects_num")==0)
		return sprintf(buffer, "%ld", s->out);
	if (strcmp(select, "save_time") == 0)
		return sprintf(buffer, "%ld", s->offer.price);
	if (strcmp(select, "save_date") == 0)
		return sprintf(buffer, "%ld", s->offer.size);
	if (strcmp(select, "packet_dropout") == 0)
		return sprintf(buffer, "[%ld,%ld]", s->lowPx, s->interest);
	if (strcmp(select, "seq") == 0)
		return sprintf(buffer, "[%ld,%ld]", s->prevLineVolume, s->avgPx);
	if (strcmp(select, "latest_seq") == 0)
		return sprintf(buffer, "[%ld,%ld]", s->prevLineAmount, s->perValue);
	if (strcmp(select, "recv_time") == 0)
		return sprintf(buffer, "[%ld,%ld]", s->prevClosePx, s->nominalpx);
	if (strcmp(select, "snap_time") == 0)
		return sprintf(buffer, "[%ld,%d]", s->lastPx, s->roundLot);
	if (strcmp(select, "total") == 0)
		return sprintf(buffer, "[%ld,%ld]", s->in, s->pin);
	return 0;
}
