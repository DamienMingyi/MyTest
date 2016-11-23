/*
 * IndexPressStatusReply.h
 *
 *  Created on: Nov 4, 2015
 *      Author: level2
 */

#ifndef INDEXPRESSSTATUSREPLY_H_
#define INDEXPRESSSTATUSREPLY_H_

#include "../../public/reply/StatusReply.h"

class IndexPressStatusReply: public StatusReply {
public:
	IndexPressStatusReply();
	virtual ~IndexPressStatusReply();
	virtual int request(const char* script,const char* query,char* buffer,int& len);
protected:
	virtual int getSnapString(char* buffer,const char* select);
};

#endif /* INDEXPRESSSTATUSREPLY_H_ */
