/*
 * OptionMonitorReply.h
 *
 *  Created on: Oct 27, 2015
 *      Author: level2
 */

#ifndef OPTIONMONITORREPLY_H_
#define OPTIONMONITORREPLY_H_

#include "../../public/reply/MonitorReply.h"

class OptionMonitorReply: public MonitorReply {
public:
	OptionMonitorReply();
	virtual ~OptionMonitorReply();

	virtual int request(const char* script,const char* query,char* buffer,int& len);
protected:
	virtual int getSnapString(char* buffer,const char* select);
};

#endif /* OPTIONMONITORREPLY_H_ */
