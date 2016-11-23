/*
 * Stock2Trd1Reply.h
 *
 *  Created on: Apr 14, 2014
 *      Author: level2
 */

#ifndef STOCK2TRD1REPLY_H_
#define STOCK2TRD1REPLY_H_

#include "../../public/reply/Trd1Reply.h"

class Stock2Trd1Reply: public Trd1Reply {
public:
	Stock2Trd1Reply();
	virtual ~Stock2Trd1Reply();

	virtual int request(const char* script,const char* query,char* buffer,int& len);
protected:
	virtual int getTradeString(char* buffer,char* trd);
};

#endif /* STOCK2TRD1REPLY_H_ */
