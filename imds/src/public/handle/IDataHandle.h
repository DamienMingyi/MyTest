/*
 * IDataHandle.h
 *
 *  Created on: Sep 20, 2016
 *      Author: level2
 */

#ifndef IDATAHANDLE_H_
#define IDATAHANDLE_H_

#include "../protocol/IProtocol.h"

enum QuotType
{
	shanghaiLevel1Snap = 0,
	shanghaiLevel2Snap,
	shanghaiLevel2Tick,
	shanghaiLevel2Auction,
	shanghaiLevel2Index,
	shanghaiOptionSnap,
	shanghaiFixedIncomeSnap,
	shanghaiFixedIncomeDetail,
	shanghaiFixedIncomeTick,
	chinaSecurityIndexSnap,
	hongkongConnectSnap,
	shenzhenLevel1Snap,
	shenzhenLevel1Index
};

typedef void (*callback_t)(char* data, QuotType qt);

class IDataHandle {
public:
	IDataHandle();
	virtual ~IDataHandle();

	virtual void handle(Message* msg) = 0;
//	static void callback(char* data, QuotType qt)
//	{
//		if (pt == NULL)
//			return ;
//		pt->update(data, qt);
//	}
	virtual bool load(const char* shm_name)
	{
		return true;
	}
protected:
	virtual void update(char* data, QuotType) = 0;
//	static IDataHandle* pt;
//	callback_t cb;
};

#endif /* IDATAHANDLE_H_ */
