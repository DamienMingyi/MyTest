/*
 * StatusReply.h
 *
 *  Created on: Nov 4, 2015
 *      Author: level2
 */

#ifndef STATUSREPLY_H_
#define STATUSREPLY_H_

#include "HttpReply.h"

class StatusReply: public HttpReply {
public:
	StatusReply();
	virtual ~StatusReply();
protected:
	virtual bool  parse(const char* script,const char* query);
	virtual int  getSnapString(char* buffer,const char* select)=0;

	virtual int reply(char* buffer,int& len);
	char* snap;
};

#endif /* STATUSREPLY_H_ */
