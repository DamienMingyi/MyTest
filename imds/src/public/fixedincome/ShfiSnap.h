/*
 * ShfiSnap.h
 *
 *  Created on: Sep 29, 2016
 *      Author: level2
 */

#ifndef SHFISNAP_H_
#define SHFISNAP_H_

#include "../market/ISnap.h"

typedef LINE SHFILINE;
typedef TRADE SHFITRADE;

struct TICK
{
	int32_t time;
	int64_t tradeNetPrice;
	int64_t accurateInterestAmt;
	int64_t tradeFullPrice;
	int64_t yield2Marturity;
	int64_t tradeQty;
	int64_t tradeMoney;
	char tradeMethod[8];
};

struct ORDER
{
	uint64_t orderNo;
	uint32_t auctionTime;
	char auctioneer[24];
	uint64_t orderQty;
	uint64_t yield2Maturity;
};

struct LEVEL
{
	int64_t netPx;
	int64_t volume;
	int64_t ordersNum;
	//order
	ORDER orderQueue[UPDATE_ORDER_NUMBER];
};


struct SHFISNAP: public SNAP
{
	int32_t tradeTime;
	int64_t hand;
	int64_t in;
	int64_t out;
	int64_t pin;

	//static
	char tradeProduct[8];
	char attribute[8];
	char status[8];
	char pledgeCode[8];
	int32_t openTime;
	int32_t closeTime;
	char offeringWay[8];
	int32_t denomination;
	int64_t offeringPx;
	char interestType[8];
	char payingFrequence[8];
	int32_t nomialInterstRate;
	int32_t basicInterestRate;
	int32_t basicInterestMargin;
	int32_t deadline;
	int32_t offeringVolume;
	int32_t offeringBeginDate;
	int32_t offeringEndDate;
	int32_t listingDate;
	int32_t expiringDate;
	char bondType[8];
	char offeringMethod[8];
	char transMarket[8];
	char sellShort[8];
	int32_t totalShortLimit;
	int32_t traderShortLimit;
	int32_t preWeightedAvgPx;

	//real time
	int32_t imageStatus;
	int32_t weightedAvgPx;
	int64_t totalNumTrades;
	int32_t preCloseYield;
	int32_t preWeightedAvgYield;
	int32_t openYield;
	int32_t highYield;
	int32_t lowYield;
	int32_t lastYield;
	int32_t weightedAvgYield;
	int32_t accruedInterestAmt;
	LEVEL bid[UPDATE_LEVEL_NUMBER];
	LEVEL offer[UPDATE_LEVEL_NUMBER];
	//tick
	int32_t tradeNetPrice;
	int32_t accurateInterestAmt;
	int32_t tradeFullPrice;
	int32_t yield2Marturity;
	int64_t tradeQty;
	int64_t tradeMoney;
	char tradeMethod[8];

	bool noPacketdrop;

	SHFILINE line;
	SHFITRADE trade;
	TICK tick;
};



#endif /* SHFISNAP_H_ */
