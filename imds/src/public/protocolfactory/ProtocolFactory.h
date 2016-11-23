/*
 * ProtocolFactory.h
 *
 *  Created on: Sep 19, 2016
 *      Author: level2
 */

#ifndef PROTOCOLFACTORY_H_
#define PROTOCOLFACTORY_H_

#include "../protocol/EzEIProtocol.h"
#include "../protocol/FASTProtocol.h"
#include "../protocol/LDDSProtocol.h"
#include "../protocol/SZV5Protocol.h"
#include <string>
#include <boost/tokenizer.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <map>

struct PROTOCOL_CONFIG
{
	int mode;
	char* securityType;
	char* msgType;			//step message
	char* templateName;
	char* senderCompID;
	char* targetCompID;
	int heartBtInt;
	char* password;
	char* defaultApplVerID;
	char* upstream;
};

class ProtocolFactory {
public:
	ProtocolFactory();
	virtual ~ProtocolFactory();

	IProtocol* createProtocol(PROTOCOL_CONFIG* config);

private:
	IProtocol* protocol;
	std::map<std::string, IProtocol*> ph;

};

#endif /* PROTOCOLFACTORY_H_ */
