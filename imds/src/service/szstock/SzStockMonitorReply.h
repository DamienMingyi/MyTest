/*
 * SzStockMonitorReply.h
 *
 *  Created on: Jun 24, 2016
 *      Author: level2
 */

#ifndef SZSTOCKMONITORREPLY_H_
#define SZSTOCKMONITORREPLY_H_

#include "../../public/reply/MonitorReply.h"

class SzStockMonitorReply: public MonitorReply {
public:
	SzStockMonitorReply();
	virtual ~SzStockMonitorReply();

	virtual int request(const char* script, const char* query, char* buffer, int& len);

protected:
	virtual int getSnapString(char* buffer, const char* select);
};

#endif /* SZSTOCKMONITORREPLY_H_ */
