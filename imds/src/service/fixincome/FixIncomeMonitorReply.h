/*
 * FixIncomeMonitorReply.h
 *
 *  Created on: Oct 27, 2015
 *      Author: level2
 */

#ifndef FIXINCOMEMONITORREPLY_H_
#define FIXINCOMEMONITORREPLY_H_

#include "../../public/reply/MonitorReply.h"

class FixIncomeMonitorReply: public MonitorReply {
public:
	FixIncomeMonitorReply();
	virtual ~FixIncomeMonitorReply();

	virtual int request(const char* script,const char* query,char* buffer,int& len);
protected:
	virtual int getSnapString(char* buffer,const char* select);
};

#endif /* FIXINCOMEMONITORREPLY_H_ */
