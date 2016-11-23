/*
 * SzStockStatusReply.h
 *
 *  Created on: Jun 24, 2016
 *      Author: level2
 */

#ifndef SZSTOCKSTATUSREPLY_H_
#define SZSTOCKSTATUSREPLY_H_

#include "../../public/reply/StatusReply.h"

class SzStockStatusReply: public StatusReply {
public:
	SzStockStatusReply();
	virtual ~SzStockStatusReply();

	virtual int request(const char* script, const char* query, char* buffer, int& len);
protected:
	virtual int getSnapString(char* buffer, const char* select);
};

#endif /* SZSTOCKSTATUSREPLY_H_ */
