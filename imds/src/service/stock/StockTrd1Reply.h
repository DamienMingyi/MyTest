/*
 * StockTrd1Reply.h
 *
 *  Created on: Mar 17, 2014
 *      Author: level2
 */

#ifndef STOCKTRD1REPLY_H_
#define STOCKTRD1REPLY_H_

#include "../../public/reply/Trd1Reply.h"


class StockTrd1Reply: public Trd1Reply {
public:
	StockTrd1Reply();
	virtual ~StockTrd1Reply();

	virtual int request(const char* script,const char* query,char* buffer,int& len);
protected:
	virtual int getTradeString(char* buffer,char* trd);
};

#endif /* STOCKTRD1REPLY_H_ */
