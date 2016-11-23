/*
 * HKPressSnapReply.h
 *
 *  Created on: Sep 24, 2015
 *      Author: level2
 */

#ifndef HKPRESSSNAPREPLY_H_
#define HKPRESSSNAPREPLY_H_

#include "../../public/reply/SnapReply.h"

class HKPressSnapReply: public SnapReply {
public:
	HKPressSnapReply();
	virtual ~HKPressSnapReply();

	virtual int request(const char* script,const char* query,char* buffer,int& len);
protected:
	virtual int getSnapString(char* buffer,const char* select);
    virtual void registerCol();
    static COL_DESC_ST HKPressSnapDesc[];
};

#endif /* HKPRESSSNAPREPLY_H_ */
