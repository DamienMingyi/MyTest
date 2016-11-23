/*
 * FixedIncomeReference.h
 *
 *  Created on: Sep 29, 2016
 *      Author: level2
 */

#ifndef FIXEDINCOMEREFERENCE_H_
#define FIXEDINCOMEREFERENCE_H_

#include "../../public/handle/IInitial.h"

typedef std::vector<SHFISNAP> vFixedIncomeSnap;

class FixedIncomeReference: public IInitial {
public:
	FixedIncomeReference();
	virtual ~FixedIncomeReference();

	bool initial(const char* referencePath, const char* shm);
private:
	bool getInitialSnap(const char* zqxxPath);
private:
	vFixedIncomeSnap vfiSnap;
};

#endif /* FIXEDINCOMEREFERENCE_H_ */
