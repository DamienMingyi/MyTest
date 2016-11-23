/**
 * @file    DayKReply.cpp
 * @brief   Ӧ���б�����
 * @note
 * ֧��day,week,month,year���Զ�������
 * @author  ygao@sse.com.cn
 * @version 1.0
 * @date    2014-2-28
 */
#include "DayKReply.h"

/** @brief   �������¶���*/
int g_Month[]={31,28,31,30,31,30,31,31,30,31,30,31};
/** @brief   �����¶���*/
int g_Month1[]={31,29,31,30,31,30,31,31,30,31,30,31};

DayKReply::DayKReply(CShmEnv* shmEnv,const char* prefix)
	:KLineReply(shmEnv,prefix)
{
	this->dftPeriod="day";
	this->type="day";
}

DayKReply::~DayKReply()
{
}

void DayKReply::getTodayKLine()
{
	if (si!=NULL)
	{
	    ISnapShot* ISnap=this->market->getISnap();
		ISnap->attach(si->snap_item);
		CANDLESTICK kline;
		ISnap->getDayCandlestick(&kline);
		if (ksrc.empty() || ksrc[ksrc.size()-1].dateTime<kline.dateTime)
			ksrc.push_back(kline);
		ISnap->getValue(SNAP_DATE,today);
	}
	else
	{
		if (!ksrc.empty())
			today=ksrc[ksrc.size()-1].dateTime;
	}
}

void DayKReply::getKLine()
{
	const int KTYPE_DAY=0;
	const int KTYPE_WEEK=-1;
	const int KTYPE_MONTH=-2;
	const int KTYPE_YEAR=-3;

	int ktype=0;
	if (this->period.compare("day")==0)
		ktype=KTYPE_DAY;
	else if (this->period.compare("week")==0)
		ktype=KTYPE_WEEK;
	else if (this->period.compare("month")==0)
		ktype=KTYPE_MONTH;
	else if (this->period.compare("year")==0)
		ktype=KTYPE_YEAR;
	else
	{
		ktype=atoi(this->period.c_str());
		if (ktype<KTYPE_YEAR || ktype>250)
			return;
	}
	if (ktype==KTYPE_DAY )
	{
		for (unsigned int i=0;i<ksrc.size();++i)
		{
			kline.push_back(ksrc[i]);
		}
		return;
	}
	if (ksrc.empty())
		return;
	CANDLESTICK kLine;
	long yy1,yy2,mm1,mm2,dd1,dd2;
	bool isPeriod=false;
	memcpy(&kLine,&ksrc[0],sizeof(CANDLESTICK));
	for (unsigned int i=1;i<ksrc.size();++i)
	{
		isPeriod=false;
		switch (ktype)
		{
		case KTYPE_WEEK:
			yy1=ksrc[i].dateTime/10000;yy2=kLine.dateTime/10000;
			mm1=ksrc[i].dateTime%10000/100;mm2=kLine.dateTime%10000/100;
			dd1=ksrc[i].dateTime%100;dd2=kLine.dateTime%100;
			if (yy1!=yy2)
			{//��ͬ��
				if (dd1!=2||dd2!=31) isPeriod=true;
			 }
			 else
			 {//ͬ��
					if (mm1!=mm2)
					{//��ͬ��
						if (mm2<1 || mm2>12)
							isPeriod=true;
						else
							{
								if (yy1%400==0 || (yy1%100!=0 && yy1%4==0))
								{
									if (dd1!=1 || dd2!=g_Month1[mm2-1]) isPeriod=true;
								}
								else
								{
									if (dd1!=1 || dd2!=g_Month[mm2-1]) isPeriod=true;
								}
							}
					}
					else
					{//ͬ��
						if (dd1-dd2>2) isPeriod=true;
					}
				}
				break;
		case KTYPE_MONTH:
			if (ksrc[i].dateTime%10000/100!=kLine.dateTime%10000/100) isPeriod=true;//��ͬ��
				break;
		case KTYPE_YEAR:
			if (ksrc[i].dateTime/10000!=kLine.dateTime/10000) isPeriod=true;//��ͬ��
				break;
		default://�û��Զ���K������
			if (i%ktype==(ktype-1))
				isPeriod=true;
			break;
		}
		if (isPeriod)
		{
			kline.push_back(kLine);
			memcpy(&kLine,&ksrc[i],sizeof(CANDLESTICK));
		}
		else
		{
			kLine.dateTime=ksrc[i].dateTime;//date
			if (ksrc[i].highPx>kLine.highPx)
				kLine.highPx=ksrc[i].highPx;//high
			if (ksrc[i].lowPx<kLine.lowPx)
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
