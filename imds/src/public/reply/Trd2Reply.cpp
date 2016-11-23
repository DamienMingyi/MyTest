/**
 * @file    Trd2Reply.cpp
 * @brief   Ӧ��ּ۱�
 * @note
 * �����URLΪ/v1/market/trd2/code���۸�Ӹߵ��ͣ����ذ��۸����ĳɽ�ͳ��
 * ����ÿ���г��ĳɽ������ݽṹ���ܲ�ͬ������û��ʵ�ּ۸�ͳ�ƹ��ܣ���Ҫ����ʵ��
 * @author  ygao@sse.com.cn
 * @version 1.0
 * @date    2014-2-25
 */

#include "Trd2Reply.h"

Trd2Reply::Trd2Reply()
{
	type="trd2";
}

Trd2Reply::~Trd2Reply()
{
}


bool Trd2Reply::parse(const char* script,const char* query)
{
	if (market==NULL)
		return false;

	HttpReply::parse(script,query);
	if (!HttpReply::parseCode())
		return false;
	return true;
}

int Trd2Reply::reply(char* buffer,int& len)
{
	len=sprintf(buffer,"{\"code\":\"%s\",\"time\":%d,\"trd2\":[",\
					code.c_str(),time);
	map<int,long>::iterator it;
	for (it=trd2.begin();it!=trd2.end();++it)
	{
		if (it==trd2.begin())
			len+=sprintf(buffer+len,"[%s,",getFloat(it->first));
		else
			len+=sprintf(buffer+len,",[%s,",getFloat(it->first));
		len+=sprintf(buffer+len,"%ld]",it->second);
	}
	len+=sprintf(buffer+len,"]}");
	return HttpStatus::OK;
}
