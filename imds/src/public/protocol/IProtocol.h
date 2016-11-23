/*
 * IProtocol.h
 *
 *  Created on: Sep 7, 2016
 *      Author: level2
 */

#ifndef IPROTOCOL_H_
#define IPROTOCOL_H_

#include <stdio.h>
#include <cstring>
#include "../program/Logger.h"

#include <time.h>
#include <stdlib.h>
#include <string>
//#include "../fast/FastCodecs.h"
//#include <vector>
//
//typedef std::vector<FastCodecs::DecoderMessage> vecDM;
#include "../messagereader/Message.h"

class IProtocol {
public:
	IProtocol();
	virtual ~IProtocol();


	virtual int getTotalLength(char* buf)
	{
		return 0;
	}
	virtual int getHeadLength()
	{
		return 0;
	}
	virtual int getLogOn(char* buf)
	{
		return 0;
	}
	virtual bool handleProtocol(char* buf, Message* msg) = 0;
	virtual void setSuccessor(IProtocol* protocol)
	{
		this->successorProtocol = protocol;
	}
	void setTotalLength(int len)
	{
		length = len;
	}
protected:
	virtual bool decode(char* buf, Message* msg) = 0;

protected:
	int headLength;
	int length;
	int bodyLength;
	IProtocol* successorProtocol;
};

#endif /* IPROTOCOL_H_ */
