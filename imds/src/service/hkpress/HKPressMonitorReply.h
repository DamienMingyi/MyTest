/*
 * HKPressMonitorReply.h
 *
 *  Created on: Oct 27, 2015
 *      Author: level2
 */

#ifndef HKPRESSMONITORREPLY_H_
#define HKPRESSMONITORREPLY_H_

#include "../../public/reply/MonitorReply.h"

class HKPressMonitorReply: public MonitorReply {
public:
	HKPressMonitorReply();
	virtual ~HKPressMonitorReply();

	virtual int request(const char* script,const char* query,char* buffer,int& len);
protected:
	virtual int getSnapString(char* buffer,const char* select);
};

#endif /* HKPRESSMONITORREPLY_H_ */
