/*
 * OptionReference.h
 *
 *  Created on: Sep 14, 2016
 *      Author: level2
 */

#ifndef OPTIONREFERENCE_H_
#define OPTIONREFERENCE_H_

#include <vector>
#include "../../public/option/OptionSnap.h"
#include "../../public/initial/IInitial.h"
#include "../../public/program/Profile.h"
#include "../../public/program/Logger.h"

typedef std::vector<SHOSNAP> vOptionSnap;

class OptionReference: public IInitial {
public:
	OptionReference();
	virtual ~OptionReference();

	bool initial(const char* referencePath, const char* shm);
private:
	bool getInitialSnap(const char* reff03Path);
	int32_t RemainingDay(int32_t begin, int32_t end);
	bool getRate(const char* ratePath);
private:
	vOptionSnap voptSnap;
	double rate;
};

#endif /* OPTIONREFERENCE_H_ */
