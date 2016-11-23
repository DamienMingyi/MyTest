/*
 * Stock2TrdReply.h
 *
 *  Created on: Jun 13, 2014
 *      Author: level2
 */

#ifndef STOCK2TRDREPLY_H_
#define STOCK2TRDREPLY_H_

#include "../../public/reply/Trd0Reply.h"

class Stock2Trd0Reply: public Trd0Reply {
public:
	Stock2Trd0Reply();
	virtual ~Stock2Trd0Reply();

	virtual int request(const char* script,const char* query,char* buffer,int& len);
protected:
	virtual int getTradeString(char* buffer,char* trd);
};

#endif /* STOCK2TRDREPLY_H_ */
