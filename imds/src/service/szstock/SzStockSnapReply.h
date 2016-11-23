/*
 * SzStockSnapReply.h
 *
 *  Created on: Jun 24, 2016
 *      Author: level2
 */

#ifndef SZSTOCKSNAPREPLY_H_
#define SZSTOCKSNAPREPLY_H_

#include "../../public/reply/SnapReply.h"

class SzStockSnapReply: public SnapReply {
public:
	SzStockSnapReply();
	virtual ~SzStockSnapReply();

	virtual int request(const char* script, const char* query, char* buffer, int& len);

protected:
	virtual int getSnapString(char* buffer, const char* select);
    virtual void registerCol();
    static COL_DESC_ST szStockSnapDesc[];
};

#endif /* SZSTOCKSNAPREPLY_H_ */
