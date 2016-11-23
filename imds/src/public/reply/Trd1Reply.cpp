/**
 * @file    Trd1Reply.h
 * @brief   应答逐笔成交
 * @author  ygao@sse.com.cn
 * @note
 * 处理的URL为/v1/market/trd1/code?begin=-10&end=-1
 * @version 1.0
 * @date    2014-2-25
 */

#include "Trd1Reply.h"

Trd1Reply::Trd1Reply()
{
	type="trd1";
}

Trd1Reply::~Trd1Reply()
{
}

bool  Trd1Reply::parse(const char* script,const char* query)
{
	if (market==NULL)
		return false;

	HttpReply::parse(script,query);

	if (!HttpReply::parseCode())
		return false;

	return true;
}

int Trd1Reply::reply(char* buffer,int& len)
{
	len=sprintf(buffer,"{\"code\":\"%s\",\"time\":%d,\"total\":%d,\"begin\":%d,\"end\":%d,\"trd1\":[",\
				code.c_str(),time,total,begin,end);
	if (total>0)
	{
		for (unsigned int i=0;i<trd1.size();++i)
		{
			if (i>0)
				len+=sprintf(buffer+len,",");
			len+=getTradeString(buffer+len,trd1[i]);
		}
	}

	len+=sprintf(buffer+len,"]}");
	return HttpStatus::OK;
}
