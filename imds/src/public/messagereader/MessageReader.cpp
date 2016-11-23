/*
 * IMessageReader.cpp
 *
 *  Created on: Sep 7, 2016
 *      Author: level2
 */

#include "MessageReader.h"

MessageReader::MessageReader():buffer(new char[TOTAL_BUFFER_LENGTH]), bufferLength(0), isOpen(false),
	isLogOn(false), msg(new Message()) {
	// TODO Auto-generated constructor stub
}

MessageReader::~MessageReader() {
	// TODO Auto-generated destructor stub
	delete[] buffer;
	delete msg;
}

bool MessageReader::read(IProtocol* protocol, IStream* stream)
{
	if (stream->getType() == tcp || stream->getType() == file)
		return readStream(protocol, stream);
	else if (stream->getType() == udp)
		return readUDPStream(protocol, stream);
	return false;
}

bool MessageReader::streamIsOpen(IStream* stream)
{
	if (!isOpen)
		isOpen = stream->openStream();
	return isOpen;
}

bool MessageReader::readStream(IProtocol* protocol, IStream* stream)
{
	if (!streamIsOpen(stream))
		return false;

	if (!isLogOn)
	{
		char logOn[1024];
		memset(logOn, 0, 1024);
		int logOnLength = protocol->getLogOn(logOn);
		//ldds
		if (stream->writeStream(logOn, logOnLength) <= 0)
			return isLogOn;
		isLogOn = true;
	}

	memset(buffer, 0, protocol->getHeadLength());
	if (stream->readStream(buffer, protocol->getHeadLength()) == protocol->getHeadLength())
	{
		bufferLength = protocol->getTotalLength(buffer);
		if (bufferLength != 0)
		{
			if (stream->readStream(buffer + protocol->getHeadLength(), bufferLength - protocol->getHeadLength())
					== bufferLength - protocol->getHeadLength())
			{
				msg->clear();
				return protocol->handleProtocol(buffer, msg);
			}
		}
	}
	return false;
}

bool MessageReader::readUDPStream(IProtocol* protocol, IStream* stream)
{
	if (!streamIsOpen(stream))
		return false;
	bufferLength = stream->readStream(buffer, bufferLength);
	if (bufferLength <= 0)
		return false;
	protocol->setTotalLength(bufferLength);
	msg->clear();
	protocol->handleProtocol(buffer, msg);
	return true;
}

void MessageReader::close()
{
	isOpen = false;
	isLogOn = false;
}
