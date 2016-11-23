/*
 * TCPSocketStream.h
 *
 *  Created on: Sep 8, 2016
 *      Author: level2
 */

#ifndef TCPSOCKETSTREAM_H_
#define TCPSOCKETSTREAM_H_

#include "IStream.h"

class TCPSocketStream: public IStream {
public:
	TCPSocketStream(char* ip, int port);
	virtual ~TCPSocketStream();

	virtual bool openStream();
	virtual int32_t readStream(char* buf, int32_t length);
	virtual int32_t writeStream(char* buf, int32_t length);
	virtual void closeStream();
	virtual StreamType getType()
	{
		return tcp;
	}
private:
	char ip[32];
	int port;
	int tcpSocket;
};

#endif /* TCPSOCKETSTREAM_H_ */
