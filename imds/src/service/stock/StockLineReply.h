/*
 * StockLineReply.h
 *
 *  Created on: Mar 17, 2014
 *      Author: level2
 */

#ifndef STOCKLINEREPLY_H_
#define STOCKLINEREPLY_H_

#include "../../public/reply/LineReply.h"

class StockLineReply: public LineReply {
public:
	StockLineReply();
	virtual ~StockLineReply();

	virtual int request(const char* script,const char* query,char* buffer,int& len);
protected:
	int  getLineString(char* buffer,const char* select);
};

#endif /* STOCKLINEREPLY_H_ */
