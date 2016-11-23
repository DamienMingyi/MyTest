/*
 * Stock2SnapReply.h
 *
 *  Created on: Apr 14, 2014
 *      Author: level2
 */

#ifndef STOCK2SNAPREPLY_H_
#define STOCK2SNAPREPLY_H_

#include "../../public/reply/SnapReply.h"

class Stock2SnapReply: public SnapReply {
public:
	Stock2SnapReply();
	virtual ~Stock2SnapReply();

	virtual int request(const char* script,const char* query,char* buffer,int& len);
protected:
	virtual int getSnapString(char* buffer,const char* select);
    virtual void registerCol();
    static COL_DESC_ST stock2SnapDesc[];
};

#endif /* STOCK2SNAPREPLY_H_ */
