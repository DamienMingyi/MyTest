/*
 * OptionListReply.h
 *
 *  Created on: Mar 17, 2014
 *      Author: level2
 */

#ifndef OPTIONLISTREPLY_H_
#define OPTIONLISTREPLY_H_

#include "../../public/reply/ListReply.h"
class OptionListReply: public ListReply {
public:
	OptionListReply();
	virtual ~OptionListReply();

	virtual int request(const char* script,const char* query,char* buffer,int& len);
protected:
	bool getSelfList();
	bool getTypeList();
	bool getOrder(ListCmp& cmp);
	int  getSnapString(char* buffer,const char* select);
};

#endif /* OPTIONLISTREPLY_H_ */
