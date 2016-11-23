/*
 * CSIRealTimeQuotHandle.h
 *
 *  Created on: Nov 15, 2016
 *      Author: level2
 */

#ifndef CSIREALTIMEQUOTHANDLE_H_
#define CSIREALTIMEQUOTHANDLE_H_

#include "../quothandle/CSIQuotHandle.h"
#include "../../public/market/Market.h"

class CSIRealTimeQuotHandle: public CSIQuotHandle {
public:
	CSIRealTimeQuotHandle();
	virtual ~CSIRealTimeQuotHandle();

	virtual bool load(const char* shm_name);
protected:
	virtual void update(char* data, QuotType qt);
private:
	Market* market;
	CShmEnv shmEnv;
	IndexPressSnap snap;
};

#endif /* CSIREALTIMEQUOTHANDLE_H_ */
