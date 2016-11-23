/*
 * CrossPlatSort.h
 *
 *  Created on: 2016年10月24日
 *      Author: mds
 */

#ifndef SRC_CN_LIST_CNPLATSORT_H_
#define SRC_CN_LIST_CNPLATSORT_H_

#include "../../public/program/Logger.h"
#include "../../public/stock/StockSnap.h"
#include "../../public/market/Market.h"
#include "../../public/market/ISnapShot.h"
#include "../../util/shm/CShmEnv.h"
#include "../../public/szstock/SZSnap.h"
#include <cstring>
#include "../../public/cn/CrossSnap.h"

class cnPlatSort {
public:
	cnPlatSort();
	virtual ~cnPlatSort();
	bool load(const char *cross_shm_path, const char *sh1_shm_path, const char *sz1_shm_path);
	void update();
private:
	Market *sh1market;
	Market *sz1market;
	Market *cnmarket;

	CShmEnv cnShmEnv;
	CShmEnv sh1ShmEnv;
	CShmEnv sz1ShmEnv;

	StockSnap sh1snap;
	SZSnap sz1snap;
	CrossSnap cnsnap;
};

#endif /* SRC_CN_LIST_CNPLATSORT_H_ */
