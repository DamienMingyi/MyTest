/*
 * SzStockLineReply.h
 *
 *  Created on: Jun 24, 2016
 *      Author: level2
 */

#ifndef SZSTOCKLINEREPLY_H_
#define SZSTOCKLINEREPLY_H_

#include "../../public/reply/LineReply.h"

class SzStockLineReply: public LineReply {
public:
	SzStockLineReply();
	virtual ~SzStockLineReply();

	virtual int request(const char* script, const char* query, char* buffer, int& len);
protected:
	int getLineString(char* buffer, const char* select);
    virtual void registerCol();
    static COL_DESC_ST szStockLineDesc[];
};

#endif /* SZSTOCKLINEREPLY_H_ */
