/*
 * Stock2Trd2Reply.h
 *
 *  Created on: Apr 14, 2014
 *      Author: level2
 */

#ifndef STOCK2TRD2REPLY_H_
#define STOCK2TRD2REPLY_H_

#include "../../public/reply/Trd2Reply.h"

class Stock2Trd2Reply: public Trd2Reply {
public:
	Stock2Trd2Reply();
	virtual ~Stock2Trd2Reply();

	virtual int request(const char* script,const char* query,char* buffer,int& len);
};

#endif /* STOCK2TRD2REPLY_H_ */
