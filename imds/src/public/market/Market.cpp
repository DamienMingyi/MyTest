/**
 * @file    Market.cpp
 * @brief   ����г�����
 * @author  ygao@sse.com.cn
 * @author  jiachen@sse.com.cn
 * @version 1.0
 * @date    2014-2-20
 * @bug     ��
 * @warning ��
 */
#include <stdio.h>
#include <string.h>
#include "Market.h"
#include "../program/Logger.h"

Market::Market(const char* name,CShmEnv* shmEnv,ISnapShot* snap): shmEnv(shmEnv), snapVector(NULL), snap(snap), tmSnap(NULL),
date(0), init_time(0), line_size(0), trade_size(0), trade2_size(0) {
	strncpy(this->name,name,sizeof(this->name)-1);
}

Market::~Market() {

}

const char* Market::getName()
{
	return this->name;
}

bool Market::load(int snap_size,int line_size,int trade_size, int trade2_size)
{
	char cont_name[32];
	sprintf(cont_name,"%s/snap",name);
	snapVector=shmEnv->getShmVector(cont_name,snap_size,100);
	if (snapVector==NULL) return false;

	tmSnap=getItem(snapVector,0);

	this->line_size=line_size;
	this->trade_size=trade_size;
	this->trade2_size = trade2_size;
	refresh();
	return true;
}

void Market::refresh()
{
	int dd=0,tt=0;
	snap->attach(tmSnap);
	snap->getValue(SNAP_DATE,dd);
	snap->getValue(SNAP_INIT_TIME,tt);
	if (date!=dd || init_time!=tt)
	{
		INFO("refresh %s from %d %d to %d %d",name,date,init_time,dd,tt);
		date=dd;
		init_time=tt;
		finder.clear();
		int total=0;
		char code[80];
		snap->getValue(SNAP_TOTAL,total);
		for (int i=1;i<total;++i)
		{
			char* item=getItem(snapVector,i);
			snap->attach(item);
			if (snap->getValue(SNAP_SECURITY_ID,code))
			{
				SNAP_INDEX si;
				si.snap_item=item;
				char cont_name[32];
				sprintf(cont_name,"%s/line/%d",name,i);
				si.lineVector=shmEnv->getShmVector(cont_name,line_size,100);
				sprintf(cont_name,"%s/trade/%d",name,i);
				si.tradeVector=shmEnv->getShmVector(cont_name,trade_size,100);
				if (this->trade2_size != 0)
				{
					sprintf(cont_name,"%s/trade2/%d",name,i);
					si.trade2Vector=shmEnv->getShmVector(cont_name,trade2_size,100);
				}
				finder[code]=si;
			}
		}
	}
}

char* Market::getSnap(const char* code)
{
	map<string,SNAP_INDEX>::iterator it;
	it=finder.find(code);
	if (it!=finder.end())
		return it->second.snap_item;
	return NULL;
}


SNAP_INDEX* Market::getSnapIndex(const char* code)
{
	map<string,SNAP_INDEX>::iterator it;
	it=finder.find(code);
	if (it!=finder.end())
	{
		memcpy(&snap_idx,&it->second,sizeof(SNAP_INDEX));
		return &snap_idx;
	}
	return NULL;

}

