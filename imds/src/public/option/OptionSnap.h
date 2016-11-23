/*
 * OptionSnap.h
 *
 *  Created on: Sep 5, 2016
 *      Author: level2
 */

#ifndef OPTIONSNAP_H_
#define OPTIONSNAP_H_

#include "../market/ISnapShot.h"
#include "ShoSnap.h"
#include "RiskMeasureIndex.h"
#include "../market/Market.h"

class OptionSnap: public ISnapShot {
public:
	OptionSnap();
	virtual ~OptionSnap();

	virtual void attach(char* snap)
	{
		this->snap = reinterpret_cast<SHOSNAP*>(snap);
	}
	virtual bool getValue(int id, char* v);
	virtual bool getValue(int id, int& v);
	virtual void update();
	virtual void tradeUpdate();
	virtual void lineUpdate();
	virtual char* getTrade();
	virtual char* getLine();

	virtual void getDayCandlestick(CANDLESTICK* candlestick);
	virtual void getMinCandlestick(char* min, CANDLESTICK* candlestick);
	virtual char* append(int position, char* item, unsigned long sz);
private:
	SHOSNAP* snap;
	RiskMeasureIndex rmi;
	static const int32_t TRADING_TIME;// = 240;
	static const double YEAR_DAY;// = 360.0;
	static const int32_t TRADE_BEGIN;// = 92500;
	SHOLINE ln;

};

#endif /* OPTIONSNAP_H_ */
