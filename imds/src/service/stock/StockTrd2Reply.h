/*
 * StockTrd2Reply.h
 *
 *  Created on: Mar 17, 2014
 *      Author: level2
 */

#ifndef STOCKTRD2REPLY_H_
#define STOCKTRD2REPLY_H_

#include "../../public/reply/Trd2Reply.h"

class StockTrd2Reply: public Trd2Reply
{
public:
	StockTrd2Reply();
	virtual ~StockTrd2Reply();

	virtual int request(const char* script,const char* query,char* buffer,int& len);

};

#endif /* STOCKTRD2REPLY_H_ */
