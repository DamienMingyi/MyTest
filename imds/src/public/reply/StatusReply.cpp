/*
 * StatusReply.cpp
 *
 *  Created on: Nov 4, 2015
 *      Author: level2
 */

#include "StatusReply.h"

StatusReply::StatusReply() {
	// TODO Auto-generated constructor stub
	snap=NULL;
	type="status";
}

StatusReply::~StatusReply() {
	// TODO Auto-generated destructor stub
}

bool StatusReply::parse(const char* script,const char* query)
{
	if (market==NULL)
		return false;

	HttpReply::parse(script,query);

	if (!HttpReply::parseCode())
		return false;

	return true;
}

int StatusReply::reply(char* buffer,int& len)
{
	len=sprintf(buffer,"{\"date\":%d,\"status\":[", date);
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
