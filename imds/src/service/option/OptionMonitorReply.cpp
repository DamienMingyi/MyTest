/*
 * OptionMonitorReply.cpp
 *
 *  Created on: Oct 27, 2015
 *      Author: level2
 */

#include "OptionMonitorReply.h"
#include "../../public/market/Market.h"
#include "../../public/option/OptionSnap.h"

OptionMonitorReply::OptionMonitorReply() {
	// TODO Auto-generated constructor stub

}

OptionMonitorReply::~OptionMonitorReply() {
	// TODO Auto-generated destructor stub
}

int OptionMonitorReply::request(const char* script,const char* query,char* buffer,int& len)
{
	if (!MonitorReply::parse(script,query))
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	snap=market->getSnap(0);
	if (!snap)
		return HttpReply::replyError(HttpStatus::NotFound,buffer,len);
	SHOSNAP* s=(SHOSNAP*)snap;
	date=s->date;
	time=s->time;

	HttpReply::parseSelect("mem_usage,objects_num,save_date,save_time,packet_dropout,seq,latest_seq,recv_time,snap_time,total,status");

	if (paramSelect.empty())
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	return MonitorReply::reply(buffer,len);
}

int OptionMonitorReply::getSnapString(char* buffer,const char* select)
{
    SHOSNAP* s=(SHOSNAP*)snap;
	if (strcmp(select,"mem_usage")==0)
		return sprintf(buffer,"%ld",s->hand);
	if (strcmp(select,"objects_num")==0)
		return sprintf(buffer, "%ld", s->out);
	if (strcmp(select, "save_time") == 0)
		return sprintf(buffer, "%d", s->offer[4].price);
	if (strcmp(select, "save_date") == 0)
		return sprintf(buffer, "%ld", s->offer[4].size);
	if (strcmp(select, "packet_dropout") == 0)
		return sprintf(buffer, "[%ld,%ld]", s->bid[0].price, s->offer[0].price);
	if (strcmp(select, "seq") == 0)
		return sprintf(buffer, "[%ld,%ld]", s->bid[0].size, s->offer[0].size);
	if (strcmp(select, "latest_seq") == 0)
		return sprintf(buffer, "[%ld,%ld]", s->bid[1].size, s->offer[1].size);
	if (strcmp(select, "recv_time") == 0)
		return sprintf(buffer, "[%ld,%ld]", s->bid[1].price, s->offer[1].price);
	if (strcmp(select, "snap_time") == 0)
		return sprintf(buffer, "[%ld,%ld]", s->bid[2].price, s->offer[2].price);
	if (strcmp(select, "total") == 0)
		return sprintf(buffer,"[%ld]", s->amount - 1);
	if (strcmp(select, "status") == 0)
        return sprintf(buffer, "[\"%s\"]", s->tradingPhaseCode);
	return 0;
}
