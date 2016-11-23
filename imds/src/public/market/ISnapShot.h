/*
 * ISnapShot.h
 *
 *  Created on: Sep 5, 2016
 *      Author: level2
 */

#ifndef ISNAPSHOT_H_
#define ISNAPSHOT_H_

#include "ISnap.h"
#include <cstring>

const int SNAPTRADE2_SIZE = -4;
const int SNAPTRADE_SIZE=-3;
const int SNAPLINE_SIZE=-2;
const int SNAP_SIZE=-1;
const int SNAP_DATE=0;
const int SNAP_INIT_TIME=1;
const int SNAP_TOTAL=2;
const int SNAP_POS=3;
const int SNAP_SECURITY_ID=4;
const int SNAP_LINE_TIME=5;

class ISnapShot {
public:
	ISnapShot() {};
	virtual ~ISnapShot() {};

	virtual void attach(char* snap) = 0;

	virtual bool getValue(int id, char* v) = 0;
	virtual bool getValue(int id, int& v) = 0;
	virtual void update() = 0;
	virtual void lineUpdate() = 0;
	virtual void tradeUpdate() = 0;
	virtual char* getLine() = 0;
	virtual char* getTrade() = 0;
	/*add by zhuxh*/
	virtual char* getTrade2() {return NULL;};
	/*end by zhuxh*/
	virtual void getDayCandlestick(CANDLESTICK* candlestick) = 0;
	virtual void getMinCandlestick(char* min, CANDLESTICK* candlestick) = 0;

	virtual char* append(int position, char* item, unsigned long sz) {return item;};

};

#endif /* ISNAPSHOT_H_ */
