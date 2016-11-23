/*
 * FixIncomeLineReply.h
 *
 *  Created on: Aug 31, 2015
 *      Author: level2
 */

#ifndef FIXINCOMELINEREPLY_H_
#define FIXINCOMELINEREPLY_H_

#include "../../public/reply/LineReply.h"

class FixIncomeLineReply: public LineReply {
public:
	virtual ~FixIncomeLineReply();
	FixIncomeLineReply();

	virtual int request(const char* script,const char* query,char* buffer,int& len);
protected:
	int  getLineString(char* buffer,const char* select);
    virtual void registerCol();
    static COL_DESC_ST fixIncomeLineDesc[];
};

#endif /* FIXINCOMELINEREPLY_H_ */
