/*
 * ISnap.h
 *
 *  Created on: Aug 17, 2016
 *      Author: level2
 */

#ifndef ISNAP_H_
#define ISNAP_H_

#include <stdint.h>

const int32_t UPDATE_LEVEL_NUMBER = 20;
const int32_t UPDATE_ORDER_NUMBER = 100;

/**
 * @brief 类型 定义TRADE
 * @note 成交
 */
struct TRADE
{
	int32_t time;           ///<成交时间
	int32_t sign;           ///<成交方向
	int64_t price;          ///<成交价格
	int64_t volume;         ///<成交量
};

/**
 * @brief 类型 定义LEVEL
 * @note 档位
 */
struct LEVEL
{
	int64_t price;          ///<价格
	int64_t size;           ///<量
};

/**
 * @brief 类型 定义BASIC_INFO
 * @note 证券基本信息
 */
struct BASIC_INFO
{
	char phoneticize[16];   ///<拼音
//	char symbol[48];        ///<证券简称
	char englishName[48];   ///<英语简称
	char isin[16];          ///<
	char securityType[8];   ///<证券类型
	char securitySubType[8];///<证券子类型
	char industyCode[16];   ///<行情代码
	char regionCode[8];     ///<地区代码
	char conceptCode[1024];
	char exchangeBoard[8];  ///<交易所板块
	char selfBoard[8];      ///<自选股板块
	char currency[8];       ///<货币            
};

/**
 * @brief 类型 定义CAPITAL_DATA
 * @note 股本数据
 */
struct CAPITAL_DATA
{
	int32_t adjustingDate;      /* 变动日期 */
	int64_t totalCapital;       /* 总股本 */
	int64_t nonTradableShare;   /* 非流通股 */
	int64_t tradableShare;      /* 流通股本 */
	int64_t aTradableShare;     /* A股 */
	int64_t bTradableShare;     /* B股 */
	int64_t hTradableShare;     /* H股 */
};

/**
 * @brief 类型 定义CAPITAL_DATA
 * @note 财务摘要
 */
struct FINANCIAL_DIGEST
{
	int64_t totalAsset;           /* 总资产 */
	int64_t liquidAsset;          /* 流动资产 */              
	int64_t fixedAsset;           /* 固定资产 */              
	int64_t longTermAsset;        /* 长期投资 */              
	int64_t intangibleAsset;      /* 其他无形资产 */          
	int64_t longTermDebt;         /* 长期负债 */              
	int64_t currentLiabilities;   /* 流动负债 */              
	int64_t shareHolderEquity;    /* 股东权益 */              
	int64_t capitalReserve;       /* 资本公积金 */            
	int64_t mainBusinessIncome;   /* 主营业务收入 */          
	int64_t mainBusinessProfit;   /* 主营业务利润 */          
	int64_t totalProfit;          /* 利润总额 */              
	int64_t netProfit;            /* 净利润 */                
	int64_t undistributedProfit;  /* 未分配利润 */            
	int64_t operatingNetCashFlow;  /* 经营现金净流量 */
	int64_t investingNetCashFlow;  /* 投资现金净流量 */
	int64_t financingNetCashFlow;  /* 筹资现金净流量 */
	int64_t cashFlowIncrease;     /* 现金流量净增加额 */      
	int64_t shareholderNumber;    /* 股东人数 */
};

/**
 * @brief 类型 定义CAPITAL_DATA
 * @note 财务指标
 */
