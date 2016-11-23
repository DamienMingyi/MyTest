/*
 * FixIncomeListReply.h
 *
 *  Created on: Aug 31, 2015
 *      Author: level2
 */

#ifndef FIXINCOMELISTREPLY_H_
#define FIXINCOMELISTREPLY_H_

#include "../../public/reply/ListReply.h"

class FixIncomeListReply: public ListReply {
public:
	FixIncomeListReply();
	virtual ~FixIncomeListReply();

	virtual int request(const char* script,const char* query,char* buffer,int& len);
protected:
	virtual bool isExchangeSubType(string& subType,char* snap);
	bool getSelfList();
	bool getTypeList();
	bool getOrder(ListCmp& cmp);
	int  getSnapString(char* buffer,const char* select);
    virtual void registerCol();
    static COL_DESC_ST fixIncomeListDesc[];
};

#endif /* FIXINCOMELISTREPLY_H_ */
