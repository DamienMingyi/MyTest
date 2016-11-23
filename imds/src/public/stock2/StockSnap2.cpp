/*
 * StockSnap2.cpp
 *
 *  Created on: Apr 8, 2014
 *      Author: level2
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "StockSnap2.h"

StockSnap2::StockSnap2() {
	// TODO Auto-generated constructor stub
	snap=NULL;
}

StockSnap2::~StockSnap2() {
	// TODO Auto-generated destructor stub
}


bool StockSnap2::getValue(int id,char* v)
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

bool StockSnap2::getValue(int id,int& v)
{
	if (id>=0 && snap==NULL)
		return false;
	switch (id)
	{
	case SNAPTRADE2_SIZE:
		v = sizeof(STOCK2TRADE2);
		return true;
	case SNAPTRADE_SIZE:
		v=sizeof(STOCKTRADE2);
		return true;
	case SNAPLINE_SIZE:
		v=sizeof(STOCKLINE2);
		return true;
	case SNAP_SIZE:
		v=sizeof(STOCKSNAP2);
		return true;
	case SNAP_DATE:
		v=snap->date;
		return true;
	case SNAP_INIT_TIME:
		if (strcmp(snap->securityID,"000000")==0)
		{
			v=snap->time;
			return true;
		}
		return false;
	case SNAP_TOTAL:
		if (strcmp(snap->securityID,"000000")==0)
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

char* StockSnap2::getLine()
{
	if (snap->lineTime>=0)
		return reinterpret_cast<char*>(&snap->line);
	return NULL;
}

char* StockSnap2::getTrade()
{
	/*
	if (snap->volume>snap->prevVolume)
		return (char*)&snap->trade;
	*/
	if (snap->lineTime >= 0)
		return reinterpret_cast<char*>(&snap->trade);
	return NULL;
}

char* StockSnap2::getTrade2()
{
	if(snap->tradeQty > 0)
		return reinterpret_cast<char*>(&snap->trade2);
	return NULL;
}

void StockSnap2::getDayCandlestick(CANDLESTICK* candlestick)
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

void StockSnap2::getMinCandlestick(char* min, CANDLESTICK* candlestick)
{
	STOCKLINE2* line = reinterpret_cast<STOCKLINE2*>(min);
	candlestick->dateTime = snap->date * 100000 + line->dateTime;
	candlestick->openPx = line->openPx;
	candlestick->highPx = line->highPx;
	candlestick->lowPx = line->lowPx;
	candlestick->lastPx = line->lastPx;
	candlestick->amount = line->amount;
	candlestick->volume = line->volume;
	candlestick->avgPx = line->avgPx;
}


void StockSnap2::update()
{
	if(snap->amount > 0 && snap->volume > 0)
		snap->avgPx = snap->amount *1000/ snap->volume;
	lineUpdate();
	tradeUpdate();
}

void StockSnap2::tradeUpdate2()
{
	if(snap->tradeQty <= 0 && snap->tradeMoney <= 0)
		return ;
	snap->trade2.time = snap->tradeTime;
	snap->trade2.price = snap->tradePrice;
	snap->trade2.volume = snap->tradeQty;
	if(strcmp("B", snap->tradeBSFlag) == 0)
		snap->trade2.sign = 1;
	else if(strcmp("S", snap->tradeBSFlag) == 0)
		snap->trade2.sign = 0;
	else if(strcmp("N", snap->tradeBSFlag) == 0)
		snap->trade2.sign = -1;

	if (snap->trade2.sign == 1)
		snap->out2+=snap->trade2.volume;
	else if(snap->trade2.sign == 0)
		snap->in2+=snap->trade2.volume;
	snap->hand2=snap->trade2.volume;
	snap->trade2.money = snap->tradeMoney;
	snap->trade2.buyNo = snap->tradeBuyNo;
	snap->trade2.sellNo = snap->tradeSellNo;
}

void StockSnap2::tradeUpdate()
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
void StockSnap2::lineUpdate()
{
	snap->lineTime = getLineTime(snap->time);
	if (snap->lineTime<0) return;
	//the new minute
	if(getLineTime(snap->line.dateTime) != getLineTime(snap->time) || snap->line.lastPx==0)
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
		if(snap->line.highPx < snap->lastPx)
			snap->line.highPx = snap->lastPx;
		if(snap->line.lowPx > snap->lastPx)
			snap->line.lowPx = snap->lastPx;
		snap->line.lastPx = snap->lastPx;
	}
	if (snap->avgPx)
		snap->line.avgPx = snap->avgPx;
	else
		snap->line.avgPx=snap->line.lastPx;
}

char* StockSnap2::append(int position, char *item, unsigned long sz)
{
	memcpy(&ln, item, sizeof(STOCKLINE2));
	ln.amount = 0;
	ln.volume = 0;
	ln.dateTime = getDatetime(position);
	if(sz == 0)
		ln.openPx = ln.highPx = ln.lowPx = ln.lastPx = ln.avgPx = snap->prevClosePx;
	return reinterpret_cast<char*>(&ln);
}
