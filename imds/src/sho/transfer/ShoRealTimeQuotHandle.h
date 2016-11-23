/*
 * ShoRealTimeQuotHandle.h
 *
 *  Created on: Oct 24, 2016
 *      Author: level2
 */

#ifndef SHOREALTIMEQUOTHANDLE_H_
#define SHOREALTIMEQUOTHANDLE_H_

#include "../quothandle/ShoQuotHandle.h"
#include "../../public/market/Market.h"

class ShoRealTimeQuotHandle: public ShoQuotHandle {
public:
	ShoRealTimeQuotHandle();
	virtual ~ShoRealTimeQuotHandle();
	virtual bool load(const char* shm_name);
protected:
	virtual void update(char* data, QuotType qt);
private:
	Market* market;
	CShmEnv shmEnv;
	OptionSnap snap;
};

#endif /* SHOREALTIMEQUOTHANDLE_H_ */
