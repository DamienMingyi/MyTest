/*
 * SzStockTrd2Reply.h
 *
 *  Created on: Jun 24, 2016
 *      Author: level2
 */

#ifndef SZSTOCKTRD2REPLY_H_
#define SZSTOCKTRD2REPLY_H_

#include "../../public/reply/Trd2Reply.h"

class SzStockTrd2Reply: public Trd2Reply {
public:
	SzStockTrd2Reply();
	virtual ~SzStockTrd2Reply();

	virtual int request(const char* script, const char* query, char* buffer, int& len);
};

#endif /* SZSTOCKTRD2REPLY_H_ */
