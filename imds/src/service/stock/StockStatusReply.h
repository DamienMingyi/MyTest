/*
 * StockStatusReply.h
 *
 *  Created on: Nov 4, 2015
 *      Author: level2
 */

#ifndef STOCKSTATUSREPLY_H_
#define STOCKSTATUSREPLY_H_

#include "../../public/reply/StatusReply.h"

class StockStatusReply: public StatusReply {
public:
	StockStatusReply();
	virtual ~StockStatusReply();
	virtual int request(const char* script,const char* query,char* buffer,int& len);
protected:
	virtual int getSnapString(char* buffer,const char* select);
};

#endif /* STOCKSTATUSREPLY_H_ */
