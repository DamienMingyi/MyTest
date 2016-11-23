/*
 * HKPressTrd1Reply.h
 *
 *  Created on: Sep 24, 2015
 *      Author: level2
 */

#ifndef HKPRESSTRD1REPLY_H_
#define HKPRESSTRD1REPLY_H_

#include "../../public/reply/Trd1Reply.h"

class HKPressTrd1Reply: public Trd1Reply {
public:
	HKPressTrd1Reply();
	virtual ~HKPressTrd1Reply();

	virtual int request(const char* script,const char* query,char* buffer,int& len);
protected:
	virtual int getTradeString(char* buffer,char* trd);
};

#endif /* HKPRESSTRD1REPLY_H_ */
