/*
 * SzStockListReply.cpp
 *
 *  Created on: Jun 24, 2016
 *      Author: level2
 */

#include "SzStockListReply.h"
#include "../../public/szstock/SZSnap.h"

COL_DESC_ST SzStockListReply::szStockListDesc[] =
{
    //colName,  dataType,   offset, plevel, structName, formatFunc, calcRule
    /* 日期 */
    { "date", DATATYPE_INT32, OFFSET(SZSNAP, date), 0, NULL, default_format_func, NULL, NULL },
    /* 时间 */
    { "time", DATATYPE_INT32, OFFSET(SZSNAP, time), 0, NULL, default_format_func, NULL, NULL },
    /* 交易所代码 */
    { "exch_code", DATATYPE_STR, OFFSET(SZSNAP, exchangeCode), 0, NULL, default_format_func, NULL, NULL },
    /* 市场代码 */
    { "mktcode", DATATYPE_STR, OFFSET(SZSNAP, marketCode), 0, NULL, default_format_func, NULL, NULL },
    /* 证券代码 */
    { "code", DATATYPE_STR, OFFSET(SZSNAP, securityID), 0, NULL, default_format_func, NULL, NULL },
    /* 证券名称*/
    { "name", DATATYPE_STR, OFFSET(SZSNAP, symbol), 0, NULL, default_format_func, NULL, NULL },
    /* 事务类型 */
    { "sectype", DATATYPE_CHAR, OFFSET(SZSNAP, sectiontype), 0, NULL, default_format_func, NULL, NULL },
    /* 分时线时间 */
    { "line_time", DATATYPE_INT32, OFFSET(SZSNAP, lineTime), 0, NULL, default_format_func, NULL, NULL },
    /* 位置 */
    { "pos", DATATYPE_INT32, OFFSET(SZSNAP, pos), 0, NULL, default_format_func, NULL, NULL },
    /* 昨收 */
    { "prev_close", DATATYPE_INT64, OFFSET(SZSNAP, prevClosePx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 开盘价 */
    { "open", DATATYPE_INT64, OFFSET(SZSNAP, openPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 最高价 */
    { "high", DATATYPE_INT64, OFFSET(SZSNAP, highPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 最低价 */
    { "low", DATATYPE_INT64, OFFSET(SZSNAP, lowPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 最新价 */
    { "last", DATATYPE_INT64, OFFSET(SZSNAP, lastPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 均价 */
    { "avg_px", DATATYPE_INT64, OFFSET(SZSNAP, avgPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 成交量 */
    { "volume", DATATYPE_INT64, OFFSET(SZSNAP, volume), 0, NULL, default_format_func, NULL, NULL },
    /* 成交金额 */
    { "amount", DATATYPE_INT64, OFFSET(SZSNAP, amount), 0, NULL, default_format_func, NULL, NULL },
    /* 涨跌,需要计算字段*/
    { "change", DATATYPE_INT64, OFFSET(SZSNAP, change), 0, NULL, NULL, NULL, NULL },
    /* 涨跌幅, 需要计算字段*/
    { "chg_rate", DATATYPE_INT64, OFFSET(SZSNAP, amountOfIncrease), 0, NULL, NULL, NULL, NULL },
    /* 振幅, 需要计算字段 */
    { "amp_rate", DATATYPE_INT64, OFFSET(SZSNAP, amplitude), 0, NULL, NULL, NULL, NULL },
    //利率
    { "int_rate", DATATYPE_INT64, OFFSET(SZSNAP, interestRate), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 前成交量 */
    { "pre_vol", DATATYPE_INT64, OFFSET(SZSNAP, prevVolume), 0, NULL, default_format_func, NULL, NULL },
    /* 前成交金额 */
    { "pre_amt", DATATYPE_INT64, OFFSET(SZSNAP, prevAmount), 0, NULL, default_format_func, NULL, NULL },
    /* 前快照最高价 */
    { "pre_snp_high", DATATYPE_INT64, OFFSET(SZSNAP, prevSnapHigh), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 前快照最低价 */
    { "pre_snp_low", DATATYPE_INT64, OFFSET(SZSNAP, prevSnapLow), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 前分时线量 */
    { "pre_ln_vol", DATATYPE_INT64, OFFSET(SZSNAP, prevLineVolume), 0, NULL, default_format_func, NULL, NULL },
    /* 前分时线金额 */
    { "pre_ln_amt", DATATYPE_INT64, OFFSET(SZSNAP, prevLineAmount), 0, NULL, default_format_func, NULL, NULL },
    /*证券代码源*/
    { "code_source", DATATYPE_STR, OFFSET(SZSNAP, securityIDSource), 0, NULL, default_format_func, NULL, NULL },
    /*基础证券代码*/
    { "underlying_code", DATATYPE_STR, OFFSET(SZSNAP, underlyingSecurityID), 0, NULL, default_format_func, NULL, NULL },

    /*证券基本信息*/
    { "basicinfo", DATATYPE_STRUCT, OFFSET(SZSNAP, basicinfo), 0, "BASIC_INFO", NULL, NULL, NULL },
    /*拼音*/
    { "pinyin", DATATYPE_STR, OFFSET(SZSNAP, basicinfo.phoneticize), 0, NULL, default_format_func, NULL, NULL },
    /*英语简称*/
    { "ename", DATATYPE_STR, OFFSET(SZSNAP, basicinfo.englishName), 0, NULL, default_format_func, NULL, NULL },
    /*isin*/
    { "isin", DATATYPE_STR, OFFSET(SZSNAP, basicinfo.isin), 0, NULL, default_format_func, NULL, NULL },
    /*证券类型*/
    { "type", DATATYPE_STR, OFFSET(SZSNAP, basicinfo.securityType), 0, NULL, default_format_func, NULL, NULL },
    /*证券类型*/
    { "subtype", DATATYPE_STR, OFFSET(SZSNAP, basicinfo.securitySubType), 0, NULL, default_format_func, NULL, NULL },
    /*行业代码*/
    { "indscode", DATATYPE_STR, OFFSET(SZSNAP, basicinfo.industyCode), 0, NULL, default_format_func, NULL, NULL },
    /*地区代码*/
    { "rgncode", DATATYPE_STR, OFFSET(SZSNAP, basicinfo.regionCode), 0, NULL, default_format_func, NULL, NULL },
    /*交易所板块*/
    { "exch_sector", DATATYPE_STR, OFFSET(SZSNAP, basicinfo.exchangeBoard), 0, NULL, default_format_func, NULL, NULL },
    /*自选股板块*/
    { "self_sector", DATATYPE_STR, OFFSET(SZSNAP, basicinfo.selfBoard), 0, NULL, default_format_func, NULL, NULL },
    /*货币*/
    { "ccy", DATATYPE_STR, OFFSET(SZSNAP, basicinfo.currency), 0, NULL, default_format_func, NULL, NULL },

    /*股本数据*/
    { "capitaldata", DATATYPE_STRUCT, OFFSET(SZSNAP, capitaldata), 0, "CAPITAL_DATA", NULL, NULL, NULL },
    /*变动日期*/
    { "cp_adjdat", DATATYPE_INT32, OFFSET(SZSNAP, capitaldata.adjustingDate), 0, NULL, default_format_func, NULL, NULL },
    /*总股本market capitalization*/
    { "cp_tolcap", DATATYPE_INT64, OFFSET(SZSNAP, capitaldata.totalCapital), 0, NULL, default_format_func, NULL, NULL },
    /*非流通股*/
    { "cp_nontrdshr", DATATYPE_INT64, OFFSET(SZSNAP, capitaldata.nonTradableShare), 0, NULL, default_format_func, NULL, NULL },
    /*流通股本*/
    { "cp_trdshr", DATATYPE_INT64, OFFSET(SZSNAP, capitaldata.tradableShare), 0, NULL, default_format_func, NULL, NULL },
    /*A股*/
    { "cp_atrdshr", DATATYPE_INT64, OFFSET(SZSNAP, capitaldata.aTradableShare), 0, NULL, default_format_func, NULL, NULL },
    /*B股*/
    { "cp_btrdshr", DATATYPE_INT64, OFFSET(SZSNAP, capitaldata.bTradableShare), 0, NULL, default_format_func, NULL, NULL },
    /*H股*/
    { "cp_htrdshr", DATATYPE_INT64, OFFSET(SZSNAP, capitaldata.hTradableShare), 0, NULL, default_format_func, NULL, NULL },

    /*财务摘要*/
    { "financialdigest", DATATYPE_STRUCT, OFFSET(SZSNAP, financialDigest), 0, "FINANCIAL_DIGEST", NULL, NULL, NULL },
    /* 总资产 */
    { "fd_tolaset", DATATYPE_INT64, OFFSET(SZSNAP, financialDigest.totalAsset), 0, NULL, default_format_func, NULL, NULL },
    /* 流动资产 */
    { "fd_liqaset", DATATYPE_INT64, OFFSET(SZSNAP, financialDigest.liquidAsset), 0, NULL, default_format_func, NULL, NULL },
    /* 固定资产 */
    { "fd_fxaset", DATATYPE_INT64, OFFSET(SZSNAP, financialDigest.fixedAsset), 0, NULL, default_format_func, NULL, NULL },
    /* 长期投资 */
    { "fd_ltaset", DATATYPE_INT64, OFFSET(SZSNAP, financialDigest.longTermAsset), 0, NULL, default_format_func, NULL, NULL },
    /* 其他无形资产 IA*/
    { "fd_intaset", DATATYPE_INT64, OFFSET(SZSNAP, financialDigest.intangibleAsset), 0, NULL, default_format_func, NULL, NULL },
    /* 长期负债*/
    { "fd_ltdebt", DATATYPE_INT64, OFFSET(SZSNAP, financialDigest.longTermDebt), 0, NULL, default_format_func, NULL, NULL },
    /* 流动负债*/
    { "fd_curliab", DATATYPE_INT64, OFFSET(SZSNAP, financialDigest.currentLiabilities), 0, NULL, default_format_func, NULL, NULL },
    /* 股东权益 */
    { "fd_shrhldr_eqty", DATATYPE_INT64, OFFSET(SZSNAP, financialDigest.shareHolderEquity), 0, NULL, default_format_func, NULL, NULL },
    /* 资本公积金 */
    { "fd_cprsv", DATATYPE_INT64, OFFSET(SZSNAP, financialDigest.shareHolderEquity), 0, NULL, default_format_func, NULL, NULL },
    /* 主营业务收入 */
    { "fd_mbusincm", DATATYPE_INT64, OFFSET(SZSNAP, financialDigest.mainBusinessIncome), 0, NULL, default_format_func, NULL, NULL },
    /* 主营业务利润 */
    { "fd_mbusprft", DATATYPE_INT64, OFFSET(SZSNAP, financialDigest.mainBusinessProfit), 0, NULL, default_format_func, NULL, NULL },
    /* 利润总额 */
    { "fd_tolprft", DATATYPE_INT64, OFFSET(SZSNAP, financialDigest.totalProfit), 0, NULL, default_format_func, NULL, NULL },
    /* 净利润 */
    { "fd_netprft", DATATYPE_INT64, OFFSET(SZSNAP, financialDigest.netProfit), 0, NULL, default_format_func, NULL, NULL },
    /* 未分配利润 */
    { "fd_undistdprft", DATATYPE_INT64, OFFSET(SZSNAP, financialDigest.undistributedProfit), 0, NULL, default_format_func, NULL, NULL },
    /* 经营现金净流量 */
    { "fd_opernetcf", DATATYPE_INT64, OFFSET(SZSNAP, financialDigest.operatingNetCashFlow), 0, NULL, default_format_func, NULL, NULL },
    /* 投资现金净流量 */
    { "fd_invnetcf", DATATYPE_INT64, OFFSET(SZSNAP, financialDigest.investingNetCashFlow), 0, NULL, default_format_func, NULL, NULL },
    /* 筹资现金净流量 */
    { "fd_fincnetcf", DATATYPE_INT64, OFFSET(SZSNAP, financialDigest.financingNetCashFlow), 0, NULL, default_format_func, NULL, NULL },
    /* 现金流量净增加额*/
    { "fd_cfincr", DATATYPE_INT64, OFFSET(SZSNAP, financialDigest.cashFlowIncrease), 0, NULL, default_format_func, NULL, NULL },
    /* 股东人数*/
    { "fd_shrhldr_num", DATATYPE_INT64, OFFSET(SZSNAP, financialDigest.shareholderNumber), 0, NULL, default_format_func, NULL, NULL },

    /*财务摘要*/
    { "financialindex", DATATYPE_STRUCT, OFFSET(SZSNAP, financialIndex), 0, "FINANCIAL_INDEX", NULL, NULL, NULL },
    /* 每股收益*/
    { "fi_earn_pershr", DATATYPE_INT64, OFFSET(SZSNAP, financialIndex.earningPerShare), 0, NULL, default_format_func, NULL, NULL },
    /* 每股净资产*/
    { "fi_netaset_pershr", DATATYPE_INT64, OFFSET(SZSNAP, financialIndex.netAssetPerShare), 0, NULL, default_format_func, NULL, NULL },
    /* 每股营业现金流量*/
    { "fi_opercf_pershr", DATATYPE_INT64, OFFSET(SZSNAP, financialIndex.operatingCashFlowPerShare), 0, NULL, default_format_func, NULL, NULL },
    /* 每股未分利润*/
    { "fi_undistdprft_pershr", DATATYPE_INT64, OFFSET(SZSNAP, financialIndex.undistributedProfitPerShare), 0, NULL, default_format_func, NULL, NULL },
    /* 经营净利率*/
    { "fi_opernetprftrt", DATATYPE_INT64, OFFSET(SZSNAP, financialIndex.operatingNetProfitRate), 0, NULL, default_format_func, NULL, NULL },
    /* 经营毛利率*/
    { "fi_opergrprftrt", DATATYPE_INT64, OFFSET(SZSNAP, financialIndex.operatingGrossProfitRate), 0, NULL, default_format_func, NULL, NULL },
    /* 主营业务利润率*/
    { "fi_mbusprftrt", DATATYPE_INT64, OFFSET(SZSNAP, financialIndex.mainBusinessProfitRate), 0, NULL, default_format_func, NULL, NULL },
    /* 主营业务利润率*/
    { "fi_netprftrt", DATATYPE_INT64, OFFSET(SZSNAP, financialIndex.netProfitRate), 0, NULL, default_format_func, NULL, NULL },
    /* 净资产收益率*/
    { "fi_netretaset", DATATYPE_INT64, OFFSET(SZSNAP, financialIndex.netReturnOnAssets), 0, NULL, default_format_func, NULL, NULL },
    /* 流动比率*/
    { "fi_cur_ratio", DATATYPE_INT64, OFFSET(SZSNAP, financialIndex.currentRatio), 0, NULL, default_format_func, NULL, NULL },
    /* 速动比率*/
    { "fi_quick_ratio", DATATYPE_INT64, OFFSET(SZSNAP, financialIndex.quickRatio), 0, NULL, default_format_func, NULL, NULL },
    /* 资产负债比率*/
    { "fi_asetliab_ratio", DATATYPE_INT64, OFFSET(SZSNAP, financialIndex.assetLiabilityRatio), 0, NULL, default_format_func, NULL, NULL },
    /* 负债权益比率*/
    { "fi_debt2eqty_ratio", DATATYPE_INT64, OFFSET(SZSNAP, financialIndex.debt2EquityRatio), 0, NULL, default_format_func, NULL, NULL },
    /* 股东权益比率*/
    { "fi_eqty_ratio", DATATYPE_INT64, OFFSET(SZSNAP, financialIndex.equityRatio), 0, NULL, default_format_func, NULL, NULL },
    /* 应收帐款周转率*/
    { "fi_toar", DATATYPE_INT64, OFFSET(SZSNAP, financialIndex.turnoverOfAccountReceivable), 0, NULL, default_format_func, NULL, NULL },
    /* 存货周转率*/
    { "fi_stkto", DATATYPE_INT64, OFFSET(SZSNAP, financialIndex.stockTurnover), 0, NULL, default_format_func, NULL, NULL },
    /* 主营收入增长率*/
    { "fi_mrevgrwrt", DATATYPE_INT64, OFFSET(SZSNAP, financialIndex.mainRevenueGrowthRate), 0, NULL, default_format_func, NULL, NULL },
    /* 净利润增长率*/
    { "fi_netprftgrwrt", DATATYPE_INT64, OFFSET(SZSNAP, financialIndex.netProfitGrowthRate), 0, NULL, default_format_func, NULL, NULL },
    /* 每股收益增长率*/
    { "fi_epsg", DATATYPE_INT64, OFFSET(SZSNAP, financialIndex.EPSG), 0, NULL, default_format_func, NULL, NULL },
    /* 股东权益增长率*/
    { "fi_grwrtshrhldr", DATATYPE_INT64, OFFSET(SZSNAP, financialIndex.growthRateOfShareholder), 0, NULL, default_format_func, NULL, NULL },
    /* 主营利润增长率*/
    { "fi_mpip", DATATYPE_INT64, OFFSET(SZSNAP, financialIndex.MPIP), 0, NULL, default_format_func, NULL, NULL },
    /* 总资产增长率*/
    { "fi_tolasetgrwrt", DATATYPE_INT64, OFFSET(SZSNAP, financialIndex.totalAssetsGrowthRate), 0, NULL, default_format_func, NULL, NULL },

    /*权息指标*/
    { "weighteddata", DATATYPE_STRUCT, OFFSET(SZSNAP, weightedData), 0, "WEIGHT_DATA", NULL, NULL, NULL },
    /* 除权日期 ex-dividend*/
    { "wd_exdiv_date", DATATYPE_INT32, OFFSET(SZSNAP, weightedData.date), 0, NULL, default_format_func, NULL, NULL },
    /* 类型*/
    { "wd_type", DATATYPE_STR, OFFSET(SZSNAP, weightedData.type), 0, NULL, default_format_func, NULL, NULL },
    /* 比例*/
    { "wd_rate", DATATYPE_INT32, OFFSET(SZSNAP, weightedData.rate), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 价格*/
    { "wd_price", DATATYPE_INT64, OFFSET(SZSNAP, weightedData.price), 0, NULL, default_format_func, calcRule[0], NULL },

    /* 数量单位*/
    { "qty_unit", DATATYPE_INT64, OFFSET(SZSNAP, qtyUnit), 0, NULL, default_format_func, calcRule[0], calcParams[2] },
    /* 是否支持当日回转交易*/
    { "day_trading", DATATYPE_STR, OFFSET(SZSNAP, dayTrading), 0, NULL, default_format_func, NULL, NULL },
    /* 状态*/
    { "status", DATATYPE_STR, OFFSET(SZSNAP, status), 0, NULL, default_format_func, NULL, NULL },
    /* 总发行量*/
    { "outstanding_share", DATATYPE_INT64, OFFSET(SZSNAP, outstandingShare), 0, NULL, default_format_func, calcRule[0], calcParams[2] },
    /* 流通股数*/
    { "public_float_share_quantity", DATATYPE_INT64, OFFSET(SZSNAP, publicFloatShareQuantity), 0, NULL, default_format_func, calcRule[0], calcParams[2] },
    /* 面值*/
    { "par_value", DATATYPE_INT64, OFFSET(SZSNAP, parValue), 0, NULL, default_format_func, calcRule[0], calcParams[4] },
    /* 是否可作为融资融券可充抵保证金证券*/
    { "gage_flag", DATATYPE_STR, OFFSET(SZSNAP, gageFlag), 0, NULL, default_format_func, NULL, NULL },
    /* 可充抵保证金折算率*/
    { "gage_ratio", DATATYPE_INT32, OFFSET(SZSNAP, gageRatio), 0, NULL, default_format_func, calcRule[0], calcParams[2] },
    /* 是否为融资标的*/
    { "crd_buy_underlying", DATATYPE_STR, OFFSET(SZSNAP, crdBuyUnderlying), 0, NULL, default_format_func, NULL, NULL },
    /* 是否为融券标的*/
    { "crd_sell_underlying", DATATYPE_STR, OFFSET(SZSNAP, crdSellUnderlying), 0, NULL, default_format_func, NULL, NULL },
    /* 提价检查方式*/
    { "price_check_mode", DATATYPE_INT32, OFFSET(SZSNAP, priceCheckMode), 0, NULL, default_format_func, NULL, NULL },
    /* 是否可质押入库*/
    { "pledge_flag", DATATYPE_STR, OFFSET(SZSNAP, pledgeFlag), 0, NULL, default_format_func, NULL, NULL },
    /* 对回购标准券折算率*/
    { "contract_multiplier", DATATYPE_INT32, OFFSET(SZSNAP, contractMultiplier), 0, NULL, default_format_func, calcRule[0], calcParams[4] },
    /* 对应回购标准券*/
    { "regular_share", DATATYPE_STR, OFFSET(SZSNAP, regularShare), 0, NULL, default_format_func, NULL, NULL },
    /* 投资者适当性管理标识*/
    { "qualification_flag", DATATYPE_STR, OFFSET(SZSNAP, qualificationFlag), 0, NULL, default_format_func, NULL, NULL },

    //stock
    /* 行业种类*/
    { "industry_classification", DATATYPE_STR, OFFSET(SZSNAP, industryClassification), 0, NULL, default_format_func, NULL, NULL },
    /* 上年每股利润*/
    { "previous_year_profit_per_share", DATATYPE_INT64, OFFSET(SZSNAP, previousYearProfitPerShare), 0, NULL, default_format_func, calcRule[0], calcParams[4] },
    /* 本年每股利润*/
    { "current_year_profit_per_share", DATATYPE_INT64, OFFSET(SZSNAP, currentYearProfitShare), 0, NULL, default_format_func, calcRule[0], calcParams[4] },
    /* 是否处于要约收购期*/
    { "offering_flag", DATATYPE_STR, OFFSET(SZSNAP, offeringFlag), 0, NULL, default_format_func, NULL, NULL },

    //fund
    /* T-1日基金净值*/
    { "nav", DATATYPE_INT64, OFFSET(SZSNAP, nav), 0, NULL, default_format_func, calcRule[0], calcParams[4] },

    //bond
    /* 票面年利率*/
    { "coupon_rate", DATATYPE_INT32, OFFSET(SZSNAP, couponRate), 0, NULL, default_format_func, calcRule[0], calcParams[4] },
    /* 贴现发行价*/
    { "issue_price", DATATYPE_INT64, OFFSET(SZSNAP, issuePrice), 0, NULL, default_format_func, calcRule[0], calcParams[4] },
    /*每百元应计利息, 需要计算*/
    { "interest", DATATYPE_INT64, OFFSET(SZSNAP, interest), 0, NULL, NULL, NULL, NULL },
    /*发行起息日或本次付息起息日*/
    { "interest_accurate_date", DATATYPE_INT32, OFFSET(SZSNAP, interestAccrualDate), 0, NULL, default_format_func, NULL, NULL },
    /*到期日*/
    { "intematurity_daterest", DATATYPE_INT32, OFFSET(SZSNAP, maturityDate), 0, NULL, default_format_func, NULL, NULL },

    //warrant
    /* 行权价*/
    { "exercise_price", DATATYPE_INT64, OFFSET(SZSNAP, exercisePrice), 0, NULL, default_format_func, calcRule[0], calcParams[4] },
    /* 行权比例*/
    { "exercise_ratio", DATATYPE_INT64, OFFSET(SZSNAP, exerciseRatio), 0, NULL, default_format_func, calcRule[0], calcParams[4] },
    /*行权起始日*/
    { "exercise_begin_date", DATATYPE_INT32, OFFSET(SZSNAP, exerciseBeginDate), 0, NULL, default_format_func, NULL, NULL },
    /*行权截止日*/
    { "exercise_end_date", DATATYPE_INT32, OFFSET(SZSNAP, exerciseEndDate), 0, NULL, default_format_func, NULL, NULL },
    /* 认购或认沽*/
    { "call_or_put", DATATYPE_STR, OFFSET(SZSNAP, callOrPut), 0, NULL, default_format_func, NULL, NULL },
    /* 交割方式*/
    { "delivery_type", DATATYPE_STR, OFFSET(SZSNAP, deliveryType), 0, NULL, default_format_func, NULL, NULL },
    /* 结算价格*/
    { "clearing_price", DATATYPE_INT64, OFFSET(SZSNAP, clearingPrice), 0, NULL, default_format_func, calcRule[0], calcParams[4] },
    /* 行权方式*/
    { "exercise_type", DATATYPE_STR, OFFSET(SZSNAP, exerciseType), 0, NULL, default_format_func, NULL, NULL },
    /* 最后交易日*/
    { "last_trade_day", DATATYPE_INT32, OFFSET(SZSNAP, lastTradeDay), 0, NULL, default_format_func, NULL, NULL },
    /* 购回期限*/
    { "expiration_days", DATATYPE_INT32, OFFSET(SZSNAP, expirationDays), 0, NULL, default_format_func, NULL, NULL },
    /* 挂牌类型*/
    { "list_type", DATATYPE_INT32, OFFSET(SZSNAP, listType), 0, NULL, default_format_func, NULL, NULL },
    /* 挂牌类型*/
    { "delivery_day", DATATYPE_INT32, OFFSET(SZSNAP, deliveryDay), 0, NULL, default_format_func, NULL, NULL },
    /* 交割月份*/
    { "delivery_month", DATATYPE_INT32, OFFSET(SZSNAP, deliveryMonth), 0, NULL, default_format_func, NULL, NULL },
    /* 调整次数*/
    { "adjust_times", DATATYPE_INT32, OFFSET(SZSNAP, adjustTimes), 0, NULL, default_format_func, NULL, NULL },
    /* 合约单位*/
    { "contract_unit", DATATYPE_INT64, OFFSET(SZSNAP, contractUnit), 0, NULL, default_format_func, calcRule[0], calcParams[2] },
    /* 昨日结算价*/
    { "prevClearing_price", DATATYPE_INT64, OFFSET(SZSNAP, prevClearingPrice), 0, NULL, default_format_func, calcRule[0], calcParams[4] },
    /*合约持仓量*/
    { "contract_position", DATATYPE_INT64, OFFSET(SZSNAP, contractPosition), 0, NULL, default_format_func, calcRule[0], calcParams[2] },
    /*票面股息率*/
    { "Interest", DATATYPE_INT32, OFFSET(SZSNAP, Interest), 0, NULL, default_format_func, calcRule[0], calcParams[4] },

    //cashauctionparams
    /* 买数量上限*/
    { "buy_qty_upper_limit", DATATYPE_INT64, OFFSET(SZSNAP, buyQtyUpperLimit), 0, NULL, default_format_func, calcRule[0], calcParams[2] },
    /* 卖数量上限*/
    { "sell_qty_upper_limit", DATATYPE_INT64, OFFSET(SZSNAP, sellQtyUpperLimit), 0, NULL, default_format_func, calcRule[0], calcParams[2] },
    /* 买数量单位*/
    { "buy_qty_unit", DATATYPE_INT64, OFFSET(SZSNAP, buyQtyUnit), 0, NULL, default_format_func, calcRule[0], calcParams[2] },
    /* 卖数量单位*/
    { "sell_qty_unit", DATATYPE_INT64, OFFSET(SZSNAP, sellQtyUnit), 0, NULL, default_format_func, calcRule[0], calcParams[2] },
    /* 价格档位*/
    { "price_tick", DATATYPE_INT64, OFFSET(SZSNAP, priceTick), 0, NULL, default_format_func, calcRule[0], calcParams[4] },

    //PRICE_LIMIT_SETTING
    /* price_limit_setting*/
    { "price_limit_setting_o", DATATYPE_STRUCT, OFFSET(SZSNAP, priceLimitSetting[0]), 0, "PRICE_LIMIT_SETTING", NULL, NULL, NULL },
    { "price_limit_setting_t", DATATYPE_STRUCT, OFFSET(SZSNAP, priceLimitSetting[1]), 0, "PRICE_LIMIT_SETTING", NULL, NULL, NULL },
    { "price_limit_setting_c", DATATYPE_STRUCT, OFFSET(SZSNAP, priceLimitSetting[2]), 0, "PRICE_LIMIT_SETTING", NULL, NULL, NULL },

    /* 做市商标志*/
    { "market_marker_flag", DATATYPE_STR, OFFSET(SZSNAP, marketMakerFlag), 0, NULL, default_format_func, NULL, NULL },
    /* 产品所处的交易阶段代码*/
    { "tradephase", DATATYPE_STR, OFFSET(SZSNAP, tradingPhaseCode), 0, NULL, default_format_func, NULL, NULL },
    /* 行情类别*/
    { "mdStream_id", DATATYPE_STR, OFFSET(SZSNAP, mdStreamID), 0, NULL, default_format_func, NULL, NULL },
    /* 成交笔数*/
    { "num_trades", DATATYPE_INT64, OFFSET(SZSNAP, numTrades), 0, NULL, default_format_func, NULL, NULL },
    /*  */
    { "stock_num", DATATYPE_INT64, OFFSET(SZSNAP, stockNum), 0, NULL, default_format_func, NULL, NULL },

    { "bid", DATATYPE_STRUCT, OFFSET(SZSNAP, bid), 0, "LEVEL", NULL, NULL, NULL },
    { "ask", DATATYPE_STRUCT, OFFSET(SZSNAP, offer), 0, "LEVEL", NULL, NULL, NULL },
    { "bidpx1", DATATYPE_INT64, OFFSET(SZSNAP, bid[0].price), 0, NULL, default_format_func, calcRule[0], NULL },
    { "askpx1", DATATYPE_INT64, OFFSET(SZSNAP, offer[0].price), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 升跌二*/
    { "change2", DATATYPE_INT64, OFFSET(SZSNAP, change2), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 买入汇总价*/
    { "sell_sum", DATATYPE_INT64, OFFSET(SZSNAP, sellSum), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 买入汇总量*/
    { "sell_num_vol", DATATYPE_INT64, OFFSET(SZSNAP, sellSumVol), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 卖出汇总价*/
    { "buy_sum", DATATYPE_INT64, OFFSET(SZSNAP, buySum), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 卖出汇总量*/
    { "buy_sum_vol", DATATYPE_INT64, OFFSET(SZSNAP, buySumVol), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 股票市盈率一*/
    { "per1", DATATYPE_INT64, OFFSET(SZSNAP, per1), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 股票市盈率二*/
    { "per2", DATATYPE_INT64, OFFSET(SZSNAP, per2), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 基金实时参考净值*/
    { "iopv", DATATYPE_INT64, OFFSET(SZSNAP, iopv), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 权证溢价率*/
    { "premium_rate", DATATYPE_INT64, OFFSET(SZSNAP, premiumRate), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 涨停价*/
    { "up_limit", DATATYPE_INT64, OFFSET(SZSNAP, upLimit), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 跌停价*/
    { "down_limit", DATATYPE_INT64, OFFSET(SZSNAP, downLimit), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 合约持仓量*/
    { "position", DATATYPE_INT64, OFFSET(SZSNAP, position), 0, NULL, default_format_func, calcRule[0], NULL },
    /* */
    { "now_vol", DATATYPE_INT64, OFFSET(SZSNAP, hand), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "sell_vol", DATATYPE_INT64, OFFSET(SZSNAP, in), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "buy_vol", DATATYPE_INT64, OFFSET(SZSNAP, out), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "pin", DATATYPE_INT32, OFFSET(SZSNAP, pin), 0, NULL, default_format_func, NULL, NULL },

    /* */
    { "currency", DATATYPE_STR, OFFSET(SZSNAP, currency), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "list_date", DATATYPE_INT32, OFFSET(SZSNAP, listDate), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "security_type", DATATYPE_INT32, OFFSET(SZSNAP, securityType), 0, NULL, default_format_func, NULL, NULL },
};


SzStockListReply::SzStockListReply() {
	// TODO Auto-generated constructor stub

}

SzStockListReply::~SzStockListReply() {
	// TODO Auto-generated destructor stub
    if (this->hTblColDes)
    {
        HTableFree(this->hTblColDes, NULL);
        this->hTblColDes = NULL;
    }
}

void SzStockListReply::registerCol()
{
    if (this->hTblColDes == NULL)
    {
        HTableCreate(&this->hTblColDes);
        int n = sizeof(szStockListDesc)/sizeof(COL_DESC_ST);
        for (int i = 0; i < n; i++)
        {
            COL_DESC_ST *description = &szStockListDesc[i];
            HTableEnter(this->hTblColDes, description->key,
                    (char *) description, NULL);
        }
    }
}

int SzStockListReply::request(const char* script, const char* query, char* buffer, int& len)
{
	if (!ListReply::parse(script, query))
		return HttpReply::replyError(HttpStatus::BadRequest, buffer, len);

	registerCol();
	HttpReply::parseSelect("code,name,prev_close");

	date = ((SZSNAP*)list[0].snap)->date;
	time = ((SZSNAP*)list[0].snap)->time;
	return ListReply::reply(buffer, len);
}

bool SzStockListReply::getSelfList()
{
	list.clear();
	ListCmp cmp;
	if (order.find("ASE") != string::npos || order.find("ase") != string::npos)
		cmp.order = false;
	else
		cmp.order = true;
	for (int i = 0; i < self.size(); ++ i)
	{
		cmp.snap = market->getSnap(self[i].c_str());
		if (cmp.snap)
		{
			getOrder(cmp);
			list.push_back(cmp);
		}
	}
	return !list.empty();
}

bool SzStockListReply::getTypeList()
{
	list.clear();
	ListCmp cmp;
	if (order.find("ASE") != string::npos || order.find("ase") != string::npos)
		cmp.order = false;
	else
		cmp.order = true;

	if (listType.find("exchange") != string::npos)
	{
		int st = market->getSnapCount();
		for (int i = 1; i < st; ++ i)
		{
			cmp.snap = market->getSnap(i);
			if (cmp.snap == NULL)
				break;
			bool isList = false;
			if (listSubType.find("all") != string::npos)
				isList  = true;
			else
				isList = isExchangeSubType(listSubType, cmp.snap);
			if (isList)
			{
				getOrder(cmp);
				list.push_back(cmp);
			}

		}
	}
	return !list.empty();
}

bool SzStockListReply::isExchangeSubType(string& subType, char* snap)
{
	SZSNAP* s = (SZSNAP*)snap;
	if (listSubType.find("ashare") != string::npos)
		return s->securityType == 1;
	if (listSubType.find("sme") != string::npos)
		return s->securityType == 2;
	if (listSubType.find("chinext") != string::npos)
		return s->securityType == 3;
	if (listSubType.find("bshare") != string::npos)
		return s->securityType == 4;
	if (listSubType.find("treasury_bond") != string::npos)
		return s->securityType == 5;
	if (listSubType.find("enterprice_bond") != string::npos)
		return s->securityType == 6;
	if (listSubType.find("corporation_bond") != string::npos)
		return s->securityType == 7;
	if (listSubType.find("convertible") != string::npos)
		return s->securityType == 8;
	if (listSubType.find("private_raised_bond") != string::npos)
		return s->securityType == 9;
	if (listSubType.find("exchangable_private_raised_bond") != string::npos)
		return s->securityType == 10;
	if (listSubType.find("corporation_subbond") != string::npos)
		return s->securityType == 11;
	if (listSubType.find("pledge-style_repo") != string::npos)
		return s->securityType == 12;
	if (listSubType.find("abs") != string::npos)
		return s->securityType == 13;
	if (listSubType.find("etf") != string::npos)
		return s->securityType == 14;
	if (listSubType.find("cross_market_etf") != string::npos)
		return s->securityType == 15;
	if (listSubType.find("cross_border_etf") != string::npos)
		return s->securityType == 16;
	if (listSubType.find("physical_bond_etf") != string::npos)
		return s->securityType == 17;
	if (listSubType.find("cash_bond_etf") != string::npos)
		return s->securityType == 18;
	if (listSubType.find("gold_etf") != string::npos)
		return s->securityType == 19;
	if (listSubType.find("currency_etf") != string::npos)
		return s->securityType == 20;
	if (listSubType.find("gearing_etf") != string::npos)
		return s->securityType == 21;
	if (listSubType.find("commodity_future_etf") != string::npos)
		return s->securityType == 22;
	if (listSubType.find("standard_lof") != string::npos)
		return s->securityType == 23;
	if (listSubType.find("structured_fund") != string::npos)
		return s->securityType == 24;
	if (listSubType.find("closed-ended_fund") != string::npos)
		return s->securityType == 25;
	if (listSubType.find("purchase_redemption_fund") != string::npos)
		return s->securityType == 26;
	if (listSubType.find("warrant") != string::npos)
		return s->securityType == 28;
	if (listSubType.find("equity_option") != string::npos)
		return s->securityType == 29;
	if (listSubType.find("etf_option") != string::npos)
		return s->securityType == 30;
	if (listSubType.find("preferred_stock") != string::npos)
		return s->securityType == 33;
	if (listSubType.find("security_company_short-term_bond") != string::npos)
		return s->securityType == 34;
	if (listSubType.find("exchangable_corporation_bond") != string::npos)
		return s->securityType == 35;
	if (listSubType.find("index") != string::npos)
		return s->securityType == -1;
	return false;
}

bool SzStockListReply::getOrder(ListCmp& cmp)
{
	SZSNAP* s = (SZSNAP*)cmp.snap;
	if (!order.empty())
	{
        int n = sizeof(szStockListDesc) / sizeof(COL_DESC_ST);
        for (int i = 0; i < n; i++)
        {
            COL_DESC_ST *description = &szStockListDesc[i];
            if (order.find(description->key) != string::npos)
            {
                return setOrder((char *)description, cmp.snap, cmp);
            }
        }
        /*
		if (order.find("code")!=string::npos)
		{
			cmp.type=0;
			strcpy(cmp.charCmp,s->securityID);
			return true;
		}
		else if (order.find("name")!=string::npos)
		{
			cmp.type=0;
			strcpy(cmp.charCmp,s->symbol);
			return true;
		}
		else if (order.find("prev_close")!=string::npos)
		{
			cmp.type=1;
			cmp.iCmp=s->prevClosePx;
			return true;
		}
		else if (order.find("open")!=string::npos)
		{
			cmp.type=1;
			cmp.iCmp=s->openPx;
			return true;
		}
		else if (order.find("high")!=string::npos)
		{
			cmp.type=1;
			cmp.iCmp=s->highPx;
			return true;
		}
		else if (order.find("low")!=string::npos)
		{
			cmp.type=1;
			cmp.iCmp=s->lowPx;
			return true;
		}
		else if (order.find("last")!=string::npos)
		{
			cmp.type=1;
			cmp.iCmp=s->lastPx;
			return true;
		}
		else if (order.find("change")!=string::npos)
		{
			cmp.type=1;
			cmp.iCmp = s->change;
			return true;
		}
		else if (order.find("chg_rate") != string::npos)
		{
			cmp.type=3;
			cmp.dCmp= s->change * 100.0 / s->prevClosePx;
			return true;
		}
		else if (order.find("volume")!=string::npos)
		{
			cmp.type=2;
			cmp.lCmp=s->volume;
			return true;
		}
		else if (order.find("amount")!=string::npos)
		{
			cmp.type=2;
			cmp.lCmp=s->amount;
			return true;
		}
		else if (order.find("bidpx1")!=string::npos)
		{
			cmp.type=1;
			cmp.lCmp=s->bidPx[0];
			return true;
		}
		else if (order.find("askpx1")!=string::npos)
		{
			cmp.type=1;
			cmp.lCmp=s->offerPx[0];
			return true;
		}
		else if (order.find("now_vol")!=string::npos)
		{
			cmp.type=2;
			cmp.lCmp=s->hand;
			return true;
		}
		else if (order.find("buy_vol")!=string::npos)
		{
			cmp.type=2;
			cmp.lCmp=s->out;
			return true;
		}
		else if (order.find("sell_vol")!=string::npos)
		{
			cmp.type=2;
			cmp.lCmp=s->in;
			return true;
		}
		*/
	}
	order.clear();
	return false;
}

int SzStockListReply::getSnapString(char* buffer, const char* select)
{
	SZSNAP* s = (SZSNAP*)curSnap;
	int slen=0;

    if (strcmp(select, "change") == 0)
    {
        //return sprintf(buffer, "%ld", ln->change);
        if (s->lastPx > 0)
            return sprintf(buffer, "%s", getFloat(s->lastPx - s->prevClosePx));
        else
            return sprintf(buffer, "0.000");
    }
    if (strcmp(select, "chg_rate") == 0)
    {
        //return sprintf(buffer, "%ld", ln->amountOfIncrease);
        if (s->prevClosePx > 0 && s->lastPx > 0)
            return sprintf(buffer, "%.2f",
                    (s->lastPx - s->prevClosePx) * 100.0 / s->prevClosePx);
        else
            return sprintf(buffer, "0.00");
    }
    if (strcmp(select, "amp_rate") == 0)
    {
        //return sprintf(buffer, "%ld", ln->amplitude);
        if (s->prevClosePx > 0 && s->lastPx > 0)
            return sprintf(buffer, "%.2f",
                    (s->highPx - s->lowPx) * 100.0 / s->prevClosePx);
        else
            return sprintf(buffer, "0.00");
    }

    if (strcmp(select, "interest") == 0)
        return sprintf(buffer, "%.8f", s->interest / 100000000.0);
    if (strcmp(select, "prev_close") == 0)
    {
        if (s->securityType != -1)
            return sprintf(buffer, "%s", getFloat(s->prevClosePx, 3));
        else
            return sprintf(buffer, "%s", getFloat(s->prevClosePx, 5));
    }
    if (strcmp(select, "bid") == 0)
    {
        slen=sprintf(buffer+slen,"[");
        for (int i=0;i<5;++i)
        {
            if (i>0)
                slen+=sprintf(buffer+slen,",");
            slen+=sprintf(buffer+slen,"%s,%ld",getFloat(s->bid[i].price), s->bid[i].size);
        }
        slen+=sprintf(buffer+slen,"]");
        return slen;
    }
    if (strcmp(select, "ask") == 0)
    {
        slen=sprintf(buffer+slen,"[");
        for (int i=0;i<5;++i)
        {
            if (i>0)
                slen+=sprintf(buffer+slen,",");
            slen+=sprintf(buffer+slen,"%s,%ld",getFloat(s->offer[i].price), s->offer[i].size);
        }
        slen+=sprintf(buffer+slen,"]");
        return slen;
    }

    return getString( buffer, select, curSnap);
    /*
	if (strcmp(select, "code") ==0)
		return sprintf(buffer, "\"%s\"", s->securityID);
	if (strcmp(select, "code_source") == 0)
		return sprintf(buffer, "\"%s\"", s->securityIDSource);
	if (strcmp(select, "name") == 0)
		return sprintf(buffer, "\"%s\"", s->symbol);
	if (strcmp(select, "englistName") == 0)
		return sprintf(buffer, "\"%s\"", s->englishName);
	if (strcmp(select, "ISIN") == 0)
		return sprintf(buffer, "\"%s\"", s->isin);
	if (strcmp(select, "underlying_code") == 0)
		return sprintf(buffer, "\"%s\"", s->underlyingSecurityID);
	if (strcmp(select, "underlying_code_source") == 0)
		return sprintf(buffer, "\"%s\"", s->securityIDSource);
	if (strcmp(select, "list_date") == 0)
		return sprintf(buffer, "%d", s->listDate);
	if (strcmp(select, "security_type") == 0)
		return sprintf(buffer, "%d", s->securityType);
	if (strcmp(select, "currency") == 0)
		return sprintf(buffer, "\"%s\"", s->currency);
	if (strcmp(select, "qty_unit") == 0)
		return sprintf(buffer, "%s", getFloat(s->qtyUnit, 2));
	if (strcmp(select, "day_trading") == 0)
		return sprintf(buffer, "\"%s\"", s->dayTrading);

	if (strcmp(select, "outstanding_share") == 0)
		return sprintf(buffer, "%s", getFloat(s->outstandingShare, 2));
	if (strcmp(select, "public_float_share_quantity") == 0)
		return sprintf(buffer, "%s", getFloat(s->publicFloatShareQuantity, 2));
	if (strcmp(select, "par_value") == 0)
		return sprintf(buffer, "%s", getFloat(s->parValue, 4));
	if (strcmp(select, "gage_flag") == 0)
		return sprintf(buffer, "\"%s\"", s->gageFlag);
	if (strcmp(select, "gage_ratio") == 0)
		return sprintf(buffer, "%s", getFloat(s->gageRatio, 2));
	if (strcmp(select, "crd_buy_underlying") == 0)
		return sprintf(buffer, "\"%s\"", s->crdBuyUnderlying);
	if (strcmp(select, "crd_sell_underlying") == 0)
		return sprintf(buffer, "\"%s\"", s->crdSellUnderlying);
	if (strcmp(select, "price_check_mode") == 0)
		return sprintf(buffer, "%d", s->priceCheckMode);
	if (strcmp(select, "pledge_flag") == 0)
		return sprintf(buffer, "\"%s\"", s->pledgeFlag);
	if (strcmp(select, "contract_multiplier") == 0)
		return sprintf(buffer, "%s", getFloat(s->contractMultiplier, 4));
	if (strcmp(select, "regular_share") == 0)
		return sprintf(buffer, "\"%s\"", s->regularShare);
	if (strcmp(select, "qualification_flag") == 0)
		return sprintf(buffer, "\"%s\"", s->qualificationFlag);
	if (strcmp(select, "industry__classification") == 0)
		return sprintf(buffer, "\"%s\"", s->industryClassification);
	if (strcmp(select, "previous_year_profit_per_share") == 0)
		return sprintf(buffer, "%s", getFloat(s->previousYearProfitPerShare, 4));
	if (strcmp(select, "current_year_profit_per_share") == 0)
		return sprintf(buffer, "%s", getFloat(s->currentYearProfitShare, 4));
	if (strcmp(select, "offering_flag") == 0)
		return sprintf(buffer, "\"%s\"", s->offeringFlag);
	if (strcmp(select, "nav") == 0)
		return sprintf(buffer, "%s", getFloat(s->nav, 4));
	if (strcmp(select, "coupon_rate") == 0)
		return sprintf(buffer, "%s", getFloat(s->couponRate, 4));
	if (strcmp(select, "issue_price") == 0)
		return sprintf(buffer, "%s", getFloat(s->issuePrice, 4));

	if (strcmp(select, "interest_accurate_date") == 0)
		return sprintf(buffer, "%d", s->interestAccrualDate);
	if (strcmp(select, "maturity_date") == 0)
		return sprintf(buffer, "%d", s->maturityDate);
	if (strcmp(select, "exercise_price") == 0)
		return sprintf(buffer, "%s", getFloat(s->exercisePrice, 4));
	if (strcmp(select, "exercise_ratio") == 0)
		return sprintf(buffer, "%s", getFloat(s->exerciseRatio, 4));
	if (strcmp(select, "exercise_begin_date") == 0)
		return sprintf(buffer, "%d", s->exerciseBeginDate);
	if (strcmp(select, "exercise_end_date") == 0)
		return sprintf(buffer, "%d", s->exerciseEndDate);
	if (strcmp(select, "call_or_put") == 0)
		return sprintf(buffer, "\"%s\"", s->callOrPut);
	if (strcmp(select, "delivery_type") == 0)
		return sprintf(buffer, "\"%s\"", s->deliveryType);
	if (strcmp(select, "clearing_price") == 0)
		return sprintf(buffer, "%s", getFloat(s->clearingPrice, 4));
	if (strcmp(select, "exercise_type") == 0)
		return sprintf(buffer, "\"%s\"", s->exerciseType);
	if (strcmp(select, "last_trade_day") == 0)
		return sprintf(buffer, "%d", s->lastTradeDay);
	if (strcmp(select, "expiration_days") == 0)
		return sprintf(buffer, "%d", s->expirationDays);
	if (strcmp(select, "list_type") == 0)
		return sprintf(buffer, "%d", s->listType);
	if (strcmp(select, "delivery_day") == 0)
		return sprintf(buffer, "%d", s->deliveryMonth);
	if (strcmp(select, "delivery_month") == 0)
		return sprintf(buffer, "%d", s->deliveryMonth);
	if (strcmp(select, "adjust_times") == 0)
		return sprintf(buffer, "%d", s->adjustTimes);
	if (strcmp(select, "contract_unit") == 0)
		return sprintf(buffer, "%s", getFloat(s->contractUnit, 2));
	if (strcmp(select, "prevClearing_price") == 0)
		return sprintf(buffer, "%s", getFloat(s->prevClearingPrice, 4));
	if (strcmp(select, "contract_position") == 0)
		return sprintf(buffer, "%s", getFloat(s->contractPosition, 2));
	if (strcmp(select, "Interest") == 0)
		return sprintf(buffer, "%s", getFloat(s->Interest, 4));
	if (strcmp(select, "buy_qty_upper_limit") == 0)
		return sprintf(buffer, "%s", getFloat(s->buyQtyUpperLimit, 2));
	if (strcmp(select, "sell_qty_upper_limit") == 0)
		return sprintf(buffer, "%s", getFloat(s->sellQtyUpperLimit, 2));
	if (strcmp(select, "buy_qty_unit") == 0)
		return sprintf(buffer, "%s", getFloat(s->buyQtyUnit, 2));
	if (strcmp(select, "sell_qty_unit") == 0)
		return sprintf(buffer, "%s", getFloat(s->sellQtyUnit, 2));
	if (strcmp(select, "price_tick") == 0)
		return sprintf(buffer, "%s", getFloat(s->priceTick, 4));
//	if (strcmp(select, "price_limit_setting_o") == 0)
//		return sprintf(buffer, "\"%s\",\"%s\",\"%s\",\"%s\",%s,%s,%s,%s,\"%s\",\"%s\",%s,%s",
//				s->priceLimitSetting[0].type, s->priceLimitSetting[0].hasPriceLimit, s->priceLimitSetting[0].referPriceType,
//				s->priceLimitSetting[0].limitType, getFloat(s->priceLimitSetting[0].limitUpRate, 3),
//				getFloat(s->priceLimitSetting[0].limitDownRate, 3), getFloat(s->priceLimitSetting[0].limitUpAbolute, 4),
//				getFloat(s->priceLimitSetting[0].limitDownAbolute, 4), s->priceLimitSetting[0].hasAuctionLimit,
//				s->priceLimitSetting[0].auctionLimitType, getFloat(s->priceLimitSetting[0].auctionUpDownRate, 3),
//				getFloat(s->priceLimitSetting[0].auctionUpDownAbsolute, 4));
//	if (strcmp(select, "price_limit_setting_t") == 0)
//		return sprintf(buffer, "\"%s\",\"%s\",\"%s\",\"%s\",%s,%s,%s,%s,\"%s\",\"%s\",%s,%s",
//				s->priceLimitSetting[1].type, s->priceLimitSetting[1].hasPriceLimit, s->priceLimitSetting[1].referPriceType,
//				s->priceLimitSetting[1].limitType, getFloat(s->priceLimitSetting[1].limitUpRate, 3),
//				getFloat(s->priceLimitSetting[1].limitDownRate, 3), getFloat(s->priceLimitSetting[1].limitUpAbolute, 4),
//				getFloat(s->priceLimitSetting[1].limitDownAbolute, 4), s->priceLimitSetting[1].hasAuctionLimit,
//				s->priceLimitSetting[1].auctionLimitType, getFloat(s->priceLimitSetting[1].auctionUpDownRate, 3),
//				getFloat(s->priceLimitSetting[1].auctionUpDownAbsolute, 4));
//	if (strcmp(select, "price_limit_setting_c") == 0)
//		return sprintf(buffer, "\"%s\",\"%s\",\"%s\",\"%s\",%s,%s,%s,%s,\"%s\",\"%s\",%s,%s",
//				s->priceLimitSetting[2].type, s->priceLimitSetting[2].hasPriceLimit, s->priceLimitSetting[2].referPriceType,
//				s->priceLimitSetting[2].limitType, getFloat(s->priceLimitSetting[2].limitUpRate, 3),
//				getFloat(s->priceLimitSetting[2].limitDownRate, 3), getFloat(s->priceLimitSetting[2].limitUpAbolute, 4),
//				getFloat(s->priceLimitSetting[2].limitDownAbolute, 4), s->priceLimitSetting[2].hasAuctionLimit,
//				s->priceLimitSetting[2].auctionLimitType, getFloat(s->priceLimitSetting[2].auctionUpDownRate, 3),
//				getFloat(s->priceLimitSetting[2].auctionUpDownAbsolute, 4));
	if (strcmp(select, "market_marker_flag") == 0)
		return sprintf(buffer, "\"%s\"", s->marketMakerFlag);
	if (strcmp(select, "tradingPhaseCode") == 0)
		return sprintf(buffer, "\"%s\"", s->tradingPhaseCode);
	if (strcmp(select, "mdStream_id") == 0)
		return sprintf(buffer, "\"%s\"", s->mdStreamID);
	if (strcmp(select, "numTrades") == 0)
		return sprintf(buffer, "%ld", s->numTrades);
	if (strcmp(select, "amount") == 0)
		return sprintf(buffer, "%ld", s->amount);
	if (strcmp(select, "volume") == 0)
		return sprintf(buffer, "%ld", s->volume);
	if (strcmp(select, "stock_num") == 0)
		return sprintf(buffer, "%d", s->stockNum);
	if (strcmp(select,"bidpx1")==0)
		return sprintf(buffer,"%s",getFloat(s->bidPx[0]));
	if (strcmp(select,"askpx1")==0)
		return sprintf(buffer,"%s",getFloat(s->offerPx[0]));

	if (strcmp(select, "last") == 0)
		return sprintf(buffer, "%s", getFloat(s->lastPx));
	if (strcmp(select, "open") == 0)
		return sprintf(buffer, "%s", getFloat(s->openPx));
	if (strcmp(select, "low") == 0)
		return sprintf(buffer, "%s", getFloat(s->lowPx));
	if (strcmp(select, "high") == 0)
		return sprintf(buffer, "%s", getFloat(s->highPx));
	if (strcmp(select, "change") == 0)
		return sprintf(buffer, "%s", getFloat(s->change));
	if (strcmp(select, "change2") == 0)
		return sprintf(buffer, "%s", getFloat(s->change2));
	if (strcmp(select, "chg_rate") == 0)
		return sprintf(buffer, "%.2f", s->change * 100.0 / s->prevClosePx);
	if (strcmp(select, "amp_rate") == 0)
		return sprintf(buffer, "%.2f", (s->highPx - s->lowPx) * 100.0 / s->prevClosePx);
	if (strcmp(select, "now_vol") == 0)
		return sprintf(buffer, "%ld", s->hand);
	if (strcmp(select, "sell_vol") == 0)
		return sprintf(buffer, "%ld", s->in);
	if (strcmp(select, "buy_vol") == 0)
		return sprintf(buffer, "%ld", s->out);
	if (strcmp(select, "avg_px") == 0)
		return sprintf(buffer, "%s", getFloat(s->avgPx));
	if (strcmp(select, "sell_sum") == 0)
		return sprintf(buffer, "%s", getFloat(s->sellSum));
	if (strcmp(select, "buy_sum") == 0)
		return sprintf(buffer, "%s", getFloat(s->buySum));
	if (strcmp(select, "pe1") == 0)
		return sprintf(buffer, "%s", getFloat(s->per1));
	if (strcmp(select, "pe2") == 0)
		return sprintf(buffer, "%s", getFloat(s->per2));
	if (strcmp(select, "iopv") == 0)
		return sprintf(buffer, "%s", getFloat(s->iopv));
	if (strcmp(select, "premium_rate") == 0)
		return sprintf(buffer, "%s", getFloat(s->premiumRate));
	if (strcmp(select, "up_limit") == 0)
		return sprintf(buffer, "%s", getFloat(s->upLimit));
	if (strcmp(select, "down_limit") == 0)
		return sprintf(buffer, "%s", getFloat(s->downLimit));
	if (strcmp(select, "position") == 0)
		return sprintf(buffer, "%s", getFloat(s->position));
	if (strcmp(select, "buy_sum_vol") == 0)
		return sprintf(buffer, "%s", getFloat(s->buySumVol));
	if (strcmp(select, "sell_num_vol") == 0)
		return sprintf(buffer, "%s", getFloat(s->sellSumVol));
	if (strcmp(select, "after_hours_buy_last_price") == 0)
		return sprintf(buffer, "%s", getFloat(s->afterHoursBuyPx[0]));
	if (strcmp(select, "after_hours_buy_avg_price") == 0)
		return sprintf(buffer, "%s", getFloat(s->afterHoursBuyPx[1]));
	if (strcmp(select, "after_hours_buy_last_size") == 0)
		return sprintf(buffer, "%s", getFloat(s->afterHoursBuySz[0]));
	if (strcmp(select, "after_hours_buy_avg_size") == 0)
		return sprintf(buffer, "%s", getFloat(s->afterHoursBuySz[1]));
	if (strcmp(select, "after_hours_sell_last_size") == 0)
		return sprintf(buffer, "%s", getFloat(s->afterHoursSellSz[0]));
	if (strcmp(select, "after_hours_sell_avg_size") == 0)
		return sprintf(buffer, "%s", getFloat(s->afterHoursSellSz[1]));

	return 0;
	*/
}
