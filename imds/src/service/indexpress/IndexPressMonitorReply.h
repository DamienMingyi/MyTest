/*
 * IndexPressMonitorReply.h
 *
 *  Created on: Oct 27, 2015
 *      Author: level2
 */

#ifndef INDEXPRESSMONITORREPLY_H_
#define INDEXPRESSMONITORREPLY_H_

#include "../../public/reply/MonitorReply.h"

class IndexPressMonitorReply: public MonitorReply {
public:
	IndexPressMonitorReply();
	virtual ~IndexPressMonitorReply();

	virtual int request(const char* script,const char* query,char* buffer,int& len);
protected:
	virtual int getSnapString(char* buffer,const char* select);
};

#endif /* INDEXPRESSMONITORREPLY_H_ */
