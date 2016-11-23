/*
 * IndexPressListReply.h
 *
 *  Created on: Sep 9, 2015
 *      Author: level2
 */

#ifndef INDEXPRESSLISTREPLY_H_
#define INDEXPRESSLISTREPLY_H_

#include "../../public/reply/ListReply.h"
class IndexPressListReply: public ListReply {
public:
	IndexPressListReply();
	virtual ~IndexPressListReply();

	virtual int request(const char* script,const char* query,char* buffer,int& len);
protected:
	virtual bool isExchangeSubType(string& subType,char* snap);

	bool getSelfList();
	bool getTypeList();
	bool getOrder(ListCmp& cmp);

	int  getSnapString(char* buffer,const char* select);
    virtual void registerCol();
    static COL_DESC_ST indexListDesc[];

};

#endif /* INDEXPRESSLISTREPLY_H_ */
