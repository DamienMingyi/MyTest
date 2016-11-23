/*
 * FixIncomeTrd2Reply.h
 *
 *  Created on: Aug 31, 2015
 *      Author: level2
 */

#ifndef FIXINCOMETRD2REPLY_H_
#define FIXINCOMETRD2REPLY_H_

#include "../../public/reply/Trd2Reply.h"

class FixIncomeTrd2Reply: public Trd2Reply {
public:
	FixIncomeTrd2Reply();
	virtual ~FixIncomeTrd2Reply();

	virtual int request(const char* script,const char* query,char* buffer,int& len);
};

#endif /* FIXINCOMETRD2REPLY_H_ */
