/*
 * OptionTrd2Reply.h
 *
 *  Created on: Mar 17, 2014
 *      Author: level2
 */

#ifndef OPTIONTRD2REPLY_H_
#define OPTIONTRD2REPLY_H_

#include "../../public/reply/Trd2Reply.h"

class OptionTrd2Reply: public Trd2Reply {
public:
	OptionTrd2Reply();
	virtual ~OptionTrd2Reply();

	virtual int request(const char* script,const char* query,char* buffer,int& len);
};

#endif /* OPTIONTRD2REPLY_H_ */
