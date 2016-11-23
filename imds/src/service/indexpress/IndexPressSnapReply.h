/*
 * IndexPressSnapReply.h
 *
 *  Created on: Sep 9, 2015
 *      Author: level2
 */

#ifndef INDEXPRESSSNAPREPLY_H_
#define INDEXPRESSSNAPREPLY_H_

#include "../../public/reply/SnapReply.h"

class IndexPressSnapReply: public SnapReply {
public:
	IndexPressSnapReply();
	virtual ~IndexPressSnapReply();
	virtual int request(const char* script,const char* query,char* buffer,int& len);
protected:
	virtual int getSnapString(char* buffer,const char* select);
    virtual void registerCol();
    static COL_DESC_ST indexSnapDesc[];
};

#endif /* INDEXPRESSSNAPREPLY_H_ */
