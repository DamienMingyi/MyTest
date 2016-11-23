/*
 * HKPressSnap.cpp
 *
 *  Created on: Sep 15, 2015
 *      Author: level2
 */

#include "HKPressSnap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

HKPressSnap::HKPressSnap() {
	// TODO Auto-generated constructor stub
	snap=NULL;
}

HKPressSnap::~HKPressSnap() {
	// TODO Auto-generated destructor stub
}

bool HKPressSnap::getValue(int id,char* v)
{
	if (snap==NULL)	return false;

	switch (id)
	{
	case SNAP_SECURITY_ID:
		strncpy(v,snap->securityID,sizeof(snap->securityID));
		return true;
	default:
		return false;
	}
}

bool HKPressSnap::getValue(int id,int& v)
{
	if (id>=0 && snap==NULL)
		return false;
	switch (id)
	{
	case SNAPTRADE_SIZE:
		v=sizeof(HKPTRADE);
		return true;
	case SNAPLINE_SIZE:
		v=sizeof(HKPLINE);
		return true;
	case SNAP_SIZE:
		v=sizeof(HKPSNAP);
		return true;
	case SNAP_DATE:
		v=snap->date;
		return true;
	case SNAP_INIT_TIME:
		if (strcmp(snap->securityID,"00000")==0)
		{
			v=snap->time;
			return true;
		}
		return false;
	case SNAP_TOTAL:
		if (strcmp(snap->securityID,"00000")==0)
		{
			v=snap->amount;
			return true;
		}
		return false;
	case SNAP_POS:
		v=snap->pos;
		return true;
	case SNAP_LINE_TIME:
		v=snap->lineTime;
		return true;
	default:
		return false;
	}
}

void HKPressSnap::lineUpdate()
{
	snap->lineTime = getHKLineTime(snap->time);
	if (snap->lineTime<0) return;
	//the new minute
	if(getHKLineTime(snap->line.dateTime) != getHKLineTime(snap->time) || snap->line.lastPx==0)
	{
		snap->prevLineVolume += snap->line.volume;
		snap->line.volume = snap->volume - snap->prevLineVolume;
		snap->prevLineAmount += snap->line.amount;
		snap->line.amount = snap->amount - snap->prevLineAmount;
		snap->line.openPx = (snap->lastPx != 0)?snap->lastPx:snap->prevClosePx;
		snap->line.highPx = snap->line.openPx;
		snap->line.lowPx = snap->line.openPx;
		snap->line.lastPx = snap->line.openPx;
		snap->line.dateTime = getHKDateTime(snap->lineTime);
	}
	//the same minute
	else if (snap->lastPx)
	{
		snap->line.volume += snap->volume - snap->prevVolume;
		snap->line.amount += snap->amount - snap->prevAmount;
		if(snap->line.highPx < snap->lastPx)
		{
			snap->line.highPx = snap->lastPx;
		}
		if(snap->line.lowPx > snap->lastPx)
		{
			snap->line.lowPx = snap->lastPx;
		}
		snap->line.lastPx = snap->lastPx;
	}
	if (snap->avgPx)
		snap->line.avgPx = snap->avgPx;
	else
		snap->line.avgPx=snap->line.lastPx;
}

void HKPressSnap::tradeUpdate()
{
	if (snap->volume <= snap->prevVolume)
		return ;
	snap->trade.time = snap->time;
	snap->trade.price = snap->lastPx;
	snap->trade.volume = snap->volume - snap->prevVolume;
	snap->trade.sign =snap->pin;
	if (snap->trade.sign)
		snap->in+=snap->trade.volume;
	else
		snap->out+=snap->trade.volume;
	snap->hand=snap->trade.volume;
}

void HKPressSnap::update()
{
	if(snap->amount > 0 && snap->volume > 0)
		snap->avgPx = snap->amount  * 1000 / snap->volume;
	lineUpdate();
	tradeUpdate();
}

char* HKPressSnap::getLine()
{
	if (snap->lineTime>=0)
		return (char*)&snap->line;
	return NULL;
}

char* HKPressSnap::getTrade()
{
	if (snap->volume>snap->prevVolume)
		return (char*)&snap->trade;
	return NULL;
}


void HKPressSnap::getDayCandlestick(CANDLESTICK* candlestick)
{
	candlestick->dateTime = snap->date;
	candlestick->volume = snap->volume;
	candlestick->amount = snap->amount;
	if (candlestick->volume > 0 && candlestick->amount > 0)
	{
		candlestick->openPx = snap->openPx;
		candlestick->highPx = snap->highPx;
		candlestick->lowPx =  snap->lowPx;
		candlestick->lastPx = snap->lastPx;
		candlestick->avgPx = snap->avgPx;
		/*
		if (snap->colse)
		{
			candlestick->highPx = (snap->highPx == 0)?snap->lastPx:snap->highPx;
			candlestick->lowPx (snap->lowPx == 0)?snap->lastPx:snap->lowPx;
		}
		*/
	}
	else
		candlestick->openPx = candlestick->highPx = candlestick->lowPx = candlestick->lastPx =
				candlestick->avgPx = candlestick->prevClosePx;
}

void HKPressSnap::getMinCandlestick(char* min, CANDLESTICK* candlestick)
{
	HKPLINE* line = reinterpret_cast<HKPLINE*>(min);
	candlestick->dateTime = snap->date * 100000 + line->dateTime;
	candlestick->openPx = line->openPx;
	candlestick->highPx = line->highPx;
	candlestick->lowPx = line->lowPx;
	candlestick->lastPx = line->lastPx;
	candlestick->amount = line->amount;
	candlestick->volume = line->volume;
	candlestick->avgPx = line->avgPx;
}

char* HKPressSnap::append(int position, char *item, unsigned long sz)
{
	memcpy(&ln, item, sizeof(HKPLINE));
	ln.amount = 0;
	ln.volume = 0;
	ln.dateTime = getHKDateTime(position);
	if(sz == 0)
		ln.openPx = ln.highPx = ln.lowPx = ln.lastPx = ln.avgPx = snap->prevClosePx;
	return reinterpret_cast<char*>(&ln);
}
