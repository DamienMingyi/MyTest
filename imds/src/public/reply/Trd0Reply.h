/*
 * TrdReply.h
 *
 *  Created on: Jun 13, 2014
 *      Author: level2
 */

#ifndef TRDREPLY_H_
#define TRDREPLY_H_

#include "HttpReply.h"

class Trd0Reply: public HttpReply {
public:
	Trd0Reply();
	virtual ~Trd0Reply();

protected:
	virtual bool  parse(const char* script,const char* query);
	virtual int  getTradeString(char* buffer,char* trd)=0;

	int reply(char* buffer,int& len);
	vector<char*> trade0;///<Öð±Ê³É½»
};

#endif /* TRDREPLY_H_ */
