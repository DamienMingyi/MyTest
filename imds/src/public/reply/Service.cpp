/**
 * @file    Service.cpp
 * @brief   �������
 * @note
 * ���ģʽ��֧�ֶ��г��������������
 * @author  ygao@sse.com.cn
 * @version 1.0
 * @date    2014-2-28
 */
#include <stdio.h>
#include "Service.h"

Service::Service()
{
	buflen=8*1024*1024;
	buffer=new char[buflen];
	bf = new char[buflen];
}

Service::~Service()
{
	delete[] buffer;
	delete[] bf;
}

void Service::addReply(Market* market,HttpReply* reply)
{
	reply->attach(market);
	replys[reply->getURL()]=reply;
	unsigned int i=0;
	for (i=0;i<markets.size();++i)
	{
		if (markets[i]==market)
			break;
	}
	if (i==markets.size())
		markets.push_back(market);
}
void Service::refresh()
{
	for (unsigned int i=0;i<markets.size();++i)
		markets[i]->refresh();
}
IHttpReply* Service::getReply(const char* script)
{
	map<string,HttpReply*>::iterator it;
	for (it=replys.begin();it!=replys.end();++it)
	{
		if (strstr(script,it->first.c_str())!=NULL)
			return it->second;
	}
	return NULL;
}

int Service::request(const char* script,const char* query)
{
	int len=buflen;
	IHttpReply* httpReply=getReply(script);
	if (httpReply!=NULL)
	{
		return httpReply->request(script,query,buffer,len);
	}
	len=sprintf(buffer,"BadRequest");
	return HttpStatus::BadRequest;
}

void Service::show()
{
	printf("%s\r\n",buffer);
}
