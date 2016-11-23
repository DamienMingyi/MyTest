/*
 * HKPressListReply.h
 *
 *  Created on: Sep 24, 2015
 *      Author: level2
 */

#ifndef HKPRESSLISTREPLY_H_
#define HKPRESSLISTREPLY_H_

#include "../../public/reply/ListReply.h"

class HKPressListReply: public ListReply {
public:
	HKPressListReply();
	virtual ~HKPressListReply();

	virtual int request(const char* script,const char* query,char* buffer,int& len);
protected:
	virtual bool isExchangeSubType(string& subType,char* snap);
	bool getSelfList();
	bool getTypeList();
	bool getOrder(ListCmp& cmp);
	int  getSnapString(char* buffer,const char* select);
    virtual void registerCol();
    static COL_DESC_ST HKPressListDesc[];
};

#endif /* HKPRESSLISTREPLY_H_ */
