/*
 * CroSnap.h
 *
 *  Created on: 2016年10月25日
 *      Author: mds
 */

#include "../market/ISnap.h"

struct CROSNAP: public SNAP
{
	LEVEL bid[5];
	LEVEL offer[5];
	int64_t hand;							/* 现手 */
	int64_t in;								/* 内盘 */
	int64_t out;							/* 外盘 */
};
