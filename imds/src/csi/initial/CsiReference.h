/*
 * CSIPReference.h
 *
 *  Created on: Sep 1, 2015
 *      Author: level2
 */

#ifndef CSIPREFERENCE_H_
#define CSIPREFERENCE_H_

#include "../../public/market/Market.h"
#include "../../public/initial/IInitial.h"
#include "../../public/indexpress/IndexPressSnap.h"

class CsiReference:public IInitial {
public:
	CsiReference();
	virtual ~CsiReference();
	bool initial(const char* referencePath, const char* shm);
private:
	bool CsiStaticParse(char* path);
private:
	vector<INDEXSNAP> vIndex;
	int32_t recordNum;
};

#endif /* CSIPREFERENCE_H_ */
