/*
 * HKPressTrd2Reply.h
 *
 *  Created on: Sep 24, 2015
 *      Author: level2
 */

#ifndef HKPRESSTRD2REPLY_H_
#define HKPRESSTRD2REPLY_H_

#include "../../public/reply/Trd2Reply.h"

class HKPressTrd2Reply: public Trd2Reply {
public:
	HKPressTrd2Reply();
	virtual ~HKPressTrd2Reply();

	virtual int request(const char* script,const char* query,char* buffer,int& len);
};

#endif /* HKPRESSTRD2REPLY_H_ */
