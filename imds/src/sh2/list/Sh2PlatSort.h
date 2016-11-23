/*
 * Sh2PlatSort.h
 *
 *  Created on: 2016年10月21日
 *      Author: mds
 */

#ifndef SRC_SH2_LIST_SH2PLATSORT_H_
#define SRC_SH2_LIST_SH2PLATSORT_H_
#include "../../public/program/Logger.h"
#include "../../public/stock/StockSnap.h"
#include "../../public/market/Market.h"
#include "../../public/market/ISnapShot.h"
#include "../../util/shm/CShmEnv.h"
#include "../../public/stock2/StockSnap2.h"
#include <cstring>

class Sh2PlatSort {
public:
	Sh2PlatSort();
	virtual ~Sh2PlatSort();
	bool load(char* shm_name);
	void update();
private:
		Market* market;
		CShmEnv shmEnv;
		StockSnap2 snap;
};

#endif /* SRC_SH2_LIST_SH2PLATSORT_H_ */
