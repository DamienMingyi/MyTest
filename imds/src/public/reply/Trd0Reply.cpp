/*
 * TrdReply.cpp
 *
 *  Created on: Jun 13, 2014
 *      Author: level2
 */

#include "Trd0Reply.h"

Trd0Reply::Trd0Reply() {
	// TODO Auto-generated constructor stub
	type="trd0";
}

Trd0Reply::~Trd0Reply() {
	// TODO Auto-generated destructor stub
}

bool Trd0Reply::parse(const char* script,const char* query)
{
	if (market==NULL)
		return false;

	HttpReply::parse(script,query);

	if (!HttpReply::parseCode())
		return false;

	return true;
}

int Trd0Reply::reply(char* buffer,int& len)
{
	len=sprintf(buffer,"{\"code\":\"%s\",\"time\":%d,\"total\":%d,\"begin\":%d,\"end\":%d,\"trd0\":[",\
				code.c_str(),time,total,begin,end);
	if (total>0)
	{
		for (unsigned int i=0;i<trade0.size();++i)
		{
			if (i>0)
				len+=sprintf(buffer+len,",");
			len+=getTradeString(buffer+len,trade0[i]);
		}
	}

	len+=sprintf(buffer+len,"]}");
	return HttpStatus::OK;
}
