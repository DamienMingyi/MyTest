/*
 * StockSnapReply.h
 *
 *  Created on: Mar 17, 2014
 *      Author: level2
 */

#ifndef STOCKSNAPREPLY_H_
#define STOCKSNAPREPLY_H_

#include "../../public/reply/SnapReply.h"

class StockSnapReply: public SnapReply {
public:
	StockSnapReply();
	virtual ~StockSnapReply();

	virtual int request(const char* script,const char* query,char* buffer,int& len);
protected:
	virtual int getSnapString(char* buffer,const char* select);
};

#endif /* STOCKSNAPREPLY_H_ */
