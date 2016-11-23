/*
 * HKPressStatusReply.h
 *
 *  Created on: Nov 4, 2015
 *      Author: level2
 */

#ifndef HKPRESSSTATUSREPLY_H_
#define HKPRESSSTATUSREPLY_H_

#include "../../public/reply/StatusReply.h"

class HKPressStatusReply: public StatusReply {
public:
	HKPressStatusReply();
	virtual ~HKPressStatusReply();
	virtual int request(const char* script,const char* query,char* buffer,int& len);
protected:
	virtual int getSnapString(char* buffer,const char* select);
};

#endif /* HKPRESSSTATUSREPLY_H_ */
