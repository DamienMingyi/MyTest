/*
 * IInitial.h
 *
 *  Created on: Sep 29, 2016
 *      Author: level2
 */

#ifndef IINITIAL_H_
#define IINITIAL_H_

#include "../../util/atof/Parse.h"
#include "../../util/convert/Convertion.h"
#include <time.h>
#include <boost/tokenizer.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include "../../util/shm/CShmEnv.h"
#include <sys/stat.h>
#include <stdio.h>
#include <vector>
#include "../market/Market.h"

class IInitial {
public:
	IInitial();
	virtual ~IInitial();
	int32_t getFileBufferLength(const char* fileName);
	virtual bool initial(const char* referencePath, const char* shm) = 0;
protected:
	Parse ps;
	Convertion cv;
	int32_t date;
	int32_t initime;
};

#endif /* IINITIAL_H_ */
