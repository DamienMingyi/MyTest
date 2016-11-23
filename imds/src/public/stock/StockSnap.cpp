/*
 * StockSnap.cpp
 *
 *  Created on: Feb 21, 2014
 *      Author: level2
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "StockSnap.h"

char STOCKSNAP::tradingSessionID[8];

StockSnap::StockSnap(): ISnapShot()
{
	snap = NULL;
}

StockSnap::~StockSnap()
{
}

bool StockSnap::getValue(int id,char* v)
{
	if (snap == NULL)	return false;

	switch (id)
	{
	case SNAP_SECURITY_ID:
		strncpy(v, snap->securityID, sizeof(snap->securityID));
		return true;
	default:
		return false;
	}
}

bool StockSnap::getValue(int id,int& v)
{
	if (id >= 0 && snap == NULL)
		return false;
	switch (id)
	{
	case SNAPTRADE_SIZE:
		v = sizeof(STOCKTRADE);
		return true;
	case SNAPLINE_SIZE:
		v = sizeof(STOCKLINE);
		return true;
	case SNAP_SIZE:
		v = sizeof(STOCKSNAP);
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
		if (strcmp(snap->securityID,"000000") == 0)
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


void StockSnap::lineUpdate()
{
	snap->lineTime = getLineTime(snap->time);
	if (snap->lineTime < 0) return;

	//the new minute
	if(getLineTime(snap->line.dateTime) !=snap->lineTime || snap->line.lastPx == 0)
	{
		snap->prevLineVolume += snap->line.volume;
		snap->line.volume = snap->volume - snap->prevLineVolume;
		snap->prevLineAmount += snap->line.amount;
		snap->line.amount = snap->amount - snap->prevLineAmount;
		snap->line.openPx = (snap->lastPx != 0)?snap->lastPx:snap->prevClosePx;

		snap->line.highPx = (snap->prevSnapHigh < snap->highPx) ? snap->highPx : snap->line.openPx;
		snap->line.lowPx = (snap->prevSnapLow > snap->lowPx) ? snap->lowPx : snap->line.openPx;

		snap->line.lastPx = snap->line.openPx;
		snap->line.dateTime = getDatetime(snap->lineTime);
	}
	//the same minute
	else if (snap->lastPx)
	{
		snap->line.volume += snap->volume - snap->prevVolume;
		snap->line.amount += snap->amount - snap->prevAmount;

		snap->line.highPx = (snap->prevSnapHigh < snap->highPx) ? snap->highPx : snap->lastPx;
		snap->line.lowPx = (snap->prevSnapLow > snap->lowPx) ? snap->lowPx : snap->lastPx;

		snap->line.lastPx = snap->lastPx;
	}
	if (snap->avgPx)
		snap->line.avgPx = snap->avgPx;
	else
		snap->line.avgPx =snap->line.lastPx;
	snap->hand=snap->trade.volume;
}

void StockSnap::tradeUpdate()
{
	if (snap->volume < snap->prevVolume || snap->amount < snap->prevAmount)
		return ;
	snap->trade.price = snap->lastPx;
	snap->trade.time = snap->time;
	snap->trade.volume = snap->volume - snap->prevVolume;
	snap->trade.sign = snap->pin;
	if (snap->trade.sign)
		snap->in += snap->trade.volume;
	else
		snap->out += snap->trade.volume;
}


void StockSnap::update()
{
	if(snap->amount > 0 && snap->volume > 0)
	{
		if (atoi(snap->securityID) <= 999)
			snap->avgPx = snap->prevClosePx;
		else
			snap->avgPx = (strcmp(snap->cpxx.type, "D") != 0)?(snap->amount *1000/ snap->volume):(snap->amount *100/ snap->volume);
	}
	lineUpdate();
	tradeUpdate();
}


char* StockSnap::getLine()
{
	if (snap->lineTime >= 0)
		return reinterpret_cast<char*>(&snap->line);
	return NULL;
}


char* StockSnap::getTrade()
{
	if (snap->lineTime >= 0)
		return reinterpret_cast<char*>(&snap->trade);
	return NULL;
}


void StockSnap::getDayCandlestick(CANDLESTICK* candlestick)
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


void StockSnap::getMinCandlestick(char* min, CANDLESTICK* candlestick)
{
	STOCKLINE* line = reinterpret_cast<STOCKLINE*>(min);
	candlestick->dateTime = snap->date * 100000 + line->dateTime;
	candlestick->openPx = line->openPx;
	candlestick->highPx = line->highPx;
	candlestick->lowPx = line->lowPx;
	candlestick->lastPx = line->lastPx;
	candlestick->amount = line->amount;
	candlestick->volume = line->volume;
	candlestick->avgPx = line->avgPx;
}


char* StockSnap::append(int position, char *item, unsigned long sz)
{
	memcpy(&ln, item, sizeof(STOCKLINE));
	ln.amount = 0;
	ln.volume = 0;
	ln.dateTime = getDatetime(position);
	if(sz == 0)
		ln.openPx = ln.highPx = ln.lowPx = ln.lastPx = ln.avgPx = snap->prevClosePx;
	return reinterpret_cast<char*>(&ln);
}
