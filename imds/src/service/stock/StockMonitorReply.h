/*
 * StockMonitorReply.h
 *
 *  Created on: Oct 27, 2015
 *      Author: level2
 */

#ifndef STOCKMONITORREPLY_H_
#define STOCKMONITORREPLY_H_

#include "../../public/reply/MonitorReply.h"

class StockMonitorReply: public MonitorReply {
public:
	StockMonitorReply();
	virtual ~StockMonitorReply();

	virtual int request(const char* script,const char* query,char* buffer,int& len);
protected:
	virtual int getSnapString(char* buffer,const char* select);
};

#endif /* STOCKMONITORREPLY_H_ */
