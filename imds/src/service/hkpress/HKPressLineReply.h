/*
 * HKPressLineReply.h
 *
 *  Created on: Sep 24, 2015
 *      Author: level2
 */

#ifndef HKPRESSLINEREPLY_H_
#define HKPRESSLINEREPLY_H_

#include "../../public/reply/LineReply.h"

class HKPressLineReply: public LineReply {
public:
	HKPressLineReply();
	virtual ~HKPressLineReply();

	virtual int request(const char* script,const char* query,char* buffer,int& len);
protected:
	int  getLineString(char* buffer,const char* select);
    virtual void registerCol();
    static COL_DESC_ST HKPressLineDesc[];
};

#endif /* HKPRESSLINEREPLY_H_ */
