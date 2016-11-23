/*
 * OptionStatusReply.h
 *
 *  Created on: Nov 4, 2015
 *      Author: level2
 */

#ifndef OPTIONSTATUSREPLY_H_
#define OPTIONSTATUSREPLY_H_

#include "../../public/reply/StatusReply.h"

class OptionStatusReply: public StatusReply {
public:
	OptionStatusReply();
	virtual ~OptionStatusReply();
	virtual int request(const char* script,const char* query,char* buffer,int& len);
protected:
	virtual int getSnapString(char* buffer,const char* select);
};

#endif /* OPTIONSTATUSREPLY_H_ */
