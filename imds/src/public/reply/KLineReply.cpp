/**
 * @file    KLineReply.cpp
 * @brief   Ӧ��K������
 * @note
 * ���г���K�߽ṹ��ͬ
 * @author  ygao@sse.com.cn
 * @version 1.0
 * @date    2014-3-10
 */

#include "KLineReply.h"

KLineReply::KLineReply(CShmEnv* shmEnv,const char* prefix)
{
	this->shmEnv=shmEnv;
	this->kvec=NULL;
	this->si=NULL;
	this->today=0;
	this->prefix=prefix;
}

KLineReply::~KLineReply()
{
}

int KLineReply::request(const char* script,const char* query,char* buffer,int& len)
{
	if (shmEnv==NULL)
		return HttpReply::replyError(HttpStatus::InternalServerError,buffer,len);

	kvec=NULL;
	si=NULL;
	kline.clear();
	ksrc.clear();
	today=0;
	HttpReply::parse(script,query);

	if (!HttpReply::parseCode())
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);
	if (this->code.length()>80)
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);
	char kcode[128];
	sprintf(kcode,"%s/%s.%s",this->prefix.c_str(),code.c_str(),type.c_str());
	kvec=shmEnv->getShmVector(kcode);
	if (market!=NULL)
		si=market->getSnapIndex(code.c_str());
	if(kvec==NULL && si==NULL)
		return HttpReply::replyError(HttpStatus::NotFound,buffer,len);
	if (kvec!=NULL)
		for (unsigned int i=0;i<kvec->size();++i)
			ksrc.push_back(*((CANDLESTICK*)&kvec->getItem(i)));
	if (si!=NULL)
		getTodayKLine();
	if (!HttpReply::parsePeriod(this->dftPeriod.c_str()))
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	HttpReply::parseSelect("date,open,high,low,close,volume,amount");
	getKLine();
	total=kline.size();
	if (!HttpReply::parseBeginEnd(-101,-1))
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);
	return KLineReply::reply(buffer,len);
}


int KLineReply::reply(char* buffer,int& len)
{
	len=sprintf(buffer,"{\"code\":\"%s\",\"total\":%d,\"begin\":%d,\"end\":%d,\"kline\":[",\
								code.c_str(),total,begin,end);
	if (total>0)
	{

		if (this->prefix.compare("sho")==0 || this->prefix.compare("csip")==0)
			dot=4;
		else
			dot=3;
		for (unsigned int j=begin;j<kline.size()&&j<end;++j)
		{
			if (j>begin)
				len+=sprintf(buffer+len,",[");
			else
				len+=sprintf(buffer+len,"[");
			for (unsigned int i=0;i<paramSelect.size();++i)
			{
				if (i>0)
					len+=sprintf(buffer+len,",");
				int slen=this->getKLineString(buffer+len,paramSelect[i].c_str(),kline[j]);
				if (slen==0)
					return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);
				len+=slen;
			}
			len+=sprintf(buffer+len,"]");
		}
	}
	len+=sprintf(buffer+len,"]}");
	return HttpStatus::OK;
}

int  KLineReply::getKLineString(char* buffer,const char* select,CANDLESTICK& kl)
{
	if (strcmp(select,"date")==0)
		return sprintf(buffer,"%ld",kl.dateTime);
	if (strcmp(select,"open")==0)
		return sprintf(buffer,"%s",getFloat(kl.openPx));
	if (strcmp(select,"high")==0)
		return sprintf(buffer,"%s",getFloat(kl.highPx));
	if (strcmp(select,"low")==0)
		return sprintf(buffer,"%s",getFloat(kl.lowPx));
	if (strcmp(select,"close")==0)
		return sprintf(buffer,"%s",getFloat(kl.prevClosePx));
	if (strcmp(select,"volume")==0)
		return sprintf(buffer,"%ld",kl.volume);
	if (strcmp(select,"amount")==0)
		return sprintf(buffer,"%ld",kl.amount);
	if (strcmp(select,"avg")==0)
		return sprintf(buffer,"%s",getFloat(kl.avgPx));
	if (strcmp(select,"open_interest")==0)
		return sprintf(buffer,"%d",kl.openInterest);
	if (strcmp(select,"in_val")==0)
		return sprintf(buffer,"%s",getFloat(kl.inValue));
	if (strcmp(select,"time_val")==0)
		return sprintf(buffer,"%s",getFloat(kl.timeValue));
	return 0;
}
