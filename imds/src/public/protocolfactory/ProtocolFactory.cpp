/*
 * ProtocolFactory.cpp
 *
 *  Created on: Sep 19, 2016
 *      Author: level2
 */

#include "ProtocolFactory.h"

ProtocolFactory::ProtocolFactory(): protocol(NULL){
	// TODO Auto-generated constructor stub
}

ProtocolFactory::~ProtocolFactory() {
	// TODO Auto-generated destructor stub
	std::map<std::string, IProtocol*>::iterator it;
	for (it = ph.begin(); it != ph.end(); ++ it)
		delete it->second;
}

IProtocol* ProtocolFactory::createProtocol(PROTOCOL_CONFIG* config)
{
	std::string ups(config->upstream);
	int count = 0;
	for (int i = 0; i < ups.length(); ++ i)
	{
		if (ups.at(i) == '|')
		{
			++ count;
		}
	}
	std::string protocolString[count + 1];
	boost::char_separator<char> sep("|");
	int i = 0;
	boost::tokenizer<boost::char_separator<char> > token(ups, sep);
	for (boost::tokenizer<boost::char_separator<char> >::iterator const_it = token.begin();
			const_it != token.end(); ++ const_it)
	{
		protocolString[i ++] = *const_it;
		if (*const_it == "STEP")
			ph["STEP"] = new STEPProtocol(config->msgType);
		if (*const_it == "EzEI")
			ph["EzEI"] = new EzEIProtocol(config->securityType, config->mode);
		if (*const_it == "LDDS")
			ph["LDDS"] = new LDDSProtocol(config->msgType);
		if (*const_it == "FAST")
			ph["FAST"] = new FASTProtocol(config->templateName);
		if (*const_it == "SZV5")
			ph["SZV5"] = new SZV5Protocol(config->msgType, config->senderCompID, config->targetCompID,
					config->heartBtInt, config->password, config->defaultApplVerID);
	}
	if (count == 0)
		protocol = ph[protocolString[0]];
	for (int i = 0; i < count; ++ i)
	{
		if (i == 0)
			protocol = ph[protocolString[i]];
		ph[protocolString[i]]->setSuccessor(ph[protocolString[i + 1]]);
	}
	return protocol;
}
