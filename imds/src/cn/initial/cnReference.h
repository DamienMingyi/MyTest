/*
 * crossReference.h
 *
 *  Created on: 2016年10月21日
 *      Author: mds
 */

#ifndef SRC_CORSSMARKET_INITIAL_CROSSREFERENCE_H_
#define SRC_CORSSMARKET_INITIAL_CROSSREFERENCE_H_

#include "../../public/market/Market.h"
#include "../../public/initial/IInitial.h"
#include "../../public/market/ISnap.h"
#include <vector>
#include "../../public/cn/CrossSnap.h"

class cnReference: public IInitial {
public:
	cnReference();
	virtual ~cnReference();
	bool initial(const char* referencePath, const char* shm);
private:
	bool plateFileParse(char *path, const char flag);
private:
	vector<CROSNAP> vCrossSotck;
};

#endif /* SRC_CORSSMARKET_INITIAL_CROSSREFERENCE_H_ */
