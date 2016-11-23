/*
 * SZV5Struct.h
 *
 *  Created on: Oct 10, 2016
 *      Author: level2
 */

#ifndef SZV5STRUCT_H_
#define SZV5STRUCT_H_

#include <stdint.h>

#pragma pack(1)
struct LOGON_BODY
{
	char senderCompID[20];
	char targetCompID[20];
	int32_t heartBtInt;
	char password[16];
	char defaultApplVerID[32];
};

struct MSG_HEAD
{
	uint32_t msgType;
	uint32_t bodyLength;
};

struct LOGON
{
	//head
	MSG_HEAD msgHead;
	//body
	LOGON_BODY logon_body;
	//tail
	uint32_t checksum;
};

//2
struct LOGOUT_BODY
{
	int32_t sessionStatus;
	char text[20];
};
//3
struct HEART_BEAT
{
	//head
	MSG_HEAD msgHead;
	//tail
	uint32_t checksum;
};
//3900095
struct CHANNEL_HEARTBEAT_BODY
{
	uint16_t channelNo;
	int64_t appLastSeqNum;
	uint16_t endOfChannel;
};
//390090
struct MARKET_DATA_STATISTIC_BODY
{
	int64_t origTime;
	uint16_t channelNo;
	uint32_t noMDStreamID;
};

struct MARKET_DATA_CATEGORY
{
	char MDStreamID[3];
	uint32_t stockNum;
	char tradingPhaseCode[8];
};
//390013
struct REAL_TIME_STATUS_BODY
{
	int64_t origTime;
	uint16_t channelNo;
	char securityID[8];
	char securityIDSource[4];
	char financialStatus[8];
	uint32_t noSwitch;
};

struct SWITCH
{
	uint16_t securitySwitchStatus;
	uint16_t securitySwitchType;
};

struct SNP
{
	int64_t origTime;
	uint16_t channelNo;
	char MDStreamID[3];
	char securityID[8];
	char securityIDSource[4];
	char tradingPhaseCode[8];
	int64_t prevClosePx;
	int64_t numTrades;
	int64_t totalVolumeTrade;
	int64_t totalValueTrade;
	uint32_t noMDEntries;
};
//300111
struct AGGREGATION_AUCTION
{
	char MDEntryType[2];
	int64_t MDEntryPx;
	int64_t MDEntrySize;
	uint16_t MDPriceLevel;
	int64_t numberOfOrders;
	uint32_t noOrders;
//	int64_t orderQty;
};
//300611
struct CLOSE
{
	char MDEntryType[2];
	int64_t MDEntryPx;
	int64_t MDEntrySize;
};
//309011
struct INDEX
{
	char MDEntryType[2];
	int64_t MDEntryPx;
};

#pragma pack()



#endif /* SZV5STRUCT_H_ */
