/*
 * MonitorReply.h
 *
 *  Created on: Oct 27, 2015
 *      Author: level2
 */

#ifndef MONITORREPLY_H_
#define MONITORREPLY_H_

#include "HttpReply.h"

class MonitorReply: public HttpReply {
public:
	MonitorReply();
	virtual ~MonitorReply();

protected:
	virtual bool  parse(const char* script,const char* query);
	virtual int  getSnapString(char* buffer,const char* select)=0;

	virtual int reply(char* buffer,int& len);
	char* snap;
};

#endif /* MONITORREPLY_H_ */
