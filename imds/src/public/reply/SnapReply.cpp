/**
 * @file    SnapReply.h
 * @brief   应答快照
 * @author  ygao@sse.com.cn
 * @note
 * 处理的URL为/v1/market/snap/code?select=*
 * @version 1.0
 * @date    2014-3-3
 */

#include "SnapReply.h"

SnapReply::SnapReply()
{
	snap=NULL;
	type="snap";
}

SnapReply::~SnapReply()
{
}

bool  SnapReply::parse(const char* script,const char* query)
{
	if (market==NULL)
		return false;

	HttpReply::parse(script,query);

	if (!HttpReply::parseCode())
		return false;

	return true;
}

int SnapReply::reply(char* buffer,int& len)
{
	len=sprintf(buffer,"{\"code\":\"%s\",\"date\":%d,\"time\":%d,\"snap\":[",\
						code.c_str(),date,time);
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
