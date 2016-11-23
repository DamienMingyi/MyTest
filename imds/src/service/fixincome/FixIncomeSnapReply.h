/*
 * FixIncomeSnapReply.h
 *
 *  Created on: Aug 31, 2015
 *      Author: level2
 */

#ifndef FIXINCOMESNAPREPLY_H_
#define FIXINCOMESNAPREPLY_H_

#include "../../public/reply/SnapReply.h"

class FixIncomeSnapReply: public SnapReply {
public:
	FixIncomeSnapReply();
	virtual ~FixIncomeSnapReply();
	virtual int request(const char* script,const char* query,char* buffer,int& len);
protected:
	virtual int getSnapString(char* buffer,const char* select);
    virtual void registerCol();
    static COL_DESC_ST fixIncomeSnapDesc[];
};

#endif /* FIXINCOMESNAPREPLY_H_ */
