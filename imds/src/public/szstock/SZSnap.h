/*
 * SZSnap.h
 *
 *  Created on: May 31, 2016
 *      Author: level2
 */

#ifndef SZSNAP_H_
#define SZSNAP_H_

#include "../market/Market.h"
#include "../market/ISnap.h"
#include "./SZv5Snap.h"

class SZSnap: public ISnapShot {
public:
	SZSnap();
	virtual ~SZSnap();

	virtual void attach(char* snap)
	{
		this->snap = reinterpret_cast<SZSNAP*>(snap);
	}

	virtual bool getValue(int id,char* v);
	virtual bool getValue(int id,int& v);

	virtual void update();
	virtual void lineUpdate();
	virtual void tradeUpdate();
	virtual char* getLine();
	virtual char* getTrade();

	virtual void getDayCandlestick(CANDLESTICK* candlestick);
	virtual void getMinCandlestick(char* min, CANDLESTICK* candlestick);
	virtual char* append(int position, char* item, unsigned long sz);
private:
	SZSNAP* snap;
	SZ1LINE ln;
};

#endif /* SZSNAP_H_ */
