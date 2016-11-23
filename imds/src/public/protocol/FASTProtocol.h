/*
 * FASTProtocol.h
 *
 *  Created on: Sep 9, 2016
 *      Author: level2
 */

#ifndef FASTPROTOCOL_H_
#define FASTPROTOCOL_H_

#include "IProtocol.h"
#include "../fast/FastCodecs.h"

class FASTProtocol: public IProtocol {
public:
	FASTProtocol(char* tpName);
	virtual ~FASTProtocol();

	virtual bool handleProtocol(char* buf, Message* msg);

protected:
	virtual bool decode(char* buf, Message* msg);
private:
//	bool checkSum(char* buf96, int buf96Length);
	bool loadTemplate(char* tpName);

private:
	FastCodecs::FastCodecs decoder;
	int iCount;
};

#endif /* FASTPROTOCOL_H_ */
