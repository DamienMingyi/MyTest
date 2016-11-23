/*
 * FixIncomeTrd0Reply.h
 *
 *  Created on: Aug 31, 2015
 *      Author: level2
 */

#ifndef FIXINCOMETRD0REPLY_H_
#define FIXINCOMETRD0REPLY_H_

#include "../../public/reply/Trd0Reply.h"

class FixIncomeTrd0Reply: public Trd0Reply {
public:
	FixIncomeTrd0Reply();
	virtual ~FixIncomeTrd0Reply();

	virtual int request(const char* script,const char* query,char* buffer,int& len);
protected:
	virtual int getTradeString(char* buffer,char* trd);
};

#endif /* FIXINCOMETRD0REPLY_H_ */
