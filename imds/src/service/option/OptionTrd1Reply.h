/*
 * OptionTrd1Reply.h
 *
 *  Created on: Mar 17, 2014
 *      Author: level2
 */

#ifndef OPTIONTRD1REPLY_H_
#define OPTIONTRD1REPLY_H_

#include "../../public/reply/Trd1Reply.h"

class OptionTrd1Reply: public Trd1Reply {
public:
	OptionTrd1Reply();
	virtual ~OptionTrd1Reply();

	virtual int request(const char* script,const char* query,char* buffer,int& len);
protected:
	virtual int getTradeString(char* buffer,char* trd);
};

#endif /* OPTIONTRD1REPLY_H_ */
