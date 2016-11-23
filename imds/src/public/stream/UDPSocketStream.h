/*
 * UDPSocketStream.h
 *
 *  Created on: Sep 8, 2016
 *      Author: level2
 */

#ifndef UDPSOCKETSTREAM_H_
#define UDPSOCKETSTREAM_H_

#include "IStream.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

const int bufLen = 64 * 1024;

class UDPSocketStream: public IStream {
public:
	UDPSocketStream(char* ip, int port, char* localIP);
	virtual ~UDPSocketStream();

	virtual bool openStream();
	virtual int32_t readStream(char* buf, int32_t length);
	virtual int32_t writeStream(char* buf, int32_t length);
	virtual void closeStream();
	virtual StreamType getType()
	{
		return udp;
	}
private:
	char ip[32];
	int port;
	int udpSocket;
	char localIP[32];
	struct ip_mreq ipMreq;
};

#endif /* UDPSOCKETSTREAM_H_ */
