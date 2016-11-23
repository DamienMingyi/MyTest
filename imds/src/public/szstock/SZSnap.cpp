/*
 * SZSnap.cpp
 *
 *  Created on: May 31, 2016
 *      Author: level2
 */

#include "SZSnap.h"
#include <stdlib.h>
#include <stdio.h>

SZSnap::SZSnap() {
	// TODO Auto-generated constructor stub
	snap = NULL;
}

SZSnap::~SZSnap() {
	// TODO Auto-generated destructor stub
}

bool SZSnap::getValue(int id,char* v)
{
	if (snap == NULL)
		return false;

	switch (id)
	{
	case SNAP_SECURITY_ID:
		strncpy(v, snap->securityID, sizeof(snap->securityID));
		return true;
	default:
		return false;
	}
}

bool SZSnap::getValue(int id, int& v)
{
	if (snap == NULL)
		return false;
	switch (id)
	{
	case SNAPTRADE_SIZE:
		v = sizeof(SZ1TRADE);
		return true;
	case SNAPLINE_SIZE:
		v = sizeof(SZ1LINE);
		return true;
	case SNAP_SIZE:
		v = sizeof(SZSNAP);
		return true;
	case SNAP_DATE:
		v = snap->date;
		return true;
	case SNAP_INIT_TIME:
		if (strcmp(snap->securityID, "000000") == 0)
		{
			v = snap->time;
			return true;
		}
		return false;
	case SNAP_TOTAL:
		if (strcmp(snap->securityID, "000000") == 0)
		{
			v = snap->amount;
			return true;
		}
		return false;
	case SNAP_POS:
		v = snap->pos;
		return true;
	case SNAP_LINE_TIME:
		v = snap->lineTime;
		return true;
	default:
		return false;
	}
}

void SZSnap::update()
{
	if (snap->volume > 0 && snap->amount > 0)
		snap->avgPx = (snap->securityType == -1)?snap->prevClosePx:(snap->amount  * 1000 / snap->volume);

	lineUpdate();
	tradeUpdate();
}

void SZSnap::lineUpdate()
{
	snap->lineTime = getLineTime(snap->time);
	if (snap->lineTime < 0)
	{
		return ;
	}
	if (getLineTime(snap->time) != getLineTime(snap->line.dateTime) || snap->line.lastPx == 0)
	{
		snap->prevLineVolume += snap->line.volume;
		snap->prevLineAmount += snap->line.amount;
		snap->line.volume = snap->volume - snap->prevLineVolume;
		snap->line.amount = snap->amount - snap->prevLineAmount;

		snap->line.openPx = snap->lastPx != 0 ? snap->lastPx : snap->prevClosePx;

		snap->line.highPx = (snap->prevSnapHigh < snap->highPx) ? snap->highPx : snap->line.openPx;
		snap->line.lowPx = (snap->prevSnapLow > snap->lowPx) ? snap->lowPx : snap->line.openPx;

		snap->line.lastPx = snap->line.openPx;
		snap->line.dateTime = getDatetime(snap->lineTime);
	}
	else if (snap->lastPx)
	{
		snap->line.volume += snap->volume - snap->prevVolume;
		snap->line.amount += snap->amount - snap->prevAmount;

		if (snap->line.highPx < snap->lastPx)
			snap->line.highPx = snap->lastPx;
		if (snap->line.lowPx > snap->lastPx)
			snap->line.lowPx = snap->lastPx;
		snap->line.lastPx = snap->lastPx;
	}
	if (snap->avgPx)
		snap->line.avgPx = snap->avgPx;
	else
		snap->line.avgPx = snap->line.lastPx;
}

void SZSnap::tradeUpdate()
{
	if (snap->volume <= snap->prevVolume)
		return ;
	snap->trade.time = snap->time;
	snap->trade.price = snap->lastPx;
	snap->trade.volume = snap->volume - snap->prevVolume;
	snap->trade.sign = snap->pin;
	if (snap->trade.sign)
		snap->in += snap->trade.volume;
	else
		snap->out += snap->trade.volume;
	snap->hand = snap->trade.volume;
}

char* SZSnap::getLine()
{
	if (snap->lineTime>=0)
		return (char*)&snap->line;
	return NULL;
}

char* SZSnap::getTrade()
{
	if (snap->volume > snap->prevVolume)
		return (char*)&snap->trade;
	return NULL;
}


void SZSnap::getDayCandlestick(CANDLESTICK* candlestick)
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
	}
	else
		candlestick->openPx = candlestick->highPx = candlestick->lowPx = candlestick->lastPx =
				candlestick->avgPx = candlestick->prevClosePx;
}


void SZSnap::getMinCandlestick(char* min, CANDLESTICK* candlestick)
{
	SZ1LINE* line = reinterpret_cast<SZ1LINE*>(min);
	candlestick->dateTime = snap->date * 100000 + line->dateTime;
	candlestick->openPx = line->openPx;
	candlestick->highPx = line->highPx;
	candlestick->lowPx = line->lowPx;
	candlestick->lastPx = line->lastPx;
	candlestick->amount = line->amount;
	candlestick->volume = line->volume;
	candlestick->avgPx = line->avgPx;
}


char* SZSnap::append(int position, char *item, unsigned long sz)
{
	memcpy(&ln, item, sizeof(SZ1LINE));
	ln.amount = 0;
	ln.volume = 0;
	ln.dateTime = getDatetime(position);
	if(sz == 0)
		ln.openPx = ln.highPx = ln.lowPx = ln.lastPx = ln.avgPx = snap->prevClosePx;
	return reinterpret_cast<char*>(&ln);
}

