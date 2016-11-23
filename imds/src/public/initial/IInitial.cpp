/*
 * IInitial.cpp
 *
 *  Created on: Sep 29, 2016
 *      Author: level2
 */

#include "IInitial.h"

IInitial::IInitial(): cv("UTF-8", "GB2312"), date(0), initime(0) {
	// TODO Auto-generated constructor stub

}

IInitial::~IInitial() {
	// TODO Auto-generated destructor stub
}

int32_t IInitial::getFileBufferLength(const char* fileName)
{
	struct stat buf;
	int statValue = stat(fileName, &buf);
	if (statValue != -1)
		return buf.st_size;
	else
		return -1;
}
