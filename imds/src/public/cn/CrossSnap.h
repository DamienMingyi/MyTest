/*
 * CrossSnap.h
 *
 *  Created on: 2016年10月21日
 *      Author: mds
 */

#ifndef SRC_PUBLIC_CROSSMARKET_CROSSSNAP_H_
#define SRC_PUBLIC_CROSSMARKET_CROSSSNAP_H_
#include "../cn/CroSnap.h"
#include "../market/ISnapShot.h"
#include "../market/Market.h"

class CrossSnap:public ISnapShot{
public:
	CrossSnap();
	virtual ~CrossSnap();

	virtual void attach(char* snap)
	{
		this->snap = reinterpret_cast<SNAP*>(snap);
	}

	virtual bool getValue(int id, char* v);
	virtual bool getValue(int id, int& v);
	virtual void update(){};
	virtual void lineUpdate(){};
	virtual void tradeUpdate(){};
	virtual char* getLine(){return NULL;};
	virtual char* getTrade(){return NULL;};

	virtual void getDayCandlestick(CANDLESTICK* candlestick){};
	virtual void getMinCandlestick(char* min, CANDLESTICK* candlestick){};
	virtual char* append(int position, char* item, unsigned long sz){return reinterpret_cast<char*>(NULL);};

private:
	SNAP* snap;
};

#endif /* SRC_PUBLIC_CROSSMARKET_CROSSSNAP_H_ */
