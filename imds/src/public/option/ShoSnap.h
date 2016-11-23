/*
 * ShoSnap.h
 *
 *  Created on: Aug 17, 2016
 *      Author: level2
 */

#ifndef SHOSNAP_H_
#define SHOSNAP_H_

#include "../market/ISnap.h"

typedef TRADE SHOTRADE;

struct SHOLINE: public LINE
{
	int64_t openInterest;       ///<持仓量
	int64_t inValue;            ///<内在价值
	int64_t timeValue;          ///<时间价值
	double impliedVolatility;   ///<隐含波动率
};

struct SHOSNAP: public SNAP
{
	LEVEL bid[5];
	LEVEL offer[5];
    int64_t hand;                   ///<现手
    int64_t in;                     ///<内盘
    int64_t out;                    ///<外盘
    int32_t pin;                    ///<成交方向

	char RFStreamID[8];             ///<参考数据类型标识符
	char contractID[24];            ///<期权合约代码
	char underlyingSecurityID[8];   ///<标的证券代码
	char underlyingSymbol[16];      ///<基础证券证券名称
	char underlyingType[8];         ///<标的证券类型
	char optionType[8];             ///<欧式美式
	char callOrPut[8];              ///<认购认沽
	int64_t contractMultiplierUnit; ///<合约单位
	int64_t exercisePrice;          ///<期权行权价
	int32_t startDate;              ///<首个交易日
	int32_t endDate;                ///<最后交易日
	int32_t exerciseDate;           ///<期权行权日
	int32_t deliveryDate;           ///<行权交割日
	int32_t expireDate;             ///<期权到期日
	char updateVersion[8];          ///<合约版本号
	int64_t totalLongPositon;       ///<当前合约未平仓数
	int64_t prevSettlePrice;        ///<合约前结算价

	int64_t underlyingClosePx;      ///<标的证券前收盘
	char priceLimitType[8];         ///<涨跌幅限制类型
	int64_t dailyPriceUpLimit;      ///<涨幅上限价格
	int64_t dailyPriceDownLimit;    ///<跌幅下限价格
	int64_t marginUnit;             ///<单位保证金
	int32_t marginRatioParam1;      ///<保证金计算比例参数一
	int32_t marginRatioParam2;      ///<保证金计算比例参数二
	int64_t roundLot;               ///<整手数
	int64_t lmtOrdMinFloor;         ///<单笔限价申报下限
	int64_t lmtOrdMaxFloor;         ///<单笔限价申报上限
	int64_t mktOrdMinFloor;         ///<单笔市价申报下限
	int64_t mktOrdMaxFloor;         ///<单笔市价申报上限
	int64_t tickSize;               ///<最小报价单位
	char securityStatusFlag[16];    ///<期权合约状态信息标签

	int64_t stockLastPx;            ///<标的现价
	int64_t settlePx;               ///<今日结算价
	int64_t auctionPx;              ///<波动性中断参考价
	int64_t auctionQty;             ///<波动性中断集合竞价虚拟匹配量
	int64_t openInterest;           ///<总持仓量
	int64_t numTrades;              ///<
	int64_t lastVolume;             ///<
	char tradingPhaseCode[8];       ///<市场状态
	static char tradingSessionID[8];///<
	int64_t leverageRatio;          ///<杠杆比率
	int64_t premiumRate;            ///<溢价率
	int32_t remainingDays;          ///<剩余天数
	int64_t lastestEnquiryTime;     ///<最新询价时间
	int64_t inValue;                ///<
	int64_t timeValue;              ///<
	double impliedVolatility;       ///<隐含波动率
	static double riskFreeInterestRate;///<利率
	double delta;                   ///<
	double gamma;                   ///<
	double theta;                   ///<
	double vega;                    ///<
	double rho;                     ///<
	SHOLINE line;
	SHOTRADE trade;
};

#endif /* SHOSNAP_H_ */
