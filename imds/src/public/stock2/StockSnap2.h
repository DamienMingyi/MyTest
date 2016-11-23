/*
 * StockSnap2.h
 *
 *  Created on: Apr 8, 2014
 *      Author: level2
 */

#ifndef STOCKSNAP2_H_
#define STOCKSNAP2_H_

#include "../market/Market.h"
#include "../market/ISnapShot.h"
#include "../market/ISnap.h"
#include "Sh2Snap.h"

class StockSnap2:public ISnapShot{
public:
	StockSnap2();
	virtual ~StockSnap2();

	virtual void attach(char* snap)
	{
		this->snap = reinterpret_cast<STOCKSNAP2*>(snap);
	}

	virtual bool getValue(int id,char* v);
	virtual bool getValue(int id,int& v);

	virtual void update();
	virtual void lineUpdate();
	virtual void tradeUpdate();
	virtual void tradeUpdate2();
	virtual char* getLine();
	virtual char* getTrade();
	virtual char* getTrade2();

	virtual void getDayCandlestick(CANDLESTICK* candlestick);
	virtual void getMinCandlestick(char* min, CANDLESTICK* candlestick);
	virtual char* append(int position, char* item, unsigned long sz);
private:
	STOCKSNAP2* snap;
	STOCKLINE2 ln;
};

#endif /* STOCKSNAP2_H_ */
