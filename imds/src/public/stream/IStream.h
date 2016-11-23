/*
 * IStream.h
 *
 *  Created on: Sep 7, 2016
 *      Author: level2
 */

#ifndef ISTREAM_H_
#define ISTREAM_H_

#include "../program/Logger.h"
#include <stdint.h>
#include <cstring>

enum StreamType
{
	file = 0,
	tcp = 1,
	udp = 2
};

class IStream {
public:
	IStream();
	virtual ~IStream();

	virtual bool openStream() = 0;
	virtual int32_t readStream(char* buf, int32_t length) = 0;
	virtual int32_t writeStream(char* buf, int32_t length) = 0;
	virtual void closeStream() = 0;
	virtual StreamType getType() = 0;
};

#endif /* ISTREAM_H_ */
