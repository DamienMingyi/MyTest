/*
 * OptionSnap.cpp
 *
 *  Created on: Sep 5, 2016
 *      Author: level2
 */

#include "OptionSnap.h"
#include <stdlib.h>

double SHOSNAP::riskFreeInterestRate;
char SHOSNAP::tradingSessionID[8];

const int32_t OptionSnap::TRADING_TIME = 240;
const double OptionSnap::YEAR_DAY = 360.0;
const int32_t OptionSnap::TRADE_BEGIN = 92500;

OptionSnap::OptionSnap(): snap(NULL) {
	// TODO Auto-generated constructor stub

}

OptionSnap::~OptionSnap() {
	// TODO Auto-generated destructor stub
}

bool OptionSnap::getValue(int id,char* v)
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

bool OptionSnap::getValue(int id, int& v)
{
	if (id>=0 && snap==NULL)
		return false;
	switch (id)
	{
	case SNAPTRADE_SIZE:
		v=sizeof(SHOTRADE);
		return true;
	case SNAPLINE_SIZE:
		v=sizeof(SHOLINE);
		return true;
	case SNAP_SIZE:
		v=sizeof(SHOSNAP);
		return true;
	case SNAP_DATE:
		v=snap->date;
		return true;
	case SNAP_INIT_TIME:
		if (strcmp(snap->securityID,"00000000")==0)
		{
			v=snap->time;
			return true;
		}
		return false;
	case SNAP_TOTAL:
		if (strcmp(snap->securityID,"00000000")==0)
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

void OptionSnap::update()
{
	int64_t lastprice = (snap->stockLastPx != 0) ? snap->stockLastPx : snap->underlyingClosePx;
	int64_t optionlastprice = (snap->lastPx != 0)? snap->lastPx : snap->prevSettlePrice;
	if(optionlastprice == 0)
		return ;
	int64_t difference = lastprice - snap->exercisePrice;

	if(snap->callOrPut[0] == 'C')
	{
		if(difference < 0)
			snap->inValue = 0;
		else
			snap->inValue = difference;
	}
	else if(snap->callOrPut[0] == 'P')
	{
		if(difference > 0)
			snap->inValue = 0;
		else
			snap->inValue = -difference ;
	}

	snap->timeValue = optionlastprice - snap->inValue;

	if(snap->amount > 0 && snap->volume > 0)
		snap->avgPx = snap->amount * 100 / (snap->volume * snap->contractMultiplierUnit);

	snap->leverageRatio = lastprice / optionlastprice;

	snap->premiumRate = (snap->callOrPut[0] == 'C')? (snap->exercisePrice + optionlastprice) * 10000 / lastprice - 10000 :
			10000 - (snap->exercisePrice + optionlastprice) * 10000 / lastprice;

	if(snap->time > TRADE_BEGIN)
	{
		if (TRADING_TIME * snap->remainingDays + TRADING_TIME - getLineTime(snap->time) != 0)
		{
			if (snap->exercisePrice != 0)
			{
				snap->impliedVolatility = rmi.getImpliedVolatility(optionlastprice / 10000.0, lastprice / 10000.0, snap->exercisePrice / 10000.0,
						(TRADING_TIME * snap->remainingDays + TRADING_TIME - getLineTime(snap->time)) / (TRADING_TIME * YEAR_DAY),
						snap->riskFreeInterestRate / 100.0, snap->callOrPut);
				snap->gamma = rmi.getGamma(optionlastprice / 10000.0, lastprice / 10000.0, snap->exercisePrice / 10000.0,
						(TRADING_TIME * snap->remainingDays + TRADING_TIME - getLineTime(snap->time)) / (TRADING_TIME * YEAR_DAY),
						snap->riskFreeInterestRate / 100.0, snap->callOrPut, snap->impliedVolatility);
				snap->theta = rmi.getTheta(optionlastprice / 10000.0, lastprice / 10000.0, snap->exercisePrice / 10000.0,
						(TRADING_TIME * snap->remainingDays + TRADING_TIME - getLineTime(snap->time)) / (TRADING_TIME * YEAR_DAY),
						snap->riskFreeInterestRate / 100.0, snap->callOrPut, snap->impliedVolatility);
				snap->vega = rmi.getVega(optionlastprice / 10000.0, lastprice / 10000.0, snap->exercisePrice / 10000.0,
						(TRADING_TIME * snap->remainingDays + TRADING_TIME - getLineTime(snap->time)) / (TRADING_TIME * YEAR_DAY),
						snap->riskFreeInterestRate / 100.0, snap->callOrPut, snap->impliedVolatility);
				snap->rho = rmi.getRho(optionlastprice / 10000.0, lastprice / 10000.0, snap->exercisePrice / 10000.0,
						(TRADING_TIME * snap->remainingDays + TRADING_TIME - getLineTime(snap->time)) / (TRADING_TIME * YEAR_DAY),
						snap->riskFreeInterestRate / 100.0, snap->callOrPut, snap->impliedVolatility);
				if (lastprice != 0)
					snap->delta = rmi.getDelta(optionlastprice / 10000.0, lastprice / 10000.0, snap->exercisePrice / 10000.0,
							(TRADING_TIME * snap->remainingDays + TRADING_TIME - getLineTime(snap->time)) / (TRADING_TIME * YEAR_DAY),
							snap->riskFreeInterestRate / 100.0, snap->callOrPut, snap->impliedVolatility);
				snap->impliedVolatility *= 100;
			}
		}
	}

	lineUpdate();

	tradeUpdate();
}

void OptionSnap::lineUpdate()
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
		snap->line.openPx = (snap->lastPx != 0)?snap->lastPx:snap->prevSettlePrice;

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

void OptionSnap::tradeUpdate()
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

char* OptionSnap::getLine()
{
	if (snap->lineTime >= 0)
		return reinterpret_cast<char*>(&snap->line);
	return NULL;
}

char* OptionSnap::getTrade()
{
	if (snap->lineTime >= 0)
		return reinterpret_cast<char*>(&snap->trade);
	return NULL;
}

void OptionSnap::getDayCandlestick(CANDLESTICK* candlestick)
{
	candlestick->dateTime = snap->date;
	candlestick->volume = snap->volume;
	candlestick->amount = snap->amount;
	candlestick->inValue = snap->inValue;
	candlestick->timeValue = snap->timeValue;
	candlestick->openInterest = snap->openInterest;
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

void OptionSnap::getMinCandlestick(char* min, CANDLESTICK* candlestick)
{
	SHOLINE* line = reinterpret_cast<SHOLINE*>(min);
	candlestick->dateTime = snap->date * 100000 + line->dateTime;
	candlestick->openPx = line->openPx;
	candlestick->highPx = line->highPx;
	candlestick->lowPx = line->lowPx;
	candlestick->lastPx = line->lastPx;
	candlestick->amount = line->amount;
	candlestick->volume = line->volume;
	candlestick->avgPx = line->avgPx;
	candlestick->openInterest = line->openInterest;
	candlestick->timeValue = line->timeValue;
	candlestick->inValue = line->inValue;
}

char* OptionSnap::append(int position, char* item, unsigned long sz)
{
	memcpy(&ln, item, sizeof(SHOLINE));
	ln.amount = 0;
	ln.volume = 0;
	ln.dateTime = getDatetime(position);
	if(sz == 0)
		ln.openPx = ln.highPx = ln.lowPx = ln.lastPx = ln.avgPx = snap->prevClosePx;

	return reinterpret_cast<char*>(&ln);
}