void Market::initSnap(int pos,char* item)
{
	char* s=getItem(snapVector,pos);
	int len=snapVector->getItemLength();
	memcpy(s,item,len);

	char code[80];
	snap->attach(item);
	if (snap->getValue(SNAP_SECURITY_ID,code))
	{
		SNAP_INDEX si;
		si.snap_item=s;
		char cont_name[32];
		sprintf(cont_name,"%s/line/%d",name,pos);
		si.lineVector=shmEnv->getShmVector(cont_name,line_size,100);
		if (si.lineVector)
		{
			if(!si.lineVector->empty())
				si.lineVector->clear();
		}
		else
			ERROR("%s shmvector is null",cont_name);

		sprintf(cont_name,"%s/trade/%d",name,pos);
		si.tradeVector=shmEnv->getShmVector(cont_name,trade_size,100);
		if (si.tradeVector)
		{
			if(!si.tradeVector->empty())
				si.tradeVector->clear();
		}
		else
			ERROR("%s shmvector is null",cont_name);

		if (this->trade2_size != 0)
		{
			sprintf(cont_name,"%s/trade2/%d",name,pos);
			si.trade2Vector=shmEnv->getShmVector(cont_name,trade2_size,100);
			if (si.trade2Vector)
			{
				if(!si.trade2Vector->empty())
					si.trade2Vector->clear();
			}
			else
				ERROR("%s shmvector is null",cont_name);
		}

		if (si.lineVector!=NULL && si.tradeVector!=NULL && (this->trade2_size == 0 || si.trade2Vector != NULL))
			finder[code]=si;
	}
}

void Market::update(char* snap_item)
{
	snap->attach(snap_item);
	map<string,SNAP_INDEX>::iterator it;
	char code[80];
	if (!snap->getValue(SNAP_SECURITY_ID,code))
		return;
	it=finder.find(code);
	if (it==finder.end())
		return;
	SNAP_INDEX si=it->second;
	char* item=snap->getLine();
	if (item!=NULL)
	{
		int line_time=-1;
		if (snap->getValue(SNAP_LINE_TIME,line_time) && line_time>=0)
		{
			//append the missing line
			if (si.lineVector->size()<line_time)
			{
				char* ti;
				unsigned long sz = si.lineVector->size();
				if (si.lineVector->size()==0)
					ti=item;
				else
					ti=&si.lineVector->getItem(si.lineVector->size()-1);
				for (int i=si.lineVector->size();line_time-si.lineVector->size();++i)
				{
					//@bug 2014/9/1
					if (!si.lineVector->push_back(snap->append(i, ti, sz)))
					{
						WARN("line vector push back failed!");
						return ;
					}
				}
			}
			//insert the new line
			if (si.lineVector->size()==line_time)
			{
				if (!si.lineVector->push_back(item))
				{
					WARN("line vector push back failed!");
					return ;
				}
			}
			//update the line
			else if (si.lineVector->size()==line_time+1)
			{
				char* ti=&si.lineVector->getItem(line_time);
				memcpy(ti,item,this->line_size);
			}
		}
	}
	item=snap->getTrade();
	if (item!=NULL)
	{
		if (!si.tradeVector->push_back(item))
		{
			WARN("trade vector push back failed!");
			return ;
		}
	}
}

void Market::updateTick(char* snap_item)
{
	if (this->trade2_size != 0)
	{
		snap->attach(snap_item);
		map<string,SNAP_INDEX>::iterator it;
		char code[80];
		if (!snap->getValue(SNAP_SECURITY_ID,code))
			return;
		it=finder.find(code);
		if (it==finder.end())
			return;
		SNAP_INDEX si=it->second;
		char* item = snap->getTrade2();
		if(item != NULL)
		{
			if (!si.trade2Vector->push_back(item))
			{
				WARN("tick vector push back failed!");
				return ;
			}
		}
	}
}

char* Market::getItem(CShmVector* shmVector,int pos)
{
	if (shmVector->size()<=pos)
	{
		shm_size_t len=shmVector->getItemLength();
		char* tmp=new char[len];
		memset(tmp,0,len);
		for (shm_size_t i=shmVector->size();i<=pos;++i)
		{
			if (!shmVector->push_back(tmp))
			{
				WARN("shm vector push back failed!");
				return NULL;
			}
		}
		delete[] tmp;
	}
	if (shmVector->size()>pos)
	{
		return &shmVector->getItem(pos);
	}
	return NULL;
}


int Market::getSnapCount()
{
	int count=0;
	snap->attach(tmSnap);
	this->snap->getValue(SNAP_TOTAL,count);
	return count;
}

