/*
 * FixIncomeMonitorReply.cpp
 *
 *  Created on: Oct 27, 2015
 *      Author: level2
 */

#include "FixIncomeMonitorReply.h"
#include "../../public/market/Market.h"
#include "../../public/fixincome/FixIncomeSnap.h"

FixIncomeMonitorReply::FixIncomeMonitorReply() {
	// TODO Auto-generated constructor stub

}

FixIncomeMonitorReply::~FixIncomeMonitorReply() {
	// TODO Auto-generated destructor stub
}

int FixIncomeMonitorReply::request(const char* script,const char* query,char* buffer,int& len)
{
	if (!MonitorReply::parse(script,query))
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	snap=market->getSnap(0);
	if (!snap)
		return HttpReply::replyError(HttpStatus::NotFound,buffer,len);
	SHFISNAP* s=(SHFISNAP*)snap;
	date=s->date;
	time=s->time;

	HttpReply::parseSelect("mem_usage,objects_num,save_date,save_time,packet_dropout,seq,latest_seq,recv_time,snap_time,total");

	if (paramSelect.empty())
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	return MonitorReply::reply(buffer,len);
}

int FixIncomeMonitorReply::getSnapString(char* buffer,const char* select)
{
    SHFISNAP* s=(SHFISNAP*)snap;
	if (strcmp(select,"mem_usage")==0)
		return sprintf(buffer,"%ld",s->hand);
	if (strcmp(select,"objects_num")==0)
		return sprintf(buffer, "%ld", s->out);
	if (strcmp(select, "save_time") == 0)
		return sprintf(buffer, "%d", s->offer[0].netPx);
	if (strcmp(select, "save_date") == 0)
		return sprintf(buffer, "%ld", s->offer[0].volume);
	if (strcmp(select, "packet_dropout") == 0)
		return sprintf(buffer, "[%d]", s->bid[0].netPx);
	if (strcmp(select, "seq") == 0)
		return sprintf(buffer, "[%ld]", s->bid[0].volume);
	if (strcmp(select, "latest_seq") == 0)
		return sprintf(buffer, "[%ld]", s->bid[1].volume);
	if (strcmp(select, "recv_time") == 0)
		return sprintf(buffer, "[%d]", s->bid[1].netPx);
	if (strcmp(select, "snap_time") == 0)
		return sprintf(buffer, "[%d]", s->bid[2].netPx);
	if (strcmp(select, "total") == 0)
		return sprintf(buffer,"[%ld]",s->amount - 1);
	return 0;
}
