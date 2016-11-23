/*
 * Sz1v5RealTimeQuotHandle.h
 *
 *  Created on: Oct 26, 2016
 *      Author: level2
 */

#ifndef SZ1V5REALTIMEQUOTHANDLE_H_
#define SZ1V5REALTIMEQUOTHANDLE_H_

#include "../quothandle/Sz1v5QuotHandle.h"
#include "../../public/market/Market.h"

class Sz1v5RealTimeQuotHandle: public Sz1v5QuotHandle {
public:
	Sz1v5RealTimeQuotHandle();
	virtual ~Sz1v5RealTimeQuotHandle();

	virtual bool load(const char* shm_name);
protected:
	virtual void update(char* data, QuotType qt);
private:
	Market* market;
	CShmEnv shmEnv;
	SZSnap snap;
};

#endif /* SZ1V5REALTIMEQUOTHANDLE_H_ */
