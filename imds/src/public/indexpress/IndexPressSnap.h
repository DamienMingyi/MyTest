/*
 * IndexPressSnap.h
 *
 *  Created on: Sep 1, 2015
 *      Author: level2
 */

#ifndef INDEXPRESSSNAP_H_
#define INDEXPRESSSNAP_H_


#include "../market/Market.h"
#include "../market/ISnap.h"
#include "./IndexSnap.h"

class IndexPressSnap: public ISnapShot {
public:
	IndexPressSnap();
	virtual ~IndexPressSnap();

	virtual void attach(char* snap)
	{
		this->snap = reinterpret_cast<INDEXSNAP*>(snap);
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
	INDEXSNAP* snap;
	INDEXLINE ln;
};

#endif /* INDEXPRESSSNAP_H_ */
