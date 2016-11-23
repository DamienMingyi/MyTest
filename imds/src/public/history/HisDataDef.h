/*
 * HisDataDef.h
 *
 *  Created on: 2016/9/28/
 *      Author: user
 */

#ifndef SRC_PUBLIC_HISTORY_HISDATADEF_H_
#define SRC_PUBLIC_HISTORY_HISDATADEF_H_

#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <boost/make_shared.hpp>
#include <boost/format.hpp>
using namespace std;

#pragma pack(push)
#pragma pack(1)

enum
{
	MAX_LEVEL_SIZE = 5,
	MAX_LEVEL2_SIZE = 10,
	LEVEL2_ORDER_SIZE = 50
};

struct HIS_SNAP_L1
{
	uint64_t	uiDateTime;						//日期时间
	uint64_t    uiPreClosePx;					//昨收价
	uint64_t	uiOpenPx;						//开始价
	uint64_t	uiHighPx;						//最高价
	uint64_t	uiLowPx;						//最低价
	uint64_t	uiLastPx;						//收盘价
	uint64_t	uiVolume;						//成交量
	uint64_t	uiAmount;						//成交金额
	uint64_t	arrBidPx[MAX_LEVEL_SIZE];		//申买价
	uint64_t	arrBidSize[MAX_LEVEL_SIZE];		//申买量
	uint64_t	arrOfferPx[MAX_LEVEL_SIZE];		//申卖价
	uint64_t	arrOfferSize[MAX_LEVEL_SIZE];	//申卖量
	uint64_t	uiNumTrades;					//成交笔数
	uint64_t	uiIOPV;							//净值估值
	uint64_t	uiNAV;							//净资产价值
	char		strPhaseCode[8];				//交易时段
};

struct HIS_SNAP_SHO
{
	uint64_t	uiDateTime;						//日期时间
	uint64_t    uiPreSettlePx;					//昨天结算价
	uint64_t	uiOpenPx;						//开始价
	uint64_t	uiHighPx;						//最高价
	uint64_t	uiLowPx;						//最低价
	uint64_t	uiLastPx;						//收盘价
	uint64_t	uiTotalLongPosition;			//当前合约未平仓数量
	uint64_t	uiTotalVolumeTrade;				//成交量
	uint64_t	uiTotalValueTrade;				//成交金额
	uint64_t	arrBidPx[MAX_LEVEL_SIZE];		//申买价
	uint64_t	arrBidSize[MAX_LEVEL_SIZE];		//申买量
	uint64_t	arrOfferPx[MAX_LEVEL_SIZE];		//申卖价
	uint64_t	arrOfferSize[MAX_LEVEL_SIZE];	//申卖量
	char		strPhaseCode[8];				//交易时段
};

