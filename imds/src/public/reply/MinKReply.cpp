/**
 * @file    MinKReply.cpp
 * @brief   Ӧ�����K��
 * @note
 * 
 * @author  ygao@sse.com.cn
 * @version 1.0
 * @date    2014-2-28
 */
#include "MinKReply.h"

MinKReply::MinKReply(CShmEnv* shmEnv,const char* prefix)
		:KLineReply(shmEnv,prefix)
{
	this->dftPeriod="5";
	type="min";
}

MinKReply::~MinKReply()
{
}

void MinKReply::getTodayKLine()
{
	if (si!=NULL && si->lineVector!=NULL)
	{
	    ISnapShot* ISnap=this->market->getISnap();
		ISnap->attach(si->snap_item);
		ISnap->getValue(SNAP_DATE,today);
		CANDLESTICK kline;
		for (unsigned int i=0;i<si->lineVector->size();++i)
		{
			if (i==0)
			{
				if (!ksrc.empty() && ksrc[ksrc.size()-1].dateTime/1000000>=today)
					break;
			}
			ISnap->getMinCandlestick(&si->lineVector->getItem(i),&kline);
			ksrc.push_back(kline);
		}
	}
	else
	{
		if (!ksrc.empty())
			today=ksrc[ksrc.size()-1].dateTime/1000000;
	}
}
/**
 * @brief �õ����з���K��
 * @return 
 * @note ������ڲ��������з���K�ߣ�ͨ��1�������ͳ�Ƶõ�
 */
void MinKReply::getKLine()
{
	if (ksrc.empty())
		return;
	int ktype=atoi(this->period.c_str());
	if (ktype==0)
		return;
	if (ktype>250)
		return getKLinebyDate(ktype);
	if (ktype<0)
		return getBefore(ktype,today);
	if (ktype==1)
	{
		for (unsigned int i=0;i<ksrc.size();++i)
		{
			kline.push_back(ksrc[i]);
		}
		return;
	}
	CANDLESTICK kLine;
	memcpy(&kLine,&ksrc[0],sizeof(CANDLESTICK));
	bool isPeriod=false;
	int precnt=1;
	for (unsigned int i=1;i<ksrc.size();++i)
	{
		isPeriod=false;
		if ((i-precnt>0) && (i-precnt)%ktype==0)
			isPeriod=true;
		if (kLine.dateTime/1000000 != ksrc[i].dateTime/1000000)
		{
			precnt=i+1;
			isPeriod=true;
		}
		if (isPeriod)
		{
			kline.push_back(kLine);
			memcpy(&kLine,&ksrc[i],sizeof(CANDLESTICK));
		}
		else
		{
			kLine.dateTime=ksrc[i].dateTime;//date
			if (kLine.openPx==0)
				kLine.openPx=ksrc[i].openPx;
			if (kLine.highPx==0 || ksrc[i].highPx>kLine.highPx)
				kLine.highPx=ksrc[i].highPx;//high
			if (kLine.lowPx==0 || ksrc[i].lowPx<kLine.lowPx)
				kLine.lowPx=ksrc[i].lowPx;//low
			kLine.prevClosePx=ksrc[i].prevClosePx;//close
			kLine.volume+=ksrc[i].volume;
			kLine.amount+=ksrc[i].amount;
			kLine.avgPx=ksrc[i].avgPx;
			kLine.openInterest=ksrc[i].openInterest;
			kLine.inValue=ksrc[i].inValue;
			kLine.timeValue=ksrc[i].timeValue;
		}
	}
	kline.push_back(kLine);
}

void MinKReply::getBefore(int before,int date)
{
	for (int i=ksrc.size()-1;i>=0;--i)
	{
		int dd=ksrc[i].dateTime/1000000;
		if (dd<date)
		{
			date=dd;
			++before;
		}
		if (before==0)
		{
			kline.insert(kline.begin(),ksrc[i]);
		}
		else if (before>0)
			return;
	}
}
void MinKReply::getKLinebyDate(int date)
{
	if (date<19000000 && date>21000000)
		return;
	for (unsigned int i=0;i<ksrc.size();++i)
	{
		int dd=ksrc[i].dateTime/1000000;
		if (dd==date)
			kline.push_back(ksrc[i]);
		if (dd>date)
			return;
	}
}
