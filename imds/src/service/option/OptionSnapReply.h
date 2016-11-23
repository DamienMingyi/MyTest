/*
 * OptionSnapReply.h
 *
 *  Created on: Mar 17, 2014
 *      Author: level2
 */

#ifndef OPTIONSNAPREPLY_H_
#define OPTIONSNAPREPLY_H_

#include "../../public/reply/SnapReply.h"
class OptionSnapReply: public SnapReply {
public:
	OptionSnapReply();
	virtual ~OptionSnapReply();
	virtual int request(const char* script,const char* query,char* buffer,int& len);
protected:
	virtual int getSnapString(char* buffer,const char* select);
};

#endif /* OPTIONSNAPREPLY_H_ */