char* Market::getSnap(int pos)
{
	if (snapVector->size()>pos)
		return &snapVector->getItem(pos);
	return NULL;
}

ISnapShot* Market::getISnap()
{
	return this->snap;
}


Market* createMarket(const char* name,const char* shm_path,ISnapShot* ISnap)
{
	CShmEnv* shmEnv=new CShmEnv();
	if (shmEnv->openShmEnv(shm_path)!=SUCCEED)
	{
		ERROR("openShmEnv(%s) error",shm_path);
		return NULL;
	}
	int snap_size=0,line_size=0,trade_size=0;
	int trade2_size = 0;
	ISnap->getValue(SNAP_SIZE,snap_size);
	ISnap->getValue(SNAPLINE_SIZE,line_size);
	ISnap->getValue(SNAPTRADE_SIZE,trade_size);
	ISnap->getValue(SNAPTRADE2_SIZE, trade2_size);

	Market* market=new Market(name,shmEnv,ISnap);
	if (!market->load(snap_size,line_size,trade_size, trade2_size))
	{
		ERROR("%s market load error",name);
		return NULL;
	}
	return market;
}
int getLineTime(int time)
{
	//before 9:25
	if(time <= 92500)
		return -1;
	else if (time>92500 && time<93000)
		return 0;
	else if(time >=93000 && time <=113000)
	{
		return (time/10000-9)*60+time%10000/100-30+(time%100==0?0:1);
	}
	//between 11:29 and 13:00
	else if(time >= 113000 && time <130000)
	{
		return 120;
	}
	else if (time==130000)
		return 121;
	//between 13:00 and 14:59
	else if(time >130000 && time <150000)
	{
		return 121+(time/10000-13)*60+time%10000/100+(time%100==0?-1:0);
	}
	//after 15:00
	else if(time >= 150000)
	{
		return 240;
	}
	return -1;
}


int getDatetime(int line_time)
{
	if(line_time >= 0 && line_time <= 29)
		return (line_time + 930) * 100;
	else if(line_time >= 30 && line_time <= 89)
		return (line_time - 30 + 1000) * 100;
	else if(line_time >=90 && line_time <= 120)
		return (line_time - 90 + 1100) * 100;
	else if(line_time >= 121 && line_time <= 179)
		return (line_time - 120 + 1300) * 100;
	else if(line_time >= 180 && line_time <= 239)
		return (line_time - 180 + 1400) * 100;
	else if(line_time >= 240)
		return 150000;
	else
		return -1;
}

int getHKDateTime(int lineTime)
{
	if (lineTime >= 0 && lineTime <=29)
		return (lineTime + 930) * 100;
	else if(lineTime >= 30 && lineTime <= 89)
		return (lineTime - 30 + 1000) * 100;
	else if(lineTime >=90 && lineTime <= 149)
		return (lineTime - 90 + 1100) * 100;
	else if (lineTime == 150)
		return 120000;
	else if (lineTime >= 151 && lineTime <= 209)
		return (lineTime - 150 + 1300) * 100;
	else if (lineTime >= 210 && lineTime <= 269)
		return (lineTime - 210 + 1400) * 100;
	else if(lineTime >= 270 && lineTime <= 329)
		return (lineTime - 270 + 1500) * 100;
	else if (lineTime >= 330)
		return 160000;
	return -1;
}


int getHKLineTime(int time)
{
	if (time < 91500)
		return -1;
	else if (time >= 91500 && time < 93000)
		return 0;
	else if (time >=93000 && time < 120000)
		return (time/10000-9)*60+time%10000/100-30+(time%100==0?0:1);
	else if (time >= 120000 && time < 130000)
		return 150;
	else if (time >= 130000 && time < 160000)
		return 151+(time/10000-13)*60+time%10000/100+(time%100==0?-1:0);
	else if (time >= 160000)
		return 330;
	return -1;
}
