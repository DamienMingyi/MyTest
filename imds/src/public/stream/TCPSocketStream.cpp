/*
 * TCPSocketStream.cpp
 *
 *  Created on: Sep 8, 2016
 *      Author: level2
 */

#include "TCPSocketStream.h"
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>

TCPSocketStream::TCPSocketStream(char* ip, int port): port(port), tcpSocket(0) {
	// TODO Auto-generated constructor stub
	strncpy(this->ip, ip, sizeof(this->ip));
}

TCPSocketStream::~TCPSocketStream() {
	// TODO Auto-generated destructor stub
	closeStream();
}

bool TCPSocketStream::openStream()
{
	if((this->tcpSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		ERROR("socket error, error code is %d!", errno);
		return false;
	}
	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(this->port);
	serverAddr.sin_addr.s_addr = inet_addr(this->ip);
	if(connect(this->tcpSocket, (sockaddr*)&serverAddr, sizeof(sockaddr)) < 0)
	{
		ERROR("connect %s:%d error, error code is %d!", ip, port, errno);
		return false;
	}
	INFO("connect %s: %d successful!", this->ip, this->port);
	return true;
}

int32_t TCPSocketStream::readStream(char* buf, int32_t length)
{
	if (this->tcpSocket == 0)
		return 0;
	int leftByte = length;
	int readByte = 0;
	char* position = buf;

	while (leftByte > 0)
	{
		readByte = recv(this->tcpSocket, position, leftByte, 0);
		if (readByte < 0)
		{
			ERROR("receiving message from %s: %d error, error code is %d!", this->ip, this->port, errno);
			return readByte;
		}
		else if (readByte == 0)
		{
			ERROR("no message is available from %s: %d!", this->ip, this->port);
			return readByte;
		}
		leftByte -= readByte;
		position += readByte;
	}
	return length;
}

int32_t TCPSocketStream::writeStream(char* buf, int32_t length)
{
	if (this->tcpSocket == 0)
		return 0;
	int leftByte = length;
	int writeByte = 0;
	char* position = buf;

	while (leftByte > 0)
	{
		writeByte = send(this->tcpSocket, position, leftByte, 0);
		if (writeByte < 0)
		{
			ERROR("sending message to %s: %d error, error code is %d!", this->ip, this->port, errno);
			return writeByte;
		}
		leftByte -= writeByte;
		position += writeByte;
	}
	return length;
}

void TCPSocketStream::closeStream()
{
	close(tcpSocket);
}
