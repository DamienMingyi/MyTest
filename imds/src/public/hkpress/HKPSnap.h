/*
 * HKPSnap.h
 *
 *  Created on: 2016年10月18日
 *      Author: mds
 */

#ifndef SRC_PUBLIC_HKPRESS_HKPSNAP_H_
#define SRC_PUBLIC_HKPRESS_HKPSNAP_H_


#include <stdint.h>
struct MKTDT04
{
	char MDStreamID[5+1];				/*行情数据类型*/
	char SecurityID[5+1];            /*证券代码*/
	char Symbol[32+1];               /*中文证券简称*/
	char SymbolEn[15+1];             /*英文证券简称*/
	int64_t TradeVolume;             /*成交数量*/
	int64_t TotalValueTraded;        /*成交金额*/
	int64_t PreClosePx;              /*昨日收盘价*/
	int64_t NominalPrice;            /*按盘价*/
	int64_t HighPrice;               /*最高价*/
	int64_t LowPrice;                /*最低价*/
	int64_t TradePrice;              /*最新价*/
	int64_t BuyPrice1;               /*申买价一*/
	int64_t BuyVolume1;              /*申买量一*/
	int64_t SellPrice1;              /*申卖价一*/
	int64_t SellVolume1;             /*申卖量一*/
	char SecTradingStatus[8+1];      /*证券交易状态*/
	char Timestamp[12+1];            /*时间戳*/
	char Reserve[20];						/*备用*/
};

typedef LINE HKPLINE;
typedef TRADE HKPTRADE;

struct VCM
{
	int32_t VCMStartTime;			  /*市调机制开始时间*/
	int32_t VCMEndTime;             /*市调机制结束时间*/
	int64_t VCMRefPrice;            /*市调机制参考价*/
	int64_t VCMLowerPrice;          /*市调机制下限价*/
	int64_t VCMUpperPrice;          /*市调机制上限价*/
	int32_t VCMTimestamp;           /*行情时间*/
};

struct CAS
{
	int64_t CASRefPrice;			     /*收盘集合竞价时段参考价*/
	int64_t CASLowerPrice;          /*收盘集合竞价时段下限价*/
	int64_t CASUpperPrice;          /*收盘集合竞价时段上限价*/
	char OrdlmbDirection[8];        /*不能配对买卖盘方向*/
	int64_t OrdlmbQty;              /*不能配对买卖盘量*/
	int32_t CASTimestamp;           /*行情时间*/
};

struct HKPSNAP: public SNAP
{
	BASIC_INFO basicinfo;
	char securityDesc[48];			  /*英文证券全称*/
	char underlyingSecurityID[8];   /*辅助证券代码*/
	char marketID[8];               /*市场种类*/
	int32_t amountTimes;            /*AmountTimes*/
	int64_t perValue;               /*面值*/
	char perValueCurrency[8];       /*面值货币种类*/
	int64_t interest;               /*利息*/
	int32_t issueDate;              /*上市日期*/
	int32_t roundLot;               /*买卖单位*/
	char text[56];                  /*备注*/
	char securityStatusFlag[16];    /*产品状态信息*/
	int64_t nominalpx;              /*按盘价*/
	LEVEL bid;
	LEVEL offer;
	char secTradingStatus[16];      /*证券交易状态*/
	int32_t	MDTime;                 /*行情时间*/
	char secTradingStatus1[16];    	/*证券交易状态*/
	char secTradingStatus2[16];    	/*证券交易状态*/
	VCM vcm;
	CAS cas;
	bool close;
	int64_t hand;
	int64_t in;
	int64_t out;
	int64_t pin;
	HKPLINE line;
	HKPTRADE trade;
};



#endif /* SRC_PUBLIC_HKPRESS_HKPSNAP_H_ */
