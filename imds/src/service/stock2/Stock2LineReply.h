/*
 * Stock2LineReply.h
 *
 *  Created on: Apr 14, 2014
 *      Author: level2
 */

#ifndef STOCK2LINEREPLY_H_
#define STOCK2LINEREPLY_H_

#include "../../public/reply/LineReply.h"

class Stock2LineReply: public LineReply {
public:
	Stock2LineReply();
	virtual ~Stock2LineReply();

	virtual int request(const char* script,const char* query,char* buffer,int& len);
protected:
	int  getLineString(char* buffer,const char* select);
    virtual void registerCol();
    static COL_DESC_ST stock2LineDesc[];
};

#endif /* STOCK2LINEREPLY_H_ */
