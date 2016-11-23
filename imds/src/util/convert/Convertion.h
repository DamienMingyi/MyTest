/*
 * Convertion.h
 *
 *  Created on: Sep 25, 2016
 *      Author: level2
 */

#ifndef CONVERTION_H_
#define CONVERTION_H_

#include <iconv.h>
#include "../../public/program/Logger.h"

class Convertion {
public:
	Convertion(const char* origFormat, const char* convFormat);
	virtual ~Convertion();

	bool convert(char* origBufffer, size_t origLength, char* convBuffer, size_t convLength);

private:
	char fromcode[8];
	char tocode[8];
};

#endif /* CONVERTION_H_ */
