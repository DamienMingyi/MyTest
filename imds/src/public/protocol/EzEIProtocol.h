/*
 * EzEIProtocol.h
 *
 *  Created on: Sep 8, 2016
 *      Author: level2
 */

#ifndef EZEIPROTOCOL_H_
#define EZEIPROTOCOL_H_

#include "STEPProtocol.h"

class EzEIProtocol: public STEPProtocol {
public:
	EzEIProtocol(char* paramter, int mode);
	virtual ~EzEIProtocol();

	virtual int getTotalLength(char* buf);
	virtual int getHeadLength()
	{
		return STEP_HEAD_LENGTH;
	}
	virtual bool handleProtocol(char* buf, Message* msg);
protected:
	virtual bool decode(char* buf, Message* msg);
private:
	int bodyPos;
	static const int STEP_HEAD_LENGTH = 22;
	char msgHead[STEP_HEAD_LENGTH];
	int mode;
	FastCodecs::DecoderMessage decoderMessage;
};

#endif /* EZEIPROTOCOL_H_ */
