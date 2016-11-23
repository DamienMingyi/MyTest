/*
 * FileStream.h
 *
 *  Created on: Sep 7, 2016
 *      Author: level2
 */

#ifndef FILESTREAM_H_
#define FILESTREAM_H_

#include "IStream.h"
#include <stdio.h>

class FileStream: public IStream {
public:
	FileStream(const char* fileName);
	virtual ~FileStream();

	virtual bool openStream();
	virtual int32_t readStream(char* buf, int32_t length);
	virtual int32_t writeStream(char* buf, int32_t length);
	virtual void closeStream();
	virtual StreamType getType()
	{
		return file;
	}
private:
	char fileName[256];
	FILE* fp;
};

#endif /* FILESTREAM_H_ */
