/*
 * SZv5Snap.h
 *
 *  Created on: 2016年10月18日
 *      Author: mds
 */

#ifndef SRC_PUBLIC_SZSTOCK_SZV5SNAP_H_
#define SRC_PUBLIC_SZSTOCK_SZV5SNAP_H_

#include <stdint.h>

typedef LINE SZ1LINE;
typedef TRADE SZ1TRADE;

struct PRICE_LIMIT_SETTING
{
	char type[8];							/*参数类型*/
	char hasPriceLimit[8];            		/*是否有涨跌限制*/
	char referPriceType[8];           		/*基准价类型*/
	char limitType[8];                		/*涨跌限制类型*/
	int64_t limitUpRate;              		/*上涨幅度*/
	int64_t limitDownRate;            		/*下跌幅度*/
	int64_t limitUpAbolute;           		/*上涨限价*/
	int64_t limitDownAbolute;         		/*下跌限价*/
	char hasAuctionLimit[8];          		/*是否有有效竞价范围限制*/
	char auctionLimitType[8];         		/*有效范围限制类型*/
	int64_t auctionUpDownRate;        		/*有效范围涨跌幅度*/
	int64_t auctionUpDownAbsolute;    		/*有效范围涨跌价格*/
};

struct SZSNAP: public SNAP
{
	char securityIDSource[8];				/*证券代码源*/
	char underlyingSecurityID[16];        	/*基础证券代码*/
	BASIC_INFO	basicinfo;
	CAPITAL_DATA capitaldata;
	FINANCIAL_DIGEST	financialDigest;
	FINANCIAL_INDEX	financialIndex;
	WEIGHT_DATA	weightedData;
	int64_t qtyUnit;						/*数量单位*/
	char dayTrading[8];                 	/*是否支持当日回转交易*/
	char status[16];                    	/*状态*/
	int64_t outstandingShare;           	/*总发行量*/
	int64_t publicFloatShareQuantity;   	/*流通股数*/
	int64_t parValue;                   	/*面值*/
	char gageFlag[8];                   	/*是否可作为融资融券可充抵保证金证券*/
	int32_t gageRatio;                  	/*可充抵保证金折算率*/
	char crdBuyUnderlying[8];           	/*是否为融资标的*/
	char crdSellUnderlying[8];          	/*是否为融券标的*/
	int32_t priceCheckMode;             	/*提价检查方式*/
	char pledgeFlag[8];                 	/*是否可质押入库*/
	int32_t contractMultiplier;         	/*对回购标准券折算率*/
	char regularShare[8];               	/*对应回购标准券*/
	char qualificationFlag[8];          	/*投资者适当性管理标识*/
	//stock
	char industryClassification[8];			/*行业种类*/
	int64_t previousYearProfitPerShare;		/*上年每股利润*/
	int64_t currentYearProfitShare;			/*本年每股利润*/
	char offeringFlag[8];					/*是否处于要约收购期*/
	//fund
	int64_t nav;							/*T-1日基金净值*/
	//bond
	int32_t couponRate;						/*票面年利率*/
	int64_t issuePrice;						/*贴现发行价*/
	int64_t interest;						/*每百元应计利息*/
	int32_t interestAccrualDate;			/*发行起息日或本次付息起息日*/
	int32_t maturityDate;					/*到期日*/
	//warrant
	int64_t exercisePrice;					/*行权价*/
	int64_t exerciseRatio;					/*行权比例*/
	int32_t exerciseBeginDate;				/*行权起始日*/
	int32_t exerciseEndDate;				/*行权截止日*/
	char callOrPut[8];						/*认购或认沽*/
	char deliveryType[8];					/*交割方式*/
	int64_t clearingPrice;					/*结算价格*/
	char exerciseType[8];					/*行权方式*/
	int32_t lastTradeDay;					/*最后交易日*/
	int32_t expirationDays; 				/*购回期限*/
	int32_t listType;						/*挂牌类型*/
	int32_t deliveryDay;					/*交割日期*/
	int32_t deliveryMonth;					/*交割月份*/
	int32_t adjustTimes;					/*调整次数*/
	int64_t contractUnit;					/*合约单位*/
	int64_t prevClearingPrice;				/*昨日结算价*/
	int64_t contractPosition;				/*合约持仓量*/
	int32_t Interest;						/*票面股息率*/
	//cashauctionparams
	int64_t buyQtyUpperLimit;				/*买数量上限*/
	int64_t sellQtyUpperLimit;				/*卖数量上限*/
	int64_t buyQtyUnit;						/*买数量单位*/
	int64_t sellQtyUnit;					/*卖数量单位*/
	int64_t	priceTick;						/*价格档位*/
	PRICE_LIMIT_SETTING	priceLimitSetting[3];
	char marketMakerFlag[8];				/*做市商标志*/
	char tradingPhaseCode[8];				/*产品所处的交易阶段代码*/
	char mdStreamID[8];						/*行情类别*/
	int64_t numTrades;						/*成交笔数*/
	uint32_t stockNum;
	LEVEL    bid[5];
	LEVEL    offer[5];
	int64_t  change2;						/*升跌二*/
	int64_t  sellSum;						/*买入汇总价*/
	int64_t  sellSumVol;					/*买入汇总量*/
	int64_t  buySum;						/*卖出汇总价*/
	int64_t  buySumVol;						/*卖出汇总量*/
	int64_t  per1;							/*股票市盈率一*/
	int64_t  per2;							/*股票市盈率二*/
	int64_t  iopv;							/*基金实时参考净值*/
	int64_t  premiumRate;					/*权证溢价率*/
	int64_t  upLimit;						/*涨停价*/
	int64_t  downLimit;						/*跌停价*/
	int64_t  position;						/*合约持仓量*/
	uint64_t hand;
	uint64_t in;
	uint64_t out;
	int32_t  pin;
	char currency[8];
	int32_t listDate;
	int32_t securityType;
	SZ1LINE line;
	SZ1TRADE trade;
};



#endif /* SRC_PUBLIC_SZSTOCK_SZV5SNAP_H_ */
