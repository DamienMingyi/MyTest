/*
 * IndexSnap.h
 *
 *  Created on: 2016年10月18日
 *      Author: mds
 */

#ifndef SRC_PUBLIC_INDEXPRESS_INDEXSNAP_H_
#define SRC_PUBLIC_INDEXPRESS_INDEXSNAP_H_
#include <stdint.h>
#include "../market/ISnap.h"
typedef LINE INDEXLINE;
typedef TRADE INDEXTRADE;

struct INDEXSNAP: public SNAP
{
	char market[4];					/*市场*/
	int64_t exchangeRate;			/*汇率*/
	int64_t indexSeq;				/*指数展示序号*/
	int64_t lastIndex2;				/*当日收盘值2*/
	int64_t lastIndex3;				/*当日收盘值3*/
	char currency[4];
	int64_t hand;
	INDEXTRADE trade;
	INDEXLINE line;
};


#endif /* SRC_PUBLIC_INDEXPRESS_INDEXSNAP_H_ */
