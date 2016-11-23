/*
 * Convertion.cpp
 *
 *  Created on: Sep 25, 2016
 *      Author: level2
 */

#include "Convertion.h"
#include <cstring>

Convertion::Convertion(const char* origFormat, const char* convFormat) {
	// TODO Auto-generated constructor stub
	strncpy(this->fromcode, origFormat, sizeof(this->fromcode));
	strncpy(this->tocode, convFormat, sizeof(this->tocode));
}

Convertion::~Convertion() {
	// TODO Auto-generated destructor stub
}

bool Convertion::convert(char* origBufffer, size_t origLength, char* convBuffer, size_t convLength)
{
	iconv_t cd = iconv_open(this->tocode, this->fromcode);
	if(cd == iconv_t(-1))
	{
		ERROR("open conversion error!");
		iconv_close(cd);
		return false;
	}
	char **in = &origBufffer;
	char **out = &convBuffer;
	iconv(cd, in, &origLength, out, &convLength);
	iconv_close(cd);
	return true;
}

