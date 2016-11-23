/*
 * SzStockTrd1Reply.h
 *
 *  Created on: Jun 24, 2016
 *      Author: level2
 */

#ifndef SZSTOCKTRD1REPLY_H_
#define SZSTOCKTRD1REPLY_H_

#include "../../public/reply/Trd1Reply.h"

class SzStockTrd1Reply: public Trd1Reply {
public:
	SzStockTrd1Reply();
	virtual ~SzStockTrd1Reply();

	virtual int request(const char* script, const char* query, char* buffer, int& len);

protected:
	virtual int getTradeString(char* buffer, char* trd);
};

#endif /* SZSTOCKTRD1REPLY_H_ */
