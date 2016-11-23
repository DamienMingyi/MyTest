/*
 * HKPressSnap.h
 *
 *  Created on: Sep 15, 2015
 *      Author: level2
 */

#ifndef HKPRESSSNAP_H_
#define HKPRESSSNAP_H_


#include "../market/Market.h"
#include "../market/ISnap.h"
#include "./HKPSnap.h"

class HKPressSnap: public ISnapShot {
public:
	HKPressSnap();
	virtual ~HKPressSnap();

	virtual void attach(char* snap)
	{
		this->snap = reinterpret_cast<HKPSNAP*>(snap);
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
	HKPSNAP* snap;
	HKPLINE ln;
};

#endif /* HKPRESSSNAP_H_ */
