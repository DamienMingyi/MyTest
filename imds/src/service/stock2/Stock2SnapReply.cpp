/*
 * Stock2SnapReply.cpp
 *
 *  Created on: Apr 14, 2014
 *      Author: level2
 */

#include "Stock2SnapReply.h"
#include "../../public/stock2/StockSnap2.h"

COL_DESC_ST Stock2SnapReply::stock2SnapDesc[] =
{
    //colName,  dataType,   offset, plevel, structName, formatFunc, calcRule
    /* 日期 */
    { "date", DATATYPE_INT32, OFFSET(STOCKSNAP2, date), 0, NULL, default_format_func, NULL, NULL },
    /* 时间 */
    { "time", DATATYPE_INT32, OFFSET(STOCKSNAP2, time), 0, NULL, default_format_func, NULL, NULL },
    /* 交易所代码 */
    { "exch_code", DATATYPE_STR, OFFSET(STOCKSNAP2, exchangeCode), 0, NULL, default_format_func, NULL, NULL },
    /* 市场代码 */
    { "mktcode", DATATYPE_STR, OFFSET(STOCKSNAP2, marketCode), 0, NULL, default_format_func, NULL, NULL },
    /* 证券代码 */
    { "code", DATATYPE_STR, OFFSET(STOCKSNAP2, securityID), 0, NULL, default_format_func, NULL, NULL },
    /* 证券名称*/
    { "name", DATATYPE_STR, OFFSET(STOCKSNAP2, symbol), 0, NULL, default_format_func, NULL, NULL },
    /* 事务类型 */
    { "sectype", DATATYPE_CHAR, OFFSET(STOCKSNAP2, sectiontype), 0, NULL, default_format_func, NULL, NULL },
    /* 分时线时间 */
    { "line_time", DATATYPE_INT32, OFFSET(STOCKSNAP2, lineTime), 0, NULL, default_format_func, NULL, NULL },
    /* 位置 */
    { "pos", DATATYPE_INT32, OFFSET(STOCKSNAP2, pos), 0, NULL, default_format_func, NULL, NULL },
    /* 昨收 */
    { "prev_close", DATATYPE_INT64, OFFSET(STOCKSNAP2, prevClosePx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 开盘价 */
    { "open", DATATYPE_INT64, OFFSET(STOCKSNAP2, openPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 最高价 */
    { "high", DATATYPE_INT64, OFFSET(STOCKSNAP2, highPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 最低价 */
    { "low", DATATYPE_INT64, OFFSET(STOCKSNAP2, lowPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 最新价 */
    { "last", DATATYPE_INT64, OFFSET(STOCKSNAP2, lastPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 均价 */
    { "avg_px", DATATYPE_INT64, OFFSET(STOCKSNAP2, avgPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 成交量 */
    { "volume", DATATYPE_INT64, OFFSET(STOCKSNAP2, volume), 0, NULL, default_format_func, NULL, NULL },
    /* 成交金额 */
    { "amount", DATATYPE_INT64, OFFSET(STOCKSNAP2, amount), 0, NULL, default_format_func, NULL, NULL },
    /* 涨跌,需要计算字段*/
    { "change", DATATYPE_INT64, OFFSET(STOCKSNAP2, change), 0, NULL, NULL, NULL, NULL },
    /* 涨跌幅, 需要计算字段*/
    { "chg_rate", DATATYPE_INT64, OFFSET(STOCKSNAP2, amountOfIncrease), 0, NULL, NULL, NULL, NULL },
    /* 振幅, 需要计算字段 */
    { "amp_rate", DATATYPE_INT64, OFFSET(STOCKSNAP2, amplitude), 0, NULL, NULL, NULL, NULL },
    //利率
    { "int_rate", DATATYPE_INT64, OFFSET(STOCKSNAP2, interestRate), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 前成交量 */
    { "pre_vol", DATATYPE_INT64, OFFSET(STOCKSNAP2, prevVolume), 0, NULL, default_format_func, NULL, NULL },
    /* 前成交金额 */
    { "pre_amt", DATATYPE_INT64, OFFSET(STOCKSNAP2, prevAmount), 0, NULL, default_format_func, NULL, NULL },
    /* 前快照最高价 */
    { "pre_snp_high", DATATYPE_INT64, OFFSET(STOCKSNAP2, prevSnapHigh), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 前快照最低价 */
    { "pre_snp_low", DATATYPE_INT64, OFFSET(STOCKSNAP2, prevSnapLow), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 前分时线量 */
    { "pre_ln_vol", DATATYPE_INT64, OFFSET(STOCKSNAP2, prevLineVolume), 0, NULL, default_format_func, NULL, NULL },
    /* 前分时线金额 */
    { "pre_ln_amt", DATATYPE_INT64, OFFSET(STOCKSNAP2, prevLineAmount), 0, NULL, default_format_func, NULL, NULL },
    /* 成交笔数*/
    { "num_trades", DATATYPE_INT64, OFFSET(STOCKSNAP2, numTrades), 0, NULL, default_format_func, NULL, NULL },
    /* 成交笔数*/
    { "numtrades", DATATYPE_INT64, OFFSET(STOCKSNAP2, numTrades), 0, NULL, default_format_func, NULL, NULL },
    /* 产品所处的交易阶段代码*/
    { "tradephase", DATATYPE_STR, OFFSET(STOCKSNAP2, tradingPhaseCode), 0, NULL, default_format_func, NULL, NULL },

    /*证券基本信息*/
    { "basicinfo", DATATYPE_STRUCT, OFFSET(STOCKSNAP2, basicinfo), 0, "BASIC_INFO", NULL, NULL, NULL },
    /*拼音*/
    { "pinyin", DATATYPE_STR, OFFSET(STOCKSNAP2, basicinfo.phoneticize), 0, NULL, default_format_func, NULL, NULL },
    /*英语简称*/
    { "ename", DATATYPE_STR, OFFSET(STOCKSNAP2, basicinfo.englishName), 0, NULL, default_format_func, NULL, NULL },
    /*isin*/
    { "isin", DATATYPE_STR, OFFSET(STOCKSNAP2, basicinfo.isin), 0, NULL, default_format_func, NULL, NULL },
    /*证券类型*/
    { "type", DATATYPE_STR, OFFSET(STOCKSNAP2, basicinfo.securityType), 0, NULL, default_format_func, NULL, NULL },
    /*证券类型*/
    { "subtype", DATATYPE_STR, OFFSET(STOCKSNAP2, basicinfo.securitySubType), 0, NULL, default_format_func, NULL, NULL },
    /*行业代码*/
    { "indscode", DATATYPE_STR, OFFSET(STOCKSNAP2, basicinfo.industyCode), 0, NULL, default_format_func, NULL, NULL },
    /*地区代码*/
    { "rgncode", DATATYPE_STR, OFFSET(STOCKSNAP2, basicinfo.regionCode), 0, NULL, default_format_func, NULL, NULL },
    /*交易所板块*/
    { "exch_sector", DATATYPE_STR, OFFSET(STOCKSNAP2, basicinfo.exchangeBoard), 0, NULL, default_format_func, NULL, NULL },
    /*自选股板块*/
    { "self_sector", DATATYPE_STR, OFFSET(STOCKSNAP2, basicinfo.selfBoard), 0, NULL, default_format_func, NULL, NULL },
    /*货币*/
    { "ccy", DATATYPE_STR, OFFSET(STOCKSNAP2, basicinfo.currency), 0, NULL, default_format_func, NULL, NULL },

    /*股本数据*/
    { "capitaldata", DATATYPE_STRUCT, OFFSET(STOCKSNAP2, capitaldata), 0, "CAPITAL_DATA", NULL, NULL, NULL },
    /*变动日期*/
    { "cp_adjdat", DATATYPE_INT32, OFFSET(STOCKSNAP2, capitaldata.adjustingDate), 0, NULL, default_format_func, NULL, NULL },
    /*总股本market capitalization*/
    { "cp_tolcap", DATATYPE_INT64, OFFSET(STOCKSNAP2, capitaldata.totalCapital), 0, NULL, default_format_func, NULL, NULL },
    /*非流通股*/
    { "cp_nontrdshr", DATATYPE_INT64, OFFSET(STOCKSNAP2, capitaldata.nonTradableShare), 0, NULL, default_format_func, NULL, NULL },
    /*流通股本*/
    { "cp_trdshr", DATATYPE_INT64, OFFSET(STOCKSNAP2, capitaldata.tradableShare), 0, NULL, default_format_func, NULL, NULL },
    /*A股*/
    { "cp_atrdshr", DATATYPE_INT64, OFFSET(STOCKSNAP2, capitaldata.aTradableShare), 0, NULL, default_format_func, NULL, NULL },
    /*B股*/
    { "cp_btrdshr", DATATYPE_INT64, OFFSET(STOCKSNAP2, capitaldata.bTradableShare), 0, NULL, default_format_func, NULL, NULL },
    /*H股*/
    { "cp_htrdshr", DATATYPE_INT64, OFFSET(STOCKSNAP2, capitaldata.hTradableShare), 0, NULL, default_format_func, NULL, NULL },

    /*财务摘要*/
    { "financialdigest", DATATYPE_STRUCT, OFFSET(STOCKSNAP2, financialDigest), 0, "FINANCIAL_DIGEST", NULL, NULL, NULL },
    /* 总资产 */
    { "fd_tolaset", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialDigest.totalAsset), 0, NULL, default_format_func, NULL, NULL },
    /* 流动资产 */
    { "fd_liqaset", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialDigest.liquidAsset), 0, NULL, default_format_func, NULL, NULL },
    /* 固定资产 */
    { "fd_fxaset", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialDigest.fixedAsset), 0, NULL, default_format_func, NULL, NULL },
    /* 长期投资 */
    { "fd_ltaset", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialDigest.longTermAsset), 0, NULL, default_format_func, NULL, NULL },
    /* 其他无形资产 IA*/
    { "fd_intaset", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialDigest.intangibleAsset), 0, NULL, default_format_func, NULL, NULL },
    /* 长期负债*/
    { "fd_ltdebt", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialDigest.longTermDebt), 0, NULL, default_format_func, NULL, NULL },
    /* 流动负债*/
    { "fd_curliab", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialDigest.currentLiabilities), 0, NULL, default_format_func, NULL, NULL },
    /* 股东权益 */
    { "fd_shrhldr_eqty", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialDigest.shareHolderEquity), 0, NULL, default_format_func, NULL, NULL },
    /* 资本公积金 */
    { "fd_cprsv", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialDigest.shareHolderEquity), 0, NULL, default_format_func, NULL, NULL },
    /* 主营业务收入 */
    { "fd_mbusincm", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialDigest.mainBusinessIncome), 0, NULL, default_format_func, NULL, NULL },
    /* 主营业务利润 */
    { "fd_mbusprft", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialDigest.mainBusinessProfit), 0, NULL, default_format_func, NULL, NULL },
    /* 利润总额 */
    { "fd_tolprft", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialDigest.totalProfit), 0, NULL, default_format_func, NULL, NULL },
    /* 净利润 */
    { "fd_netprft", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialDigest.netProfit), 0, NULL, default_format_func, NULL, NULL },
    /* 未分配利润 */
    { "fd_undistdprft", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialDigest.undistributedProfit), 0, NULL, default_format_func, NULL, NULL },
    /* 经营现金净流量 */
    { "fd_opernetcf", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialDigest.operatingNetCashFlow), 0, NULL, default_format_func, NULL, NULL },
    /* 投资现金净流量 */
    { "fd_invnetcf", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialDigest.investingNetCashFlow), 0, NULL, default_format_func, NULL, NULL },
    /* 筹资现金净流量 */
    { "fd_fincnetcf", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialDigest.financingNetCashFlow), 0, NULL, default_format_func, NULL, NULL },
    /* 现金流量净增加额*/
    { "fd_cfincr", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialDigest.cashFlowIncrease), 0, NULL, default_format_func, NULL, NULL },
    /* 股东人数*/
    { "fd_shrhldr_num", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialDigest.shareholderNumber), 0, NULL, default_format_func, NULL, NULL },

    /*财务摘要*/
    { "financialindex", DATATYPE_STRUCT, OFFSET(STOCKSNAP2, financialIndex), 0, "FINANCIAL_INDEX", NULL, NULL, NULL },
    /* 每股收益*/
    { "fi_earn_pershr", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialIndex.earningPerShare), 0, NULL, default_format_func, NULL, NULL },
    /* 每股净资产*/
    { "fi_netaset_pershr", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialIndex.netAssetPerShare), 0, NULL, default_format_func, NULL, NULL },
    /* 每股营业现金流量*/
    { "fi_opercf_pershr", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialIndex.operatingCashFlowPerShare), 0, NULL, default_format_func, NULL, NULL },
    /* 每股未分利润*/
    { "fi_undistdprft_pershr", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialIndex.undistributedProfitPerShare), 0, NULL, default_format_func, NULL, NULL },
    /* 经营净利率*/
    { "fi_opernetprftrt", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialIndex.operatingNetProfitRate), 0, NULL, default_format_func, NULL, NULL },
    /* 经营毛利率*/
    { "fi_opergrprftrt", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialIndex.operatingGrossProfitRate), 0, NULL, default_format_func, NULL, NULL },
    /* 主营业务利润率*/
    { "fi_mbusprftrt", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialIndex.mainBusinessProfitRate), 0, NULL, default_format_func, NULL, NULL },
    /* 主营业务利润率*/
    { "fi_netprftrt", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialIndex.netProfitRate), 0, NULL, default_format_func, NULL, NULL },
    /* 净资产收益率*/
    { "fi_netretaset", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialIndex.netReturnOnAssets), 0, NULL, default_format_func, NULL, NULL },
    /* 流动比率*/
    { "fi_cur_ratio", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialIndex.currentRatio), 0, NULL, default_format_func, NULL, NULL },
    /* 速动比率*/
    { "fi_quick_ratio", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialIndex.quickRatio), 0, NULL, default_format_func, NULL, NULL },
    /* 资产负债比率*/
    { "fi_asetliab_ratio", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialIndex.assetLiabilityRatio), 0, NULL, default_format_func, NULL, NULL },
    /* 负债权益比率*/
    { "fi_debt2eqty_ratio", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialIndex.debt2EquityRatio), 0, NULL, default_format_func, NULL, NULL },
    /* 股东权益比率*/
    { "fi_eqty_ratio", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialIndex.equityRatio), 0, NULL, default_format_func, NULL, NULL },
    /* 应收帐款周转率*/
    { "fi_toar", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialIndex.turnoverOfAccountReceivable), 0, NULL, default_format_func, NULL, NULL },
    /* 存货周转率*/
    { "fi_stkto", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialIndex.stockTurnover), 0, NULL, default_format_func, NULL, NULL },
    /* 主营收入增长率*/
    { "fi_mrevgrwrt", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialIndex.mainRevenueGrowthRate), 0, NULL, default_format_func, NULL, NULL },
    /* 净利润增长率*/
    { "fi_netprftgrwrt", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialIndex.netProfitGrowthRate), 0, NULL, default_format_func, NULL, NULL },
    /* 每股收益增长率*/
    { "fi_epsg", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialIndex.EPSG), 0, NULL, default_format_func, NULL, NULL },
    /* 股东权益增长率*/
    { "fi_grwrtshrhldr", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialIndex.growthRateOfShareholder), 0, NULL, default_format_func, NULL, NULL },
    /* 主营利润增长率*/
    { "fi_mpip", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialIndex.MPIP), 0, NULL, default_format_func, NULL, NULL },
    /* 总资产增长率*/
    { "fi_tolasetgrwrt", DATATYPE_INT64, OFFSET(STOCKSNAP2, financialIndex.totalAssetsGrowthRate), 0, NULL, default_format_func, NULL, NULL },

    /*权息指标*/
    { "weighteddata", DATATYPE_STRUCT, OFFSET(STOCKSNAP2, weightedData), 0, "WEIGHT_DATA", NULL, NULL, NULL },
    /* 除权日期 ex-dividend*/
    { "wd_exdiv_date", DATATYPE_INT32, OFFSET(STOCKSNAP2, weightedData.date), 0, NULL, default_format_func, NULL, NULL },
    /* 类型*/
    { "wd_type", DATATYPE_STR, OFFSET(STOCKSNAP2, weightedData.type), 0, NULL, default_format_func, NULL, NULL },
    /* 比例*/
    { "wd_rate", DATATYPE_INT32, OFFSET(STOCKSNAP2, weightedData.rate), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 价格*/
    { "wd_price", DATATYPE_INT64, OFFSET(STOCKSNAP2, weightedData.price), 0, NULL, default_format_func, calcRule[0], NULL },

    /* */
    { "now_vol", DATATYPE_INT64, OFFSET(STOCKSNAP2, hand), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "sell_vol", DATATYPE_INT64, OFFSET(STOCKSNAP2, in), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "buy_vol", DATATYPE_INT64, OFFSET(STOCKSNAP2, out), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "pin", DATATYPE_INT32, OFFSET(STOCKSNAP2, pin), 0, NULL, default_format_func, NULL, NULL },

    { "bid", DATATYPE_STRUCT, OFFSET(STOCKSNAP2, bid), 0, "LEVELS", NULL, NULL, NULL },
    { "ask", DATATYPE_STRUCT, OFFSET(STOCKSNAP2, offer), 0, "LEVELS", NULL, NULL, NULL },
    { "bidpx1", DATATYPE_INT64, OFFSET(STOCKSNAP2, bid[0].price), 0, NULL, default_format_func, calcRule[0], NULL },
    { "askpx1", DATATYPE_INT64, OFFSET(STOCKSNAP2, offer[0].price), 0, NULL, default_format_func, calcRule[0], NULL },

    /* 成交时间 */
    { "trade_time", DATATYPE_INT32, OFFSET(STOCKSNAP2, tradeTime), 0, NULL, default_format_func, NULL, NULL },
    /* 成交价 */
    { "trade_price", DATATYPE_INT64, OFFSET(STOCKSNAP2, tradePrice), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 成交量 */
    { "trade_vol", DATATYPE_INT64, OFFSET(STOCKSNAP2, tradeQty), 0, NULL, default_format_func, NULL, NULL },
    /* 成交金额 */
    { "trade_amt", DATATYPE_INT64, OFFSET(STOCKSNAP2, tradeMoney), 0, NULL, default_format_func, NULL, NULL },
    /* 买方订单号 */
    { "trade_buyno", DATATYPE_INT64, OFFSET(STOCKSNAP2, tradeBuyNo), 0, NULL, default_format_func, NULL, NULL },
    /* 卖方订单号 */
    { "trade_sellno", DATATYPE_INT64, OFFSET(STOCKSNAP2, tradeSellNo), 0, NULL, default_format_func, NULL, NULL },
    /* 内外盘标识 */
    { "trade_bsflag", DATATYPE_STR, OFFSET(STOCKSNAP2, tradeBSFlag), 0, NULL, default_format_func, NULL, NULL },

    /* VAP*/
    { "vap", DATATYPE_STRUCT, OFFSET(STOCKSNAP2, vap), 0, "VAP", NULL, NULL, NULL },
    { "vap_price", DATATYPE_INT32, OFFSET(STOCKSNAP2, vap.price), 0, NULL, default_format_func, calcRule[0], NULL },
    { "vap_vaqty", DATATYPE_INT64, OFFSET(STOCKSNAP2, vap.virtualAuctionQty), 0, NULL, default_format_func, NULL, NULL },
    { "vap_leaveqty", DATATYPE_INT64, OFFSET(STOCKSNAP2, vap.leaveQty), 0, NULL, default_format_func, NULL, NULL },
    { "vap_side", DATATYPE_STR, OFFSET(STOCKSNAP2, vap.side), 0, NULL, default_format_func, NULL, NULL },

    /*交易状态 */
    { "instr_status", DATATYPE_STR, OFFSET(STOCKSNAP2, instrumentStatus), 0, NULL, default_format_func, NULL, NULL },
    /*委托买入总量 */
    { "totalbidqty", DATATYPE_INT64, OFFSET(STOCKSNAP2, totalBidQty), 0, NULL, default_format_func, NULL, NULL },
    /*加权平均委买价 */
    { "weightedavgbidpx", DATATYPE_INT64, OFFSET(STOCKSNAP2, weightedAvgBidPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /*债券加权平均委买价 */
    { "altweightedavgbidpx", DATATYPE_INT64, OFFSET(STOCKSNAP2, altWeightedAvgBidPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /*委托卖出总量 */
    { "totalofferqty", DATATYPE_INT64, OFFSET(STOCKSNAP2, totalOfferQty), 0, NULL, default_format_func, NULL, NULL },
    /*加权平均委卖价 */
    { "weightedavgofferpx", DATATYPE_INT64, OFFSET(STOCKSNAP2, weightAvgOfferPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /*债券加权平均委卖价 */
    { "altweightedavgofferpx", DATATYPE_INT64, OFFSET(STOCKSNAP2, altWeightedAvgOfferPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /*净值估值*/
    { "iopv", DATATYPE_INT64, OFFSET(STOCKSNAP2, iopv), 0, NULL, default_format_func, calcRule[0], NULL },
    /*ETF申购笔数 */
    { "etfbuynumber", DATATYPE_INT64, OFFSET(STOCKSNAP2, ETFBuyNumber), 0, NULL, default_format_func, NULL, NULL },
    /*ETF申购数量 */
    { "etfbuyamount", DATATYPE_INT64, OFFSET(STOCKSNAP2, ETFBuyAmount), 0, NULL, default_format_func, NULL, NULL },
    /*ETF申购金额 */
    { "etfbuymoney", DATATYPE_INT64, OFFSET(STOCKSNAP2, ETFBuyMoney), 0, NULL, default_format_func, NULL, NULL },
    /*ETF赎回笔数 */
    { "etfsellnumber", DATATYPE_INT64, OFFSET(STOCKSNAP2, ETFSellNumber), 0, NULL, default_format_func, NULL, NULL },
    /*ETF赎回数量 */
    { "etfsellamount", DATATYPE_INT64, OFFSET(STOCKSNAP2, ETFSellAmount), 0, NULL, default_format_func, NULL, NULL },
    /*ETF申购金额 */
    { "etfsellmoney", DATATYPE_INT64, OFFSET(STOCKSNAP2, ETFSellMoney), 0, NULL, default_format_func, NULL, NULL },
    /*债券到期收益率 */
    { "yieldtomaturity", DATATYPE_INT64, OFFSET(STOCKSNAP2, yield2Maturity), 0, NULL, default_format_func, calcRule[0], calcParams[4] },
    /*权证执行的总数量 */
    { "totalwarrantexecqty", DATATYPE_INT64, OFFSET(STOCKSNAP2, totalWarrantExecQty), 0, NULL, default_format_func, NULL, NULL },
    /*权证跌停价格 */
    { "warlowerpx", DATATYPE_INT64, OFFSET(STOCKSNAP2, warLowerPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /*权证涨停价格 */
    { "warupperpx", DATATYPE_INT64, OFFSET(STOCKSNAP2, warUpperPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /*买入撤单笔数 */
    { "withdrawbuynumber", DATATYPE_INT64, OFFSET(STOCKSNAP2, totalWarrantExecQty), 0, NULL, default_format_func, NULL, NULL },
    /*买入撤单笔数 */
    { "withdrawbuyamount", DATATYPE_INT64, OFFSET(STOCKSNAP2, withdrawBuyNumber), 0, NULL, default_format_func, NULL, NULL },
    /*买入撤单金额 */
    { "withdrawbuymoney", DATATYPE_INT64, OFFSET(STOCKSNAP2, withdrawBuyMoney), 0, NULL, default_format_func, NULL, NULL },
    /*卖出撤单笔数 */
    { "withdrawsellnumber", DATATYPE_INT64, OFFSET(STOCKSNAP2, withdrawSellNumber), 0, NULL, default_format_func, NULL, NULL },
    /*卖出撤单数量 */
    { "withdrawsellamount", DATATYPE_INT64, OFFSET(STOCKSNAP2, withdrawSellAmount), 0, NULL, default_format_func, NULL, NULL },
    /*卖出撤单金额 */
    { "withdrawsellmoney", DATATYPE_INT64, OFFSET(STOCKSNAP2, withdrawSellMoney), 0, NULL, default_format_func, NULL, NULL },

    /*买入总笔数 */
    { "totalbidnumber", DATATYPE_INT64, OFFSET(STOCKSNAP2, totalBidNumber), 0, NULL, default_format_func, NULL, NULL },
    /*卖出总笔数 */
    { "totaloffernumber", DATATYPE_INT64, OFFSET(STOCKSNAP2, totalOfferNumber), 0, NULL, default_format_func, NULL, NULL },
    /*买入委托成交最大等待时间 */
    { "bidtrademaxduration", DATATYPE_INT64, OFFSET(STOCKSNAP2, bidTradeMaxDuration), 0, NULL, default_format_func, NULL, NULL },
    /*卖出委托成交最大等待时间 */
    { "offertrademaxduration", DATATYPE_INT64, OFFSET(STOCKSNAP2, offerTradeMaxDuration), 0, NULL, default_format_func, NULL, NULL },
    /*买方委托价位数 */
    { "numbidorders", DATATYPE_INT64, OFFSET(STOCKSNAP2, numBidOrders), 0, NULL, default_format_func, NULL, NULL },
    /*卖方委托价位数 */
    { "numofferorders", DATATYPE_INT64, OFFSET(STOCKSNAP2, numOfferOrders), 0, NULL, default_format_func, NULL, NULL },

    /*逐笔计算内盘 */
    { "sell_vol2", DATATYPE_INT64, OFFSET(STOCKSNAP2, in2), 0, NULL, default_format_func, NULL, NULL },
    /*逐笔计算外盘 */
    { "buy_vol2", DATATYPE_INT64, OFFSET(STOCKSNAP2, out2), 0, NULL, default_format_func, NULL, NULL },
    /*逐笔计算现手 */
    { "now_vol2", DATATYPE_INT64, OFFSET(STOCKSNAP2, hand2), 0, NULL, default_format_func, NULL, NULL },

    /*产品信息*/
    { "cpxx", DATATYPE_STRUCT, OFFSET(STOCKSNAP2, cpxx), 0, "CPXX", NULL, NULL, NULL },
    /* basicSecurityCode */
    { "cpxx_code", DATATYPE_STR, OFFSET(STOCKSNAP2, cpxx.basicSecurityCode), 0, NULL, default_format_func, NULL, NULL },
    /* 市场种类 */
    { "cpxx_mkttype", DATATYPE_STR, OFFSET(STOCKSNAP2, cpxx.marketType), 0, NULL, default_format_func, NULL, NULL },
    /* 证券类别 */
    { "cpxx_type", DATATYPE_STR, OFFSET(STOCKSNAP2, cpxx.type), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "cpxx_subtype", DATATYPE_STR, OFFSET(STOCKSNAP2, cpxx.subtype), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "cpxx_cy", DATATYPE_STR, OFFSET(STOCKSNAP2, cpxx.currency), 0, NULL, default_format_func, NULL, NULL },
    /* 面值 */
    { "cpxx_parval", DATATYPE_INT64, OFFSET(STOCKSNAP2, cpxx.parValue), 0, NULL, default_format_func, NULL, NULL },
    /* 最后交易日期 */
    { "cpxx_lastrdat", DATATYPE_INT32, OFFSET(STOCKSNAP2, cpxx.lastTradingDate), 0, NULL, default_format_func, NULL, NULL },
    { "cpxx_buynum", DATATYPE_INT64, OFFSET(STOCKSNAP2, cpxx.buyNum), 0, NULL, default_format_func, NULL, NULL },
    { "cpxx_sellnum", DATATYPE_INT64, OFFSET(STOCKSNAP2, cpxx.sellNum), 0, NULL, default_format_func, NULL, NULL },
    { "cpxx_appuplmt", DATATYPE_INT64, OFFSET(STOCKSNAP2, cpxx.applyUpimit), 0, NULL, default_format_func, NULL, NULL },
    { "cpxx_appdwnlmt", DATATYPE_INT64, OFFSET(STOCKSNAP2, cpxx.applyDownlimit), 0, NULL, default_format_func, NULL, NULL },
    { "cpxx_pxlev", DATATYPE_INT64, OFFSET(STOCKSNAP2, cpxx.priceLevel), 0, NULL, default_format_func, NULL, NULL },
    /* 上市日期*/
    { "cpxx_lisdat", DATATYPE_INT32, OFFSET(STOCKSNAP2, cpxx.listingDate), 0, NULL, default_format_func, NULL, NULL },
    /* 除权比例*/
    { "cpxx_xrrate", DATATYPE_INT32, OFFSET(STOCKSNAP2, cpxx.XRRate), 0, NULL, default_format_func, NULL, NULL },
    /* 除息金额*/
    { "cpxx_xdamt", DATATYPE_INT32, OFFSET(STOCKSNAP2, cpxx.XDAmount), 0, NULL, default_format_func, NULL, NULL },
    /* 融资标的标志*/
    { "cpxx_finatag", DATATYPE_STR, OFFSET(STOCKSNAP2, cpxx.financialTag), 0, NULL, default_format_func, NULL, NULL },
    /* 融券标的标志*/
    { "cpxx_lendtag", DATATYPE_STR, OFFSET(STOCKSNAP2, cpxx.securityLendingTag), 0, NULL, default_format_func, NULL, NULL },
    /* 产品状态标志*/
    { "cpxx_produsta", DATATYPE_STR, OFFSET(STOCKSNAP2, cpxx.productStatus), 0, NULL, default_format_func, NULL, NULL },
    /* 涨跌幅限制类型*/
    { "cpxx_updwn_lmttype", DATATYPE_STR, OFFSET(STOCKSNAP2, cpxx.upDownLimitType), 0, NULL, default_format_func, NULL, NULL },
    /* 涨跌幅限制类型*/
    { "cpxx_uplmtpx", DATATYPE_STR, OFFSET(STOCKSNAP2, cpxx.upLimitPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 涨跌幅限制类型*/
    { "cpxx_dwnlmtpx", DATATYPE_STR, OFFSET(STOCKSNAP2, cpxx.downLimitPx), 0, NULL, default_format_func, calcRule[0], NULL },

    /*国债利息*/
    { "gzlx", DATATYPE_STRUCT, OFFSET(STOCKSNAP2, gzlx), 0, "GZLX", NULL, NULL, NULL },
    /* 计息日期 */
    { "gzlx_intdt", DATATYPE_INT32, OFFSET(STOCKSNAP2, gzlx.interestDate), 0, NULL, default_format_func, NULL, NULL },
    /* 每百元应计利息额 */
    { "gzlx_intper100", DATATYPE_INT64, OFFSET(STOCKSNAP2, gzlx.interestPerHundred), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 利息天数 */
    { "gzlx_intday", DATATYPE_INT32, OFFSET(STOCKSNAP2, gzlx.daysOfInterest), 0, NULL, default_format_func, NULL, NULL },
    /* 票面利率*/
    { "gzlx_cpnrt", DATATYPE_INT64, OFFSET(STOCKSNAP2, gzlx.couponRate), 0, NULL, default_format_func, calcRule[0], NULL },

    /*非交易*/
    { "fjy", DATATYPE_STRUCT, OFFSET(STOCKSNAP2, fjy), 0, "FJY", NULL, NULL, NULL },
    /* 非交易业务类型*/
    { "fjy_nontrd_type", DATATYPE_STR, OFFSET(STOCKSNAP2, fjy.nonTradeType), 0, NULL, default_format_func, NULL, NULL },
    /* 非交易订单输入开始日期*/
    { "fjy_nontrdord_begdt", DATATYPE_INT32, OFFSET(STOCKSNAP2, fjy.nonTradeOrderBeginDate), 0, NULL, default_format_func, NULL, NULL },
    /* 非交易订单输入结束日期*/
    { "fjy_nontrdord_enddt", DATATYPE_INT32, OFFSET(STOCKSNAP2, fjy.nonTradeOrderEndDate), 0, NULL, default_format_func, NULL, NULL },
    /* 非交易价格*/
    { "fjy_nontrde", DATATYPE_INT64, OFFSET(STOCKSNAP2, fjy.nonTradePx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* IPO总量*/
    { "fjy_ipo_qutm", DATATYPE_INT64, OFFSET(STOCKSNAP2, fjy.ipoQuantum), 0, NULL, default_format_func, NULL, NULL },
    /* IPO分配方法*/
    { "fjy_ipo_dismthd", DATATYPE_STR, OFFSET(STOCKSNAP2, fjy.ipoDistributeMethod), 0, NULL, default_format_func, NULL, NULL },
    /* IPO申购价格区间下限*/
    { "fjy_ipo_dwnlmtpx", DATATYPE_INT64, OFFSET(STOCKSNAP2, fjy.ipoDownLimitPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* IPO申购价格区间上限 */
    { "fjy_ipo_uplmtpx", DATATYPE_INT64, OFFSET(STOCKSNAP2, fjy.ipoUpLimitPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 配股股权登记日 */
    { "fjy_regidate", DATATYPE_INT32, OFFSET(STOCKSNAP2, fjy.registerDate), 0, NULL, default_format_func, NULL, NULL },
    /* 配股除权日 */
    { "fjy_drdate", DATATYPE_INT32, OFFSET(STOCKSNAP2, fjy.DRDate), 0, NULL, default_format_func, NULL, NULL },
    /* 配股比例 */
    { "fjy_isurt", DATATYPE_INT64, OFFSET(STOCKSNAP2, fjy.rightIssueRate), 0, NULL, default_format_func, NULL, NULL },
    /* 配股总量 */
    { "fjy_isu_qutm", DATATYPE_INT64, OFFSET(STOCKSNAP2, fjy.rightIssueQuantum), 0, NULL, default_format_func, NULL, NULL },
    /* T-2日基金收益/基金净值*/
    { "fjy_t2_iopv", DATATYPE_INT64, OFFSET(STOCKSNAP2, fjy.t2iopv), 0, NULL, default_format_func, calcRule[0], NULL },
    /* T-1日基金收益/基金净值*/
    { "fjy_t1_iopv", DATATYPE_INT64, OFFSET(STOCKSNAP2, fjy.t1iopv), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 发行方式*/
    { "fjy_offer_mthd", DATATYPE_STR, OFFSET(STOCKSNAP2, fjy.offeringMethod), 0, NULL, default_format_func, NULL, NULL },

    /*担保品*/
    { "dbp", DATATYPE_STRUCT, OFFSET(STOCKSNAP2, dbp), 0, "DBP", NULL, NULL, NULL },
    /* 融资余额 */
    { "dbp_fina", DATATYPE_INT64, OFFSET(STOCKSNAP2, dbp.finance), 0, NULL, default_format_func, NULL, NULL },
    /* 融券余量 */
    { "dbp_lend", DATATYPE_INT64, OFFSET(STOCKSNAP2, dbp.securityLending), 0, NULL, default_format_func, NULL, NULL },

    /* 丢包判断 */
    { "corrupted", DATATYPE_BOOL, OFFSET(STOCKSNAP2, corrupted), 0, NULL, default_format_func, NULL, NULL }
};


Stock2SnapReply::Stock2SnapReply()
{
    // TODO Auto-generated constructor stub

}

Stock2SnapReply::~Stock2SnapReply()
{
    // TODO Auto-generated destructor stub
    if (this->hTblColDes)
    {
        HTableFree(this->hTblColDes, NULL);
        this->hTblColDes = NULL;
    }
}

void Stock2SnapReply::registerCol()
{
    if (this->hTblColDes == NULL)
    {
        HTableCreate(&this->hTblColDes);
        int n = sizeof(stock2SnapDesc) / sizeof(COL_DESC_ST);
        for (int i = 0; i < n; i++)
        {
            COL_DESC_ST *description = &stock2SnapDesc[i];
            HTableEnter(this->hTblColDes, description->key,
                    (char *) description, NULL);
        }
    }
}

int Stock2SnapReply::request(const char* script,const char* query,char* buffer,int& len)
{
	if (!SnapReply::parse(script,query))
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	registerCol();

	snap=((Market*)market)->getSnap(code.c_str());
	if (!snap)
	{
		if (code.compare("000000")==0)
			snap=market->getSnap(0);
		if (!snap)
			return HttpReply::replyError(HttpStatus::NotFound,buffer,len);
	}
	STOCKSNAP2* s=(STOCKSNAP2*)snap;
	date=s->date;
	time=s->time;

	HttpReply::parseSelect
	("name,prev_close,open,high,low,last,change,chg_rate,volume,amount,now_vol,buy_vol,sell_vol,bid,ask");

	if (paramSelect.empty())
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	return SnapReply::reply(buffer,len);
}

int Stock2SnapReply::getSnapString(char* buffer,const char* select)
{
	STOCKSNAP2* s=(STOCKSNAP2*)snap;

    if (strcmp(select,"change")==0)
    {
        if (s->lastPx!=0)
            return sprintf(buffer,"%s",getFloat(s->lastPx-s->prevClosePx));
        else
            return sprintf(buffer,"0.000");
    }
    if (strcmp(select,"chg_rate")==0)
    {
        if (s->lastPx!=0 && s->prevClosePx != 0)
            return sprintf(buffer,"%.2f",(s->lastPx-s->prevClosePx)*100.0/s->prevClosePx);
        else
            return sprintf(buffer,"0.00");
    }
    if (strcmp(select,"amp_rate")==0)
    {
        if (s->prevClosePx!=0 && s->lastPx != 0)
            return sprintf(buffer,"%.2f",(s->highPx-s->lowPx)*100.0/s->prevClosePx);
        else
            return sprintf(buffer,"0.00");
    }
    int slen = 0;
    if (strcmp(select, "bid") == 0)
    {
        slen = sprintf(buffer + slen, "[");
        for (int i = 0; i < 10; ++i)
        {
            if (i > 0)
                slen += sprintf(buffer + slen, ",");
            int orders;
            if (i == 0)
                orders = s->bid[i].numOrders <= 50 ? s->bid[i].numOrders : 50;
            else
                orders = 0;
            slen += sprintf(buffer + slen, "%s,%ld,%ld,%d", getFloat(s->bid[i].price),
                    s->bid[i].orderQty, s->bid[i].numOrders, orders);
            //          if(i == 0)
            //          {
            slen += sprintf(buffer + slen, ",[");
            for (int j = 0; j < orders; ++j)
            {
                if (j > 0)
                    slen += sprintf(buffer + slen, ",");
                slen += sprintf(buffer + slen, "%ld", s->bid[i].order[j]);
            }
            slen += sprintf(buffer + slen, "]");
            //          }

        }
        //      slen+=sprintf(buffer+slen,"],[");
        //      for (int i = 0; i < 50; ++ i)
        //      {
        //          if (i > 0)
        //              slen += sprintf(buffer + slen, ",");
        //          slen += sprintf(buffer + slen, "%ld", s->bid[0].order[i]);
        //      }
        slen += sprintf(buffer + slen, "]");
        return slen;
    }
    if (strcmp(select, "ask") == 0)
    {
        slen = sprintf(buffer + slen, "[");
        for (int i = 0; i < 10; ++i)
        {
            if (i > 0)
                slen += sprintf(buffer + slen, ",");
            int orders;
            if (i == 0)
                orders = s->offer[i].numOrders <= 50 ? s->offer[i].numOrders : 50;
            else
                orders = 0;
            slen += sprintf(buffer + slen, "%s,%ld,%ld,%d", getFloat(s->offer[i].price),
                    s->offer[i].orderQty, s->offer[i].numOrders, orders);
            //          if(i == 0)
            //          {
            slen += sprintf(buffer + slen, ",[");
            for (int j = 0; j < orders; ++j)
            {
                if (j > 0)
                    slen += sprintf(buffer + slen, ",");
                slen += sprintf(buffer + slen, "%ld", s->offer[i].order[j]);
            }
            slen += sprintf(buffer + slen, "]");
            //          }
        }
        //      slen+=sprintf(buffer+slen,"],[");
        //      for (int i = 0; i < 50; ++ i)
        //      {
        //          if (i > 0)
        //              slen += sprintf(buffer + slen, ",");
        //          slen += sprintf(buffer + slen, "%ld", s->offer[0].order[i]);
        //      }
        slen += sprintf(buffer + slen, "]");
        return slen;
    }
    return getString( buffer, select, snap);
    /*
	if (strcmp(select,"code")==0)
		return sprintf(buffer,"\"%s\"",s->securityID);
	if (strcmp(select,"name")==0)
		return sprintf(buffer,"\"%s\"",s->symbol);
	if (strcmp(select,"pinyin")==0)
		return sprintf(buffer,"\"%s\"",s->basicinfo.phoneticize);
	if (strcmp(select,"prev_close")==0)
		return sprintf(buffer,"%s",getFloat(s->prevClosePx));
	if (strcmp(select,"open")==0)
		return sprintf(buffer,"%s",getFloat(s->openPx));
	if (strcmp(select,"high")==0)
		return sprintf(buffer,"%s",getFloat(s->highPx));
	if (strcmp(select,"low")==0)
		return sprintf(buffer,"%s",getFloat(s->lowPx));
	if (strcmp(select,"last")==0)
		return sprintf(buffer,"%s",getFloat(s->lastPx));
	if (strcmp(select,"avg_px")==0)
		return sprintf(buffer,"%s",getFloat(s->avgPx));
	if (strcmp(select,"change")==0)
	{
		if (s->lastPx!=0)
			return sprintf(buffer,"%s",getFloat(s->lastPx-s->prevClosePx));
		else
			return sprintf(buffer,"0.000");
	}
	if (strcmp(select,"chg_rate")==0)
	{
		if (s->lastPx!=0 && s->prevClosePx != 0)
			return sprintf(buffer,"%.2f",(s->lastPx-s->prevClosePx)*100.0/s->prevClosePx);
		else
			return sprintf(buffer,"0.00");
	}
	if (strcmp(select,"amp_rate")==0)
	{
		if (s->prevClosePx!=0 && s->lastPx != 0)
			return sprintf(buffer,"%.2f",(s->highPx-s->lowPx)*100.0/s->prevClosePx);
		else
			return sprintf(buffer,"0.00");
	}
	if (strcmp(select,"volume")==0)
	{
		return sprintf(buffer,"%ld",s->volume);
	}
	if (strcmp(select,"amount")==0)
	{
		return sprintf(buffer,"%ld",s->amount);
	}
	if (strcmp(select,"now_vol")==0)
	{
		return sprintf(buffer,"%ld",s->hand);
	}
	if (strcmp(select,"buy_vol")==0)
	{
		return sprintf(buffer,"%ld",s->out);
	}
	if (strcmp(select,"sell_vol")==0)
	{
		return sprintf(buffer,"%ld",s->in);
	}

	if (strcmp(select,"now_vol2")==0)
	{
		return sprintf(buffer,"%ld",s->hand2);
	}
	if (strcmp(select,"buy_vol2")==0)
	{
		return sprintf(buffer,"%ld",s->out2);
	}
	if (strcmp(select,"sell_vol2")==0)
	{
		return sprintf(buffer,"%ld",s->in2);
	}


	if(strcmp(select, "numtrades") == 0)
		return sprintf(buffer, "%d", s->numTrades);
	if(strcmp(select, "totalbidqty") == 0)
		return sprintf(buffer, "%ld", s->totalBidQty);			//
	if(strcmp(select, "weightedavgbidpx") == 0)
		return sprintf(buffer, "%s", getFloat(s->weightedAvgBidPx));
	if(strcmp(select, "altweightedavgbidpx") == 0)
		return sprintf(buffer, "%s", getFloat(s->altWeightedAvgBidPx));
	if(strcmp(select, "totalofferqty") == 0)
		return sprintf(buffer, "%ld", s->totalOfferQty);		//
	if(strcmp(select, "weightedavgofferpx") == 0)
		return sprintf(buffer, "%s", getFloat(s->weightAvgOfferPx));
	if(strcmp(select, "altweightedavgofferpx") == 0)
		return sprintf(buffer, "%s", getFloat(s->altWeightedAvgOfferPx));
	if(strcmp(select, "iopv") == 0)
		return sprintf(buffer, "%s", getFloat(s->iopv));
	if(strcmp(select, "etfbuynumber") == 0)
		return sprintf(buffer, "%d", s->ETFBuyNumber);
	if(strcmp(select, "etfbuyamount") == 0)
		return sprintf(buffer, "%ld", s->ETFBuyAmount);			//
	if(strcmp(select, "etfbuymoney") == 0)
		return sprintf(buffer, "%ld", s->ETFBuyMoney);			//
	if(strcmp(select, "etfsellnumber") == 0)
		return sprintf(buffer, "%d", s->ETFSellNumber);
	if(strcmp(select, "etfsellamount") == 0)
		return sprintf(buffer, "%ld", s->ETFSellAmount);			//
	if(strcmp(select, "etfsellmoney") == 0)
		return sprintf(buffer, "%ld", s->ETFSellMoney);			//
	if(strcmp(select, "yieldtomaturity") == 0)
		return sprintf(buffer, "%s", getFloat(s->yield2Maturity, 4));
	if(strcmp(select, "totalwarrantexecqty") == 0)
		return sprintf(buffer, "%ld", s->totalWarrantExecQty);
	if(strcmp(select, "warlowerpx") == 0)
		return sprintf(buffer, "%s", getFloat(s->warLowerPx));
	if(strcmp(select, "warupperpx") == 0)
		return sprintf(buffer, "%s", getFloat(s->warUpperPx));
	if(strcmp(select, "withdrawbuynumber") == 0)
		return sprintf(buffer, "%d", s->withdrawBuyNumber);
	if(strcmp(select, "withdrawbuyamount") == 0)
		return sprintf(buffer, "%ld", s->withdrawBuyAmount);		//
	if(strcmp(select, "withdrawbuymoney") == 0)
		return sprintf(buffer, "%ld", s->withdrawBuyMoney);		//
	if(strcmp(select, "withdrawsellnumber") == 0)
		return sprintf(buffer, "%d", s->withdrawSellNumber);
	if(strcmp(select, "withdrawsellamount") == 0)
		return sprintf(buffer, "%ld", s->withdrawSellAmount);		//
	if(strcmp(select, "withdrawsellmoney") == 0)
		return sprintf(buffer, "%ld", s->withdrawSellMoney);	//
	if(strcmp(select, "totalbidnumber") == 0)
		return sprintf(buffer, "%d", s->totalBidNumber);
	if(strcmp(select, "totaloffernumber") == 0)
		return sprintf(buffer, "%d", s->totalOfferNumber);
	if(strcmp(select, "bidtrademaxduration") == 0)
		return sprintf(buffer, "%d", s->bidTradeMaxDuration);
	if(strcmp(select, "offertrademaxduration") == 0)
		return sprintf(buffer, "%d", s->offerTradeMaxDuration);
	if(strcmp(select, "numbidorders") == 0)
		return sprintf(buffer, "%d", s->numBidOrders);
	if(strcmp(select, "numofferorders") == 0)
		return sprintf(buffer, "%d", s->numOfferOrders);
	return 0;
	*/
}

