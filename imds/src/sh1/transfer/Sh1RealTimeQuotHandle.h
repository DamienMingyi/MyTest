/*
 * Sh1RealTimeQuotHandle.h
 *
 *  Created on: Oct 25, 2016
 *      Author: level2
 */

#ifndef SH1REALTIMEQUOTHANDLE_H_
#define SH1REALTIMEQUOTHANDLE_H_

#include "../quothandle/Sh1QuotHandle.h"
#include "../../public/market/Market.h"

class Sh1RealTimeQuotHandle: public Sh1QuotHandle {
public:
	Sh1RealTimeQuotHandle();
	virtual ~Sh1RealTimeQuotHandle();
	virtual bool load(const char* shm_name);
protected:
	virtual void update(char* data, QuotType qt);
private:
	Market* market;
	CShmEnv shmEnv;
	StockSnap snap;

};

#endif /* SH1REALTIMEQUOTHANDLE_H_ */
