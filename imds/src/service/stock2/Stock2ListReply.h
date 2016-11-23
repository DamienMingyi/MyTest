/*
 * Stock2ListReply.h
 *
 *  Created on: Apr 14, 2014
 *      Author: level2
 */

#ifndef STOCK2LISTREPLY_H_
#define STOCK2LISTREPLY_H_

#include "../../public/reply/ListReply.h"
class Stock2ListReply: public ListReply {
public:
	Stock2ListReply();
	virtual ~Stock2ListReply();

	virtual int request(const char* script,const char* query,char* buffer,int& len);
protected:
	bool getSelfList();
	bool getTypeList();
	bool getOrder(ListCmp& cmp);
	int  getSnapString(char* buffer,const char* select);
    virtual void registerCol();
    static COL_DESC_ST stock2ListDesc[];
};

#endif /* STOCK2LISTREPLY_H_ */