struct FINANCIAL_INDEX
{
	int64_t earningPerShare;               	 /* 每股收益 */         
	int64_t netAssetPerShare;                /* 每股净资产 */       
	int64_t operatingCashFlowPerShare;       /* 每股营业现金流量 */ 
	int64_t undistributedProfitPerShare;     /* 每股未分利润  */    
	int64_t operatingNetProfitRate;          /* 经营净利率 */       
	int64_t operatingGrossProfitRate;        /* 经营毛利率 */       
	int64_t mainBusinessProfitRate;          /* 主营业务利润率 */   
	int64_t netProfitRate;                   /* 净利润率 */         
	int64_t netReturnOnAssets;               /* 净资产收益率 */     
	int64_t currentRatio;                    /* 流动比率 */         
	int64_t quickRatio;                      /* 速动比率 */         
	int64_t assetLiabilityRatio;             /* 资产负债比率 */     
	int64_t debt2EquityRatio;                /* 负债权益比率 */     
	int64_t equityRatio;                     /* 股东权益比率 */     
	int64_t turnoverOfAccountReceivable;     /* 应收帐款周转率 */   
	int64_t stockTurnover;                   /* 存货周转率 */       
	int64_t mainRevenueGrowthRate;           /* 主营收入增长率 */   
	int64_t netProfitGrowthRate;             /* 净利润增长率 */     
	int64_t EPSG;                            /* 每股收益增长率  */  
	int64_t growthRateOfShareholder;         /* 股东权益增长率 */   
	int64_t MPIP;                            /* 主营利润增长率 */   
	int64_t totalAssetsGrowthRate;           /* 总资产增长率  */  
};

/**
 * @brief 类型 定义CAPITAL_DATA
 * @note 权息指标
 */
struct WEIGHT_DATA
{
	int32_t date;				/* 除权日期 */
	char type[8];           	/* 类型 */         
	int32_t rate;           	/* 比例 */         
	int64_t price;          	/* 价格 */ 
};

/**
 * @brief 类型 定义CAPITAL_DATA
 * @note K线
 */
struct CANDLESTICK
{
	int64_t dateTime;             	/* 日期时间 */        
	int64_t prevClosePx;            /* 昨收 */            
	int64_t openPx;                 /* 开盘价 */          
	int64_t highPx;                 /* 最高价 */          
	int64_t lowPx;                  /* 最低价 */          
	int64_t lastPx;                 /* 最新价 */          
	int64_t volume;                 /* 成交量 */          
	int64_t amount;                 /* 成交金额 */        
	int64_t avgPx;                  /* 均价 */            
	//others
	int64_t iopv;					/* 债券到期收益率 */
	int64_t yield2Mature;              
	int64_t openInterest;           /* 持仓量 */
	int64_t finance;                /* 融资（内在价值） */
	int64_t securityLending;        /* 融券（时间价值） */
	int64_t inValue;
	int64_t timeValue;
};

/**
 * @brief 类型 定义CAPITAL_DATA
 * @note 分时线
 */
struct LINE
{
	int64_t dateTime;			/* 时间 */
	int64_t openPx;             /* 开盘价 */      
	int64_t highPx;             /* 最高价 */      
	int64_t lowPx;              /* 最低价 */      
	int64_t lastPx;             /* 最新价 */      
	int64_t avgPx;              /* 均价 */        
	int64_t volume;             /* 成交量 */      
	int64_t amount;             /* 成交金额 */    
	int64_t change;             /* 涨跌 */        
	int64_t amountOfIncrease;   /* 涨跌幅 */      
	int64_t amplitude;          /* 振幅 */
};

struct SNAP
{
	int32_t date;					/* 日期 */
	int32_t time;                   /* 时间 */     
	char exchangeCode[8];           /* 交易所代码 */
	char marketCode[8];             /* 市场代码 */ 
	char securityID[16];            /* 证券代码 */ 
	char symbol[48];
	char sectiontype;
	int32_t lineTime;				/* 分时线时间 */
	int32_t pos;                    /* 位置 */         
	int64_t prevClosePx;            /* 昨收 */         
	int64_t openPx;                 /* 开盘价 */       
	int64_t highPx;                 /* 最高价 */       
	int64_t lowPx;                  /* 最低价 */       
	int64_t lastPx;                 /* 最新价 */       
	int64_t avgPx;                  /* 均价 */         
	int64_t volume;                 /* 成交量 */       
	int64_t amount;                 /* 成交金额 */     
	int64_t change;                 /* 涨跌 */         
	int64_t amountOfIncrease;       /* 涨跌幅 */       
	int64_t amplitude;              /* 振幅 */         
	int64_t interestRate;           
	int64_t prevVolume;             /* 前成交量 */
	int64_t prevAmount;             /* 前成交金额 */
	int64_t prevSnapHigh;          
	int64_t prevSnapLow;           
	int64_t prevLineVolume;			/* 前分时线量 */
	int64_t prevLineAmount;         /* 前分时线金额 */
};


#endif /* ISNAP_H_ */
