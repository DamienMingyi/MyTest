/*
 * Sh2Snap.h
 *
 *  Created on: 2016年10月18日
 *      Author: mds
 */
#include "../market/ISnap.h"
#include "../stock/Sh1Snap.h"

typedef LINE STOCKLINE2;
typedef TRADE STOCKTRADE2;

struct STOCK2TRADE2
{
	int32_t time;											/* 成交时间 */
	int32_t sign;                     				/* 内外盘标识 */
	int64_t price;                    				/* 成交价 */
	int64_t volume;                   				/* 成交量 */
	int64_t money;                    				/* 成交金额 */
	int64_t buyNo;                    				/* 买方订单号 */
	int64_t sellNo;                   				/* 卖方订单号 */
};

struct VAP
{
	int32_t price;
	int64_t virtualAuctionQty;
	int64_t leaveQty;
	char side[4];
};

struct LEVELS
{
	int64_t price;											/* 委托价 */
	int64_t orderQty;                   			/* 委托量 */
	int64_t numOrders;                  			/* 委托总笔数 */
	int64_t order[UPDATE_ORDER_NUMBER];				/* 前50订单笔数 */
};

struct STOCKSNAP2: public SNAP
{
	int64_t numTrades;
	char tradingPhaseCode[16];
	BASIC_INFO basicinfo;
	CAPITAL_DATA capitaldata;
	FINANCIAL_DIGEST financialDigest;
	FINANCIAL_INDEX financialIndex;
	WEIGHT_DATA weightedData;
	int64_t hand;
	int64_t in;
	int64_t out;
	int32_t pin;
	LEVELS bid[UPDATE_LEVEL_NUMBER];
	LEVELS offer[UPDATE_LEVEL_NUMBER];

	//tick by tick
	int32_t tradeTime;										/* 成交时间 */
	int64_t tradePrice;         							/* 成交价 */
	int64_t tradeQty;           							/* 成交量 */
	int64_t tradeMoney;         							/* 成交金额 */
	int64_t tradeBuyNo;         							/* 买方订单号 */
	int64_t tradeSellNo;        							/* 卖方订单号 */
	char tradeBSFlag[8];        							/* 内外盘标识 */
	//virtual auction price
	VAP vap;
	//market data
	char instrumentStatus[8];								/*交易状态 */
	int64_t totalBidQty;                  				/*委托买入总量 */
	int64_t weightedAvgBidPx;             				/*加权平均委买价 */
	int64_t altWeightedAvgBidPx;          				/*债券加权平均委买价 */
	int64_t totalOfferQty;                				/*委托卖出总量 */
	int64_t weightAvgOfferPx;             				/*加权平均委卖价 */
	int64_t altWeightedAvgOfferPx;        				/*债券加权平均委卖价 */
	int64_t iopv;										/*净值估值*/
	int64_t ETFBuyNumber;                 				/*ETF申购笔数 */
	int64_t ETFBuyAmount;                 				/*ETF申购数量 */
	int64_t ETFBuyMoney;                  				/*ETF申购金额 */
	int64_t ETFSellNumber;                				/*ETF赎回笔数 */
	int64_t ETFSellAmount;                				/*ETF赎回数量 */
	int64_t ETFSellMoney;                 				/*ETF申购金额 */
	int64_t yield2Maturity;               				/*债券到期收益率 */
	int64_t totalWarrantExecQty;          				/*权证执行的总数量 */
	int64_t warLowerPx;                   				/*权证跌停价格 */
	int64_t warUpperPx;                   				/*权证涨停价格 */
	int64_t withdrawBuyNumber;            				/*买入撤单笔数 */
	int64_t withdrawBuyAmount;            				/*买入撤单笔数 */
	int64_t withdrawBuyMoney;             				/*买入撤单金额 */
	int64_t withdrawSellNumber;           				/*卖出撤单笔数 */
	int64_t withdrawSellAmount;           				/*卖出撤单数量 */
	int64_t withdrawSellMoney;            				/*卖出撤单金额 */
	int64_t totalBidNumber;               				/*买入总笔数 */
	int64_t totalOfferNumber;             				/*卖出总笔数 */
	int64_t bidTradeMaxDuration;          				/*买入委托成交最大等待时间 */
	int64_t offerTradeMaxDuration;        				/*卖出委托成交最大等待时间 */
	int64_t numBidOrders;                 				/*买方委托价位数 */
	int64_t numOfferOrders;               				/*卖方委托价位数 */
	int64_t in2;                          				/*逐笔计算内盘 */
	int64_t out2;                         				/*逐笔计算外盘 */
	int64_t hand2;                        				/*逐笔计算现手 */
	CPXX cpxx;
	GZLX gzlx;
	FJY fjy;
	DBP dbp;
	bool corrupted;										/* 丢包判断 */
	STOCKLINE2 line;
	STOCKTRADE2 trade;
	STOCK2TRADE2 trade2;
};
