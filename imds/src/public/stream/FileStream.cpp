/*
 * FileStream.cpp
 *
 *  Created on: Sep 7, 2016
 *      Author: level2
 */

#include "FileStream.h"

FileStream::FileStream(const char* fileName): fp(NULL) {
	// TODO Auto-generated constructor stub
	strncpy(this->fileName, fileName, sizeof(this->fileName));
}

FileStream::~FileStream() {
	// TODO Auto-generated destructor stub
	closeStream();
}

bool FileStream::openStream()
{
	fp = fopen(this->fileName, "rb");
	if (fp != NULL)
	{
		INFO("open %s successful!", this->fileName);
		return true;
	}
	ERROR("open %s failed!", this->fileName);
	return false;
}

int32_t FileStream::readStream(char* buf, int32_t length)
{
	if (fp != NULL)
	{
		if (length == fread(buf, 1, length, fp))
			return length;
	}
	return 0;
}

int32_t FileStream::writeStream(char* buf, int32_t length)
{
	return 1;
}

void FileStream::closeStream()
{
	if (fp != NULL)
	{
		fclose(fp);
		fp = NULL;
	}
}
