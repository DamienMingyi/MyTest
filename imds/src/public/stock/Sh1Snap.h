/*
 * Sh1Snap.h
 *
 *  Created on: Oct 17, 2016
 *      Author: level2
 */

#ifndef SH1SNAP_H_
#define SH1SNAP_H_

#include "../market/ISnap.h"

typedef LINE STOCKLINE;
typedef TRADE STOCKTRADE;

struct SGZLX
{
	char gzdm[7];
	char interestDate[8];
	char interestPerHundred[15];
	char daysOfInterest[6];
	char couponRate[8];
};

struct CPXX
{
	char basicSecurityCode[8];
	char marketType[8];       				/* 市场种类 */
	char type[6];							/* 证券类别 */
	char subtype[6];
	char currency[6];
	int64_t parValue;               		/* 面值 */
	int32_t lastTradingDate;        		/* 最后交易日期 */
	int64_t buyNum;
	int64_t sellNum;
	int64_t applyUpimit;
	int64_t applyDownlimit;
	int64_t priceLevel;
	int32_t listingDate;            		/* 上市日期 */
	int32_t XRRate;                 		/* 除权比例 */
	int32_t XDAmount;               		/* 除息金额 */
	char financialTag[8];           		/* 融资标的标志 */
	char securityLendingTag[8];     		/* 融券标的标志 */
	char productStatus[24];         		/* 产品状态标志 */
	char upDownLimitType[8];        		/* 涨跌幅限制类型 */
	int64_t upLimitPx;              		/* 涨幅上限价格 */
	int64_t downLimitPx;            		/* 跌幅下限价格 */
};

struct GZLX
{
	int32_t interestDate;					/* 计息日期 */
	int64_t interestPerHundred;				/* 每百元应计利息额 */
	int32_t daysOfInterest;					/* 利息天数 */
	int64_t couponRate;						/* 票面利率 */
};

struct FJY
{
	char nonTradeType[8];					/* 非交易业务类型 */
	int32_t nonTradeOrderBeginDate;       	/* 非交易订单输入开始日期 */
	int32_t nonTradeOrderEndDate;         	/* 非交易订单输入结束日期 */
	int64_t nonTradePx;                   	/* 非交易价格 */
	int64_t ipoQuantum;                   	/* IPO总量 */
	char ipoDistributeMethod[2];             	/* IPO分配方法 */
	int64_t ipoDownLimitPx;               	/* IPO申购价格区间下限 */
	int64_t ipoUpLimitPx;                 	/* IPO申购价格区间上限 */
	int32_t registerDate;                 	/* 配股股权登记日 */
	int32_t DRDate;                       	/* 配股除权日 */
	int64_t rightIssueRate;               	/* 配股比例 */
	int64_t rightIssueQuantum;            	/* 配股总量 */
	int64_t t2iopv;                       	/* T-2日基金收益/基金净值 */
	int64_t t1iopv;                       	/* T-1日基金收益/基金净值 */
	char offeringMethod[8];               	/* 发行方式 */
};

struct DBP
{
	int64_t finance;						/* 融资余额 */
	int64_t securityLending;				/* 融券余量 */
};


struct STOCKSNAP: public SNAP
{
	LEVEL bid[5];
	LEVEL offer[5];
	BASIC_INFO basicinfo;
	CAPITAL_DATA capitaldata;
	FINANCIAL_DIGEST financialDigest;
	FINANCIAL_INDEX financialIndex;
	WEIGHT_DATA weightedData;
	int64_t numTrades;
	int64_t iopv;
	int64_t nav;							/* nav */
	int64_t hand;							/* 现手 */
	int64_t in;								/* 内盘 */
	int64_t out;							/* 外盘 */
	int32_t pin;							/* 成交方向 */
	CPXX cpxx;
	GZLX gzlx;
	FJY fjy;
	DBP dbp;
	char tradingPhaseCode[8];
	static char tradingSessionID[8];
	STOCKLINE line;
	STOCKTRADE trade;
};


#endif /* SH1SNAP_H_ */
