/*
 * MonitorReply.cpp
 *
 *  Created on: Oct 27, 2015
 *      Author: level2
 */

#include "MonitorReply.h"

MonitorReply::MonitorReply() {
	// TODO Auto-generated constructor stub
	snap=NULL;
	type="monitor";
}

MonitorReply::~MonitorReply() {
	// TODO Auto-generated destructor stub
}

bool MonitorReply::parse(const char* script,const char* query)
{
	if (market==NULL)
		return false;

	HttpReply::parse(script,query);

	if (!HttpReply::parseCode())
		return false;

	return true;
}

int MonitorReply::reply(char* buffer,int& len)
{
	len=sprintf(buffer,"{\"date\":%d,\"time\":%d,\"monitor\":[", date,time);
	for (unsigned int i=0;i<paramSelect.size();++i)
	{
		if (i>0)
			len+=sprintf(buffer+len,",");
		int slen=getSnapString(buffer+len,paramSelect[i].c_str());
		if (slen==0)
			return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);
		len+=slen;
	}
	len+=sprintf(buffer+len,"]}");
	return HttpStatus::OK;
}