struct HIS_SNAP_L2 {
	uint64_t 	uiDateTime;							    //日期时间
	uint64_t 	uiPreClosePx;                           //昨收价
	uint64_t 	uiOpenPx;                               //开始价
	uint64_t 	uiHighPx;                               //最高价
	uint64_t 	uiLowPx;                                //最低价
	uint64_t 	uiLastPx;                               //收盘价
	uint64_t 	uiNumTrades;                            //成交笔数
	char 		strInstrumentStatus[8];                 //交易状态
	uint64_t 	uiTotalVolumeTrade;                     //成交总量
	uint64_t 	uiTotalValueTrade;                      //成交总金额
	uint64_t	uiTotalBidQty;                          //委托买入总量
	uint64_t	uiTotalOfferQty;                        //委托卖出总量
	uint64_t	uiWeightedAvgBidPx;                     //加权平均委买价格（债券共用）
	uint64_t	uiWeightedAvgOfferPx;                   //加权平均委卖价格（债券共用）
	uint64_t	uiWithdrawBuyNumber;                    //买入撤单笔数
	uint64_t	uiWithdrawBuyAmount;                    //买入撤单数量
	uint64_t	uiWithdrawBuyMoney;                     //买入撤单金额
	uint64_t	uiWithdrawSellNumber;                   //卖出撤单笔数
	uint64_t	uiWithdrawSellAmount;                   //卖出撤单数量
	uint64_t	uiWithdrawSellMoney;                    //卖出撤单金额
	uint64_t	uiTotalBidNumber;                       //买入总笔数
	uint64_t	uiTotalOfferNumber;                     //卖出总笔数
	uint64_t	uiBidTradeMaxDuration;                  //买入委托成交最大等待时间
	uint64_t	uiOfferTradeMaxDuration;                //卖出委托成交最大等待时间
	uint64_t	uiNumBidOrders;                         //买方委托价位数
	uint64_t 	uiNumOfferOrders;                       //卖方委托价位数
	uint64_t 	arrBidPrice[MAX_LEVEL2_SIZE];                   //申买价
	uint64_t 	arrBidOrderQty[MAX_LEVEL2_SIZE];                //申买量
	uint64_t 	arrBidNumOrders[MAX_LEVEL2_SIZE];               //申买十实际总委托笔数
	uint64_t 	arrBidOrders[LEVEL2_ORDER_SIZE];                //申买一前50笔订单
	uint64_t 	arrOfferPrice[MAX_LEVEL2_SIZE];                 //申卖价
	uint64_t 	arrOfferOrderQty[MAX_LEVEL2_SIZE];              //申卖量
	uint64_t 	arrOfferNumOrders[MAX_LEVEL2_SIZE];             //申卖十实际总委托笔数
	uint64_t 	arrOfferOrders[LEVEL2_ORDER_SIZE];              //申卖一前50笔订单
	uint64_t 	uiIOPV;                                 //ETF净值估值（ETF）
	uint64_t 	uiETFBuyNumber;                         //ETF申购笔数（ETF）
	uint64_t 	uiETFBuyAmount;                         //ETF申购数量（ETF）
	uint64_t 	uiETFBuyMoney;                          //ETF申购金额（ETF）
	uint64_t 	uiETFSellNumber;                        //ETF赎回笔数（ETF）
	uint64_t 	uiETFSellAmount;                        //ETF赎回数量（ETF）
	uint64_t 	uiETFSellMoney;                         //ETF赎回金额（ETF）
};

struct HIS_AUCTION
{
	uint64_t	uiDateTime;						//日期时间
	uint64_t	uiPrice;						//成交价
	uint64_t	uiVirtualAuctionQty;			//虚拟匹配量
	uint64_t	uiLeaveQty;						//虚拟未匹配量
	char		strSide[8];						//匹配情况
};

struct HIS_TICK
{
	uint64_t 	uiDateTime;                     //日期时间
	uint64_t 	uiPrice;                        //成交价
	uint64_t 	uiVolume;                       //成交数据量
	uint64_t 	uiAmount;                       //成交金额
	uint64_t 	uiBuyNo;                        //买方订单号
	uint64_t 	uiSellNo;                       //卖方订单号
};

struct HIS_CANDLESTICK
{
	uint64_t	uiDateTime;						//日期时间
	uint64_t	uiPreClosePx;					//昨收价
	uint64_t	uiOpenPx;						//开始价
	uint64_t	uiHighPx;						//最高价
	uint64_t	uiLowPx;						//最低价
	uint64_t	uiLastPx;						//收盘价
	uint64_t	uiVolume;						//成交量
	uint64_t	uiAmount;						//成交金额
	int64_t		iMF;							//ETF净值估值或期权持仓量
};

typedef map<string, vector<boost::shared_ptr<HIS_SNAP_SHO> > > SHO_SNAP_MAP_VECTOR;
typedef map<string, vector<boost::shared_ptr<HIS_SNAP_L1> > > SH1_SNAP_MAP_VECTOR;
typedef map<string, vector<boost::shared_ptr<HIS_SNAP_L2> > > SH2_SNAP_MAP_VECTOR;
typedef map<string, vector<boost::shared_ptr<HIS_AUCTION> > > AUCTION_MAP_VECTOR;
typedef map<string, vector<boost::shared_ptr<HIS_CANDLESTICK> > > CANDLESTICK_MAP_VECTOR;
typedef map<string, boost::shared_ptr<HIS_CANDLESTICK> > CANDLESTICK_MAP;

#pragma pack(pop)

#endif /* SRC_PUBLIC_HISTORY_HISDATADEF_H_ */
