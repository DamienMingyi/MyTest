/*
 * STEPProtocol.h
 *
 *  Created on: Sep 7, 2016
 *      Author: level2
 */

#ifndef STEPPROTOCOL_H_
#define STEPPROTOCOL_H_

#include "IProtocol.h"

class STEPProtocol: public IProtocol {
public:
	STEPProtocol(char* parameter);
	virtual ~STEPProtocol();

	virtual int getTotalLength(char* buf);

	virtual int getLogOn(char* buf);
	virtual int getHeadLength()
	{
		return STEP_HEAD_LENGTH;
	}
	virtual bool handleProtocol(char* buf, Message* msg);
protected:
	virtual bool decode(char* buf, Message* msg);
protected:
	char* parameter;
private:
	int bodyPos;
	static const int STEP_HEAD_LENGTH = 24;
	char msgHead[STEP_HEAD_LENGTH];
	char tag35[8];

};

#endif /* STEPPROTOCOL_H_ */
