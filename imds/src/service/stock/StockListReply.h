/*
 * StockListReply.h
 *
 *  Created on: Mar 17, 2014
 *      Author: level2
 */

#ifndef STOCKLISTREPLY_H_
#define STOCKLISTREPLY_H_

#include "../../public/reply/ListReply.h"
class StockListReply: public ListReply {
public:
	StockListReply();
	virtual ~StockListReply();

	virtual int request(const char* script,const char* query,char* buffer,int& len);
protected:
	virtual bool isExchangeSubType(string& subType,char* snap);
	bool getSelfList();
	bool getTypeList();
	bool getOrder(ListCmp& cmp);
	int  getSnapString(char* buffer,const char* select);
};

#endif /* STOCKLISTREPLY_H_ */
