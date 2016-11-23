/*
 * IMessageReader.h
 *
 *  Created on: Sep 7, 2016
 *      Author: level2
 */

#ifndef MESSAGEREADER_H_
#define MESSAGEREADER_H_

#include "../stream/IStream.h"
#include "../protocol/IProtocol.h"


const int TOTAL_BUFFER_LENGTH = 4 * 1024 * 1024;

class MessageReader {
public:
	MessageReader();
	virtual ~MessageReader();

	bool read(IProtocol* protocol, IStream* stream);
	Message* getMessage()
	{
		return msg;
	}
	void close();
private:
	bool readStream(IProtocol* protocol, IStream* stream);
	bool readUDPStream(IProtocol* protocol, IStream* stream);
	bool streamIsOpen(IStream* stream);
private:
	char* buffer;
	int bufferLength;
	bool isOpen;
	bool isLogOn;
	Message* msg;
//	vecDM vectorDecoderMessage;

};

#endif /* MESSAGEREADER_H_ */
