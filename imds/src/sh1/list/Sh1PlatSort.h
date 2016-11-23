/*
 * Sh1PlatSort.h
 *
 *  Created on: 2016年10月19日
 *      Author: mds
 */

#ifndef SRC_SH1_LIST_SH1PLATSORT_H_
#define SRC_SH1_LIST_SH1PLATSORT_H_

#include "../../public/program/Logger.h"
#include "../../public/stock/StockSnap.h"
#include "../../public/market/Market.h"
#include "../../public/market/ISnapShot.h"
#include "../../util/shm/CShmEnv.h"
#include <map>
#include <cstring>

class Sh1PlatSort {
public:
	Sh1PlatSort();
	virtual ~Sh1PlatSort();
	bool load(char* shm_name);
	void update();
private:
	Market* market;
	CShmEnv shmEnv;
	StockSnap snap;
};

#endif /* SRC_SH1_LIST_SH1PLATSORT_H_ */
