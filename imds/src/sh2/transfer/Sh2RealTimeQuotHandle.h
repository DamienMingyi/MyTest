/*
 * Sh2RealTimeQuotHandle.h
 *
 *  Created on: Nov 8, 2016
 *      Author: level2
 */

#ifndef SH2REALTIMEQUOTHANDLE_H_
#define SH2REALTIMEQUOTHANDLE_H_

#include "../quothandle/Sh2QuotHandle.h"
#include "../../public/market/Market.h"

class Sh2RealTimeQuotHandle: public Sh2QuotHandle {
public:
	Sh2RealTimeQuotHandle();
	virtual ~Sh2RealTimeQuotHandle();

	virtual bool load(const char* shm_name);
protected:
	virtual void update(char* data, QuotType qt);
private:
	Market* market;
	CShmEnv shmEnv;
	StockSnap2 snap;
};

#endif /* SH2REALTIMEQUOTHANDLE_H_ */
