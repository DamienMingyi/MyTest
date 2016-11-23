/*
 * IndexPressLineReply.h
 *
 *  Created on: Sep 9, 2015
 *      Author: level2
 */

#ifndef INDEXPRESSLINEREPLY_H_
#define INDEXPRESSLINEREPLY_H_

#include "../../public/reply/LineReply.h"

class IndexPressLineReply: public LineReply {
public:
	IndexPressLineReply();
	virtual ~IndexPressLineReply();

	virtual int request(const char* script,const char* query,char* buffer,int& len);
protected:
	int  getLineString(char* buffer,const char* select);
    virtual void registerCol();
    static COL_DESC_ST indexLineDesc[];
};

#endif /* INDEXPRESSLINEREPLY_H_ */
