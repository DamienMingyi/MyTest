/*
 * OptionLineReply.h
 *
 *  Created on: Mar 17, 2014
 *      Author: level2
 */

#ifndef OPTIONLINEREPLY_H_
#define OPTIONLINEREPLY_H_

#include "../../public/reply/LineReply.h"

class OptionLineReply: public LineReply {
public:
	OptionLineReply();
	virtual ~OptionLineReply();

	virtual int request(const char* script,const char* query,char* buffer,int& len);
protected:
	int  getLineString(char* buffer,const char* select);
};


#endif /* OPTIONLINEREPLY_H_ */
