/*
 * UDPSocketStream.cpp
 *
 *  Created on: Sep 8, 2016
 *      Author: level2
 */

#include "UDPSocketStream.h"
#include <errno.h>
#include <stdio.h>

UDPSocketStream::UDPSocketStream(char* ip, int port, char* localIP): port(port), udpSocket(0) {
	// TODO Auto-generated constructor stub
	strncpy(this->ip, ip, sizeof(this->ip));
	strncpy(this->localIP, localIP, sizeof(this->localIP));
}

UDPSocketStream::~UDPSocketStream() {
	// TODO Auto-generated destructor stub
	closeStream();
}

bool UDPSocketStream::openStream()
{
	this->udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (this->udpSocket == -1)
	{
		ERROR("create socket failed, error code is %d!", errno);
		return false;
	}
	struct sockaddr_in serveraddr;
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	serveraddr.sin_addr.s_addr = inet_addr(ip);

	int flag = 1;
	if (setsockopt(this->udpSocket, SOL_SOCKET, SO_RCVBUF, (const char*)&bufLen, sizeof(bufLen)) == -1)
	{
		ERROR("set receiving buffer failed, error code is %d!", errno);
		return false;
	}

	if (setsockopt(this->udpSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&flag, sizeof(flag)) == -1)
	{
		ERROR("set reusing address failed, error code is %d!", errno);
		return false;
	}

	if (bind(this->udpSocket, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
	{
		ERROR("bind group address failed, error code is %d!", errno);
		return false;
	}

	bzero(&ipMreq, sizeof(ipMreq));
	ipMreq.imr_multiaddr.s_addr = inet_addr(this->ip);
//	ipMreq.imr_interface.s_addr  = htonl(INADDR_ANY);
	ipMreq.imr_interface.s_addr  = inet_addr(this->localIP);
	if (setsockopt(this->udpSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char*)&ipMreq, sizeof(ipMreq)) == -1)
	{
		ERROR("add group address failed, error code is %d!", errno);
		return false;
	}
	INFO("create udp multicast socket %s: %d successful!", this->ip, this->port);
	return true;
}

int32_t UDPSocketStream::readStream(char* buf, int32_t length)
{
	int32_t size = recv(this->udpSocket, buf, bufLen, 0);
	if (size == 0)
		ERROR("no message is available from %s: %d!", this->ip, this->port);
	else if (size < 0)
		ERROR("receiving message from %s: %d error, error code is %d!", this->ip, this->port, errno);
	return size;
}

int32_t UDPSocketStream::writeStream(char* buf, int32_t length)
{
	return 0;
}

void UDPSocketStream::closeStream()
{
	setsockopt(udpSocket, IPPROTO_IP, IP_DROP_MEMBERSHIP, (const char*)&ipMreq, sizeof(ipMreq));
	close(udpSocket);
}
