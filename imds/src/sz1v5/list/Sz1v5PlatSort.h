/*
 * Sz1v5PlatSort.h
 *
 *  Created on: 2016年10月21日
 *      Author: mds
 */

#ifndef SRC_SZ1V5_LIST_SZ1V5PLATSORT_H_
#define SRC_SZ1V5_LIST_SZ1V5PLATSORT_H_

#include "../../public/program/Logger.h"
#include "../../public/market/Market.h"
#include "../../public/market/ISnapShot.h"
#include "../../util/shm/CShmEnv.h"
#include "../../public/szstock/SZSnap.h"
#include <cstring>

class Sz1v5PlatSort {
public:
	Sz1v5PlatSort();
	virtual ~Sz1v5PlatSort();
	bool load(char* shm_name);
	void update();
private:
		Market* market;
		CShmEnv shmEnv;
		SZSnap snap;
};

#endif /* SRC_SZ1V5_LIST_SZ1V5PLATSORT_H_ */
