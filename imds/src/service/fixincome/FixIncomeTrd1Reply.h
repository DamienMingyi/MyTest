/*
 * FixIncomeTrd1Reply.h
 *
 *  Created on: Aug 31, 2015
 *      Author: level2
 */

#ifndef FIXINCOMETRD1REPLY_H_
#define FIXINCOMETRD1REPLY_H_

#include "../../public/reply/Trd1Reply.h"

class FixIncomeTrd1Reply: public Trd1Reply {
public:
	FixIncomeTrd1Reply();
	virtual ~FixIncomeTrd1Reply();
	virtual int request(const char* script,const char* query,char* buffer,int& len);
protected:
	virtual int getTradeString(char* buffer,char* trd);
};

#endif /* FIXINCOMETRD1REPLY_H_ */
