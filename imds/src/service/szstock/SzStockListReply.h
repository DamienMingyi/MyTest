/*
 * SzStockListReply.h
 *
 *  Created on: Jun 24, 2016
 *      Author: level2
 */

#ifndef SZSTOCKLISTREPLY_H_
#define SZSTOCKLISTREPLY_H_

#include "../../public/reply/ListReply.h"

class SzStockListReply: public ListReply {
public:
	SzStockListReply();
	virtual ~SzStockListReply();

	virtual int request(const char* script, const char* query, char* buffer, int& len);

protected:
	virtual bool isExchangeSubType(string& subType, char* snap);
	bool getSelfList();
	bool getTypeList();
	bool getOrder(ListCmp& cmp);
	int getSnapString(char* buffer, const char* select);
    virtual void registerCol();
    static COL_DESC_ST szStockListDesc[];
};

#endif /* SZSTOCKLISTREPLY_H_ */
