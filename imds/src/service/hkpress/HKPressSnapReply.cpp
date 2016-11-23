/*
 * HKPressSnapReply.cpp
 *
 *  Created on: Sep 24, 2015
 *      Author: level2
 */

#include "HKPressSnapReply.h"
#include "../../public/market/Market.h"
#include "../../public/hkpress/HKPressSnap.h"

COL_DESC_ST HKPressSnapReply::HKPressSnapDesc[] =
{
    //colName,  dataType,   offset, plevel, structName, formatFunc, calcRule
    /* 日期 */
    { "date", DATATYPE_INT32, OFFSET(HKPSNAP, date), 0, NULL, default_format_func, NULL, NULL },
    /* 时间 */
    { "time", DATATYPE_INT32, OFFSET(HKPSNAP, time), 0, NULL, default_format_func, NULL, NULL },
    /* 交易所代码 */
    { "exch_code", DATATYPE_STR, OFFSET(HKPSNAP, exchangeCode), 0, NULL, default_format_func, NULL, NULL },
    /* 市场代码 */
    { "mktcode", DATATYPE_STR, OFFSET(HKPSNAP, marketCode), 0, NULL, default_format_func, NULL, NULL },
    /* 证券代码 */
    { "code", DATATYPE_STR, OFFSET(HKPSNAP, securityID), 0, NULL, default_format_func, NULL, NULL },
    /* 证券名称*/
    { "name", DATATYPE_STR, OFFSET(HKPSNAP, symbol), 0, NULL, default_format_func, NULL, NULL },
    /* 事务类型 */
    { "sectype", DATATYPE_CHAR, OFFSET(HKPSNAP, sectiontype), 0, NULL, default_format_func, NULL, NULL },
    /* 分时线时间 */
    { "line_time", DATATYPE_INT32, OFFSET(HKPSNAP, lineTime), 0, NULL, default_format_func, NULL, NULL },
    /* 位置 */
    { "pos", DATATYPE_INT32, OFFSET(HKPSNAP, pos), 0, NULL, default_format_func, NULL, NULL },
    /* 昨收 */
    { "prev_close", DATATYPE_INT64, OFFSET(HKPSNAP, prevClosePx), 0, NULL, default_format_func, calcRule[1], NULL },
    /* 开盘价 */
    { "open", DATATYPE_INT64, OFFSET(HKPSNAP, openPx), 0, NULL, default_format_func, calcRule[1], NULL },
    /* 最高价 */
    { "high", DATATYPE_INT64, OFFSET(HKPSNAP, highPx), 0, NULL, default_format_func, calcRule[1], NULL },
    /* 最低价 */
    { "low", DATATYPE_INT64, OFFSET(HKPSNAP, lowPx), 0, NULL, default_format_func, calcRule[1], NULL },
    /* 最新价 */
    { "last", DATATYPE_INT64, OFFSET(HKPSNAP, lastPx), 0, NULL, default_format_func, calcRule[1], NULL },
    /* 均价 */
    { "avg_px", DATATYPE_INT64, OFFSET(HKPSNAP, avgPx), 0, NULL, default_format_func, NULL, NULL },
    /* 成交量 */
    { "volume", DATATYPE_INT64, OFFSET(HKPSNAP, volume), 0, NULL, default_format_func, NULL, NULL },
    /* 成交金额 */
    { "amount", DATATYPE_INT64, OFFSET(HKPSNAP, amount), 0, NULL, default_format_func, NULL, NULL },
    /* 涨跌,需要计算字段*/
    { "change", DATATYPE_INT64, OFFSET(HKPSNAP, change), 0, NULL, NULL, NULL, NULL },
    /* 涨跌幅, 需要计算字段*/
    { "chg_rate", DATATYPE_INT64, OFFSET(HKPSNAP, amountOfIncrease), 0, NULL, NULL, NULL, NULL },
    /* 振幅, 需要计算字段 */
    { "amp_rate", DATATYPE_INT64, OFFSET(HKPSNAP, amplitude), 0, NULL, NULL, NULL, NULL },
    //利率
    { "int_rate", DATATYPE_INT64, OFFSET(HKPSNAP, interestRate), 0, NULL, default_format_func, NULL, NULL },
    /* 前成交量 */
    { "pre_vol", DATATYPE_INT64, OFFSET(HKPSNAP, prevVolume), 0, NULL, default_format_func, NULL, NULL },
    /* 前成交金额 */
    { "pre_amt", DATATYPE_INT64, OFFSET(HKPSNAP, prevAmount), 0, NULL, default_format_func, NULL, NULL },
    /* 前快照最高价 */
    { "pre_snp_high", DATATYPE_INT64, OFFSET(HKPSNAP, prevSnapHigh), 0, NULL, default_format_func, NULL, NULL },
    /* 前快照最低价 */
    { "pre_snp_low", DATATYPE_INT64, OFFSET(HKPSNAP, prevSnapLow), 0, NULL, default_format_func, NULL, NULL },
    /* 前分时线量 */
    { "pre_ln_vol", DATATYPE_INT64, OFFSET(HKPSNAP, prevLineVolume), 0, NULL, default_format_func, NULL, NULL },
    /* 前分时线金额 */
    { "pre_ln_amt", DATATYPE_INT64, OFFSET(HKPSNAP, prevLineAmount), 0, NULL, default_format_func, NULL, NULL },

//    /*证券代码源*/
//    { "code_source", DATATYPE_STR, OFFSET(HKPSNAP, securityIDSource), 0, NULL, default_format_func, NULL, NULL },
//    /*基础证券代码*/
//    { "underlying_code", DATATYPE_STR, OFFSET(HKPSNAP, underlyingSecurityID), 0, NULL, default_format_func, NULL, NULL },

    /*证券基本信息*/
    { "basicinfo", DATATYPE_STRUCT, OFFSET(HKPSNAP, basicinfo), 0, "BASIC_INFO", NULL, NULL, NULL },
    /*拼音*/
    { "pinyin", DATATYPE_STR, OFFSET(HKPSNAP, basicinfo.phoneticize), 0, NULL, default_format_func, NULL, NULL },
    /*英语简称*/
    { "ename", DATATYPE_STR, OFFSET(HKPSNAP, basicinfo.englishName), 0, NULL, default_format_func, NULL, NULL },
    /*isin*/
    { "isin", DATATYPE_STR, OFFSET(HKPSNAP, basicinfo.isin), 0, NULL, default_format_func, NULL, NULL },
    /*证券类型*/
    { "type", DATATYPE_STR, OFFSET(HKPSNAP, basicinfo.securityType), 0, NULL, default_format_func, NULL, NULL },
    /*证券类型*/
    { "subtype", DATATYPE_STR, OFFSET(HKPSNAP, basicinfo.securitySubType), 0, NULL, default_format_func, NULL, NULL },
    /*行业代码*/
    { "indscode", DATATYPE_STR, OFFSET(HKPSNAP, basicinfo.industyCode), 0, NULL, default_format_func, NULL, NULL },
    /*地区代码*/
    { "rgncode", DATATYPE_STR, OFFSET(HKPSNAP, basicinfo.regionCode), 0, NULL, default_format_func, NULL, NULL },
    /*交易所板块*/
    { "exch_sector", DATATYPE_STR, OFFSET(HKPSNAP, basicinfo.exchangeBoard), 0, NULL, default_format_func, NULL, NULL },
    /*自选股板块*/
    { "self_sector", DATATYPE_STR, OFFSET(HKPSNAP, basicinfo.selfBoard), 0, NULL, default_format_func, NULL, NULL },
    /*货币*/
    { "ccy", DATATYPE_STR, OFFSET(HKPSNAP, basicinfo.currency), 0, NULL, default_format_func, NULL, NULL },

    { "currency", DATATYPE_STR, OFFSET(HKPSNAP, basicinfo.currency), 0, NULL, default_format_func, NULL, NULL },
    { "securityType", DATATYPE_STR, OFFSET(HKPSNAP, basicinfo.securityType), 0, NULL, default_format_func, NULL, NULL },

    /*英文证券全称*/
    { "efullname", DATATYPE_STR, OFFSET(HKPSNAP, securityDesc), 0, NULL, default_format_func, NULL, NULL },
    /*辅助证券代码*/
    { "underlyingsecurityid", DATATYPE_STR, OFFSET(HKPSNAP, underlyingSecurityID), 0, NULL, default_format_func, NULL, NULL },
    /*市场种类*/
    { "market", DATATYPE_STR, OFFSET(HKPSNAP, marketID), 0, NULL, default_format_func, NULL, NULL },
    /*AmountTimes*/
    { "amountTimes", DATATYPE_INT32, OFFSET(HKPSNAP, amountTimes), 0, NULL, default_format_func, NULL, NULL },
    /*面值*/
    { "perValue", DATATYPE_INT64, OFFSET(HKPSNAP, perValue), 0, NULL, default_format_func, NULL, NULL },
    /*面值货币种类*/
    { "perValueCurrency", DATATYPE_STR, OFFSET(HKPSNAP, perValueCurrency), 0, NULL, default_format_func, NULL, NULL },
    /*利息*/
    { "interest", DATATYPE_INT64, OFFSET(HKPSNAP, interest), 0, NULL, default_format_func, NULL, NULL },
    /*上市日期*/
    { "issueDate", DATATYPE_INT32, OFFSET(HKPSNAP, issueDate), 0, NULL, default_format_func, NULL, NULL },
    /*买卖单位*/
    { "roundLot", DATATYPE_INT32, OFFSET(HKPSNAP, roundLot), 0, NULL, default_format_func, NULL, NULL },
    /*备注*/
    { "text", DATATYPE_STR, OFFSET(HKPSNAP, text), 0, NULL, default_format_func, NULL, NULL },
    /*产品状态信息*/
    { "securityStatusFlag", DATATYPE_STR, OFFSET(HKPSNAP, securityStatusFlag), 0, NULL, default_format_func, NULL, NULL },
    /*按盘价*/
    { "nominal", DATATYPE_INT64, OFFSET(HKPSNAP, nominalpx), 0, NULL, default_format_func,  calcRule[1], NULL },
    /*bid*/
    { "bid", DATATYPE_STRUCT, OFFSET(HKPSNAP, bid), 0, "LEVEL", NULL, NULL, NULL },
    { "bidpx1", DATATYPE_INT64, OFFSET(HKPSNAP, bid.price), 0, NULL, default_format_func, calcRule[1], NULL },
    /*offer*/
    { "offer", DATATYPE_STRUCT, OFFSET(HKPSNAP, offer), 0, "LEVEL", NULL, NULL, NULL },
    { "askpx1", DATATYPE_INT64, OFFSET(HKPSNAP, offer.price), 0, NULL, default_format_func, calcRule[1], NULL },

    /*证券交易状态*/
    { "tradingStatus", DATATYPE_STR, OFFSET(HKPSNAP, secTradingStatus), 0, NULL, default_format_func, NULL, NULL },
    /*行情时间*/
    { "mdtime", DATATYPE_INT32, OFFSET(HKPSNAP, MDTime), 0, NULL, default_format_func, NULL, NULL },
    /*证券交易状态*/
    { "tradingStatus1", DATATYPE_STR, OFFSET(HKPSNAP, secTradingStatus1), 0, NULL, default_format_func, NULL, NULL },
    /*证券交易状态*/
    { "tradingStatus2", DATATYPE_STR, OFFSET(HKPSNAP, secTradingStatus2), 0, NULL, default_format_func, NULL, NULL },
    /*vcm*/
    { "vcm", DATATYPE_STRUCT, OFFSET(HKPSNAP, vcm), 0, "VCM", NULL, NULL, NULL },
    /*市调机制开始时间*/
    { "VCMStartTime", DATATYPE_INT32, OFFSET(HKPSNAP, vcm.VCMStartTime), 0, NULL, default_format_func, NULL, NULL },
    /*市调机制结束时间*/
    { "VCMEndTime", DATATYPE_INT32, OFFSET(HKPSNAP, vcm.VCMEndTime), 0, NULL, default_format_func, NULL, NULL },
    /*市调机制参考价*/
    { "VCMRefPx", DATATYPE_INT64, OFFSET(HKPSNAP, vcm.VCMRefPrice), 0, NULL, default_format_func, calcRule[1], NULL },
    /*市调机制下限价*/
    { "VCMLowerPx", DATATYPE_INT64, OFFSET(HKPSNAP, vcm.VCMLowerPrice), 0, NULL, default_format_func, calcRule[1], NULL },
    /*市调机制上限价*/
    { "VCMUpperPx", DATATYPE_INT64, OFFSET(HKPSNAP, vcm.VCMUpperPrice), 0, NULL, default_format_func, calcRule[1], NULL },
    /*行情时间*/
    { "VCMTimestamp", DATATYPE_INT32, OFFSET(HKPSNAP, vcm.VCMTimestamp), 0, NULL, default_format_func, NULL, NULL },

    /*cas*/
    { "cas", DATATYPE_STRUCT, OFFSET(HKPSNAP, cas), 0, "CAS", NULL, NULL, NULL },
    /*收盘集合竞价时段参考价*/
    { "CASRefPx", DATATYPE_INT64, OFFSET(HKPSNAP, cas.CASRefPrice), 0, NULL, default_format_func, calcRule[1], NULL },
    /*收盘集合竞价时段下限价*/
    { "CASLowerPx", DATATYPE_INT64, OFFSET(HKPSNAP, cas.CASLowerPrice), 0, NULL, default_format_func, calcRule[1], NULL },
    /*收盘集合竞价时段上限价*/
    { "CASUpperPx", DATATYPE_INT64, OFFSET(HKPSNAP, cas.CASUpperPrice), 0, NULL, default_format_func, calcRule[1], NULL },
    /*不能配对买卖盘方向*/
    { "OrdlmbDirection", DATATYPE_STR, OFFSET(HKPSNAP, cas.OrdlmbDirection), 0, NULL, default_format_func, NULL, NULL },
    /*不能配对买卖盘量*/
    { "OrdlmbQty", DATATYPE_INT64, OFFSET(HKPSNAP, cas.OrdlmbQty), 0, NULL, default_format_func, NULL, NULL },
    /*行情时间*/
    { "CASTimestamp", DATATYPE_INT32, OFFSET(HKPSNAP, cas.CASTimestamp), 0, NULL, default_format_func, NULL, NULL },

    /*close*/
    { "close", DATATYPE_BOOL, OFFSET(HKPSNAP, close), 0, NULL, default_format_func, NULL, NULL },
    /*hand*/
    { "now_vol", DATATYPE_INT64, OFFSET(HKPSNAP, hand), 0, NULL, default_format_func, NULL, NULL },
    /*in*/
    { "sell_vol", DATATYPE_INT64, OFFSET(HKPSNAP, in), 0, NULL, default_format_func, NULL, NULL },
    /*out*/
    { "buy_vol", DATATYPE_INT64, OFFSET(HKPSNAP, out), 0, NULL, default_format_func, NULL, NULL },
    /*out*/
    { "pin", DATATYPE_INT64, OFFSET(HKPSNAP, pin), 0, NULL, default_format_func, NULL, NULL },
};

HKPressSnapReply::HKPressSnapReply() {
	// TODO Auto-generated constructor stub

}

HKPressSnapReply::~HKPressSnapReply() {
	// TODO Auto-generated destructor stub
    if (this->hTblColDes)
    {
        HTableFree(this->hTblColDes, NULL);
        this->hTblColDes = NULL;
    }
}

void HKPressSnapReply::registerCol()
{
    if (this->hTblColDes == NULL)
    {
        HTableCreate(&this->hTblColDes);
        int n = sizeof(HKPressSnapDesc) / sizeof(COL_DESC_ST);
        for (int i = 0; i < n; i++)
        {
            COL_DESC_ST *description = &HKPressSnapDesc[i];
            HTableEnter(this->hTblColDes, description->key,
                    (char *) description, NULL);
        }
    }
}

int HKPressSnapReply::request(const char* script,const char* query,char* buffer,int& len)
{
	if (!SnapReply::parse(script,query))
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	registerCol();

	snap=market->getSnap(code.c_str());
	if (!snap)
	{
		if (code.compare("00000")==0)
			snap=market->getSnap(0);
		if (!snap)
			return HttpReply::replyError(HttpStatus::NotFound,buffer,len);
	}
	HKPSNAP* s=(HKPSNAP*)snap;
	date=s->date;
	time=s->time;

	HttpReply::parseSelect("name,prev_close,open,high,low,last,change,chg_rate,volume,amount,now_vol,buy_vol,sell_vol,bid,ask");

	if (paramSelect.empty())
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	return SnapReply::reply(buffer,len);
}

int HKPressSnapReply::getSnapString(char* buffer,const char* select)
{
	HKPSNAP* s=(HKPSNAP*)snap;
    if (strcmp(select, "change") == 0)
    {
        if (s->lastPx != 0)
            return sprintf(buffer, "%s",
                    getHKFloat(static_cast<int64_t>(s->lastPx - s->prevClosePx)));
        else
            return sprintf(buffer, "0.000");
    }
    if (strcmp(select, "chg_rate") == 0)
    {
        if (s->prevClosePx != 0 && s->lastPx > 0)
            return sprintf(buffer, "%.2f",
                    static_cast<int64_t>((s->lastPx - s->prevClosePx)) * 100.0
                            / s->prevClosePx);
        else
            return sprintf(buffer, "0.00");
    }
    if (strcmp(select, "amp_rate") == 0)
    {
        if (s->prevClosePx != 0 && s->lastPx > 0)
            return sprintf(buffer, "%.2f",
                    static_cast<int64_t>((s->highPx - s->lowPx)) * 100.0 / s->prevClosePx);
        else
            return sprintf(buffer, "0.00");
    }

    int slen = 0;
    if (strcmp(select, "bid") == 0)
    {
        slen = sprintf(buffer + slen, "[");
        for (int i = 0; i < 1; ++i)
        {
            if (i > 0)
                slen += sprintf(buffer + slen, ",");
            slen += sprintf(buffer + slen, "%s,%ld", getHKFloat(s->bid.price),
                    s->bid.size);
        }
        slen += sprintf(buffer + slen, "]");
        return slen;
    }
    if (strcmp(select, "ask") == 0)
    {
        slen = sprintf(buffer + slen, "[");
        for (int i = 0; i < 1; ++i)
        {
            if (i > 0)
                slen += sprintf(buffer + slen, ",");
            slen += sprintf(buffer + slen, "%s,%ld", getHKFloat(s->offer.price),
                    s->offer.size);
        }
        slen += sprintf(buffer + slen, "]");
        return slen;
    }

    if (strcmp(select, "perValue") == 0)
        return sprintf(buffer, "%lf", s->perValue / 100000000.0);
    if (strcmp(select, "perValueCurrency") == 0)
        return sprintf(buffer, "\"%s\"", s->perValueCurrency);
    if (strcmp(select, "interest") == 0)
        return sprintf(buffer, "%lf", s->interest / 100000000.0);

    return getString( buffer, select, snap);

    /*
    if (strcmp(select,"code")==0)
        return sprintf(buffer,"\"%s\"",s->securityID);
    if (strcmp(select,"name")==0)
        return sprintf(buffer,"\"%s\"",s->symbol);
    if (strcmp(select,"prev_close")==0)
        return sprintf(buffer,"%s",getHKFloat(s->prevClosePx));
    if (strcmp(select,"open")==0)
        return sprintf(buffer,"%s",getHKFloat(s->openPx));
    if (strcmp(select,"high")==0)
        return sprintf(buffer,"%s",getHKFloat(s->highPx));
    if (strcmp(select,"low")==0)
        return sprintf(buffer,"%s",getHKFloat(s->lowPx));
    if (strcmp(select,"last")==0)
        return sprintf(buffer,"%s",getHKFloat(s->lastPx));
    if (strcmp(select,"nominal")==0)
        return sprintf(buffer,"%s",getHKFloat(s->nominalpx));
    if (strcmp(select,"avg_px")==0)
        return sprintf(buffer,"%s",getHKFloat(s->avgPx));
    if (strcmp(select,"volume")==0)
    {
        return sprintf(buffer,"%ld",s->volume);
    }
    if (strcmp(select,"amount")==0)
    {
        return sprintf(buffer,"%ld", s->amount);
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
	if (strcmp(select, "tradingStatus") == 0)
		return sprintf(buffer, "\"%s\"", s->secTradingStatus);
	if (strcmp(select, "tradingStatus1") == 0)
		return sprintf(buffer, "\"%s\"", s->secTradingStatus1);
	if (strcmp(select, "tradingStatus2") == 0)
		return sprintf(buffer, "\"%s\"", s->secTradingStatus2);
	if (strcmp(select, "isin") == 0)
		return sprintf(buffer, "\"%s\"", s->basicinfo.isin);
	if (strcmp(select, "ename") == 0)
		return sprintf(buffer, "\"%s\"", s->securityDesc);
	if (strcmp(select, "efullname") == 0)
		return sprintf(buffer, "\"%s\"", s->securityDesc);
	if (strcmp(select, "underlyingSecurityID") == 0)
		return sprintf(buffer, "\"%s\"", s->underlyingSecurityID);
	if (strcmp(select, "market") == 0)
		return sprintf(buffer, "\"%s\"", s->marketID);
	if (strcmp(select, "securityType") == 0)
		return sprintf(buffer, "\"%s\"", s->basicinfo.securityType);
	if (strcmp(select, "currency") == 0)
		return sprintf(buffer, "\"%s\"", s->basicinfo.currency);
	if (strcmp(select, "amountTimes") == 0)
		return sprintf(buffer, "%d", s->amountTimes);

	if (strcmp(select, "issueDate") == 0)
		return sprintf(buffer, "%d", s->issueDate);
	if (strcmp(select, "roundLot") == 0)
		return sprintf(buffer, "%d", s->roundLot);
	if (strcmp(select, "securityStatusFlag") == 0)
		return sprintf(buffer, "\"%s\"", s->securityStatusFlag);
	if (strcmp(select, "text") == 0)
		return sprintf(buffer, "\"%s\"", s->text);
	//vcm
	if (strcmp(select, "VCMStartTime") == 0)
		return sprintf(buffer, "%d", s->vcm.VCMStartTime);
	if (strcmp(select, "VCMEndTime") == 0)
		return sprintf(buffer, "%d", s->vcm.VCMEndTime);
	if (strcmp(select, "VCMRefPx") == 0)
		return sprintf(buffer, "%s", getHKFloat(s->vcm.VCMRefPrice));
	if (strcmp(select, "VCMLowerPx") == 0)
		return sprintf(buffer, "%s", getHKFloat(s->vcm.VCMLowerPrice));
	if (strcmp(select, "VCMUpperPx") == 0)
		return sprintf(buffer, "%s", getHKFloat(s->vcm.VCMUpperPrice));
	if (strcmp(select, "VCMTimestamp") == 0)
		return sprintf(buffer, "%d", s->vcm.VCMTimestamp);
	//cas
	if (strcmp(select, "CASRefPx") == 0)
		return sprintf(buffer, "%s", getHKFloat(s->cas.CASRefPrice));
	if (strcmp(select, "CASLowerPx") == 0)
		return sprintf(buffer, "%s", getHKFloat(s->cas.CASLowerPrice));
	if (strcmp(select, "CASUpperPx") == 0)
		return sprintf(buffer, "%s", getHKFloat(s->cas.CASUpperPrice));
	if (strcmp(select, "OrdlmbDirection") == 0)
		return sprintf(buffer, "\"%s\"", s->cas.OrdlmbDirection);
	if (strcmp(select, "OrdlmbQty") == 0)
		return sprintf(buffer, "%ld", s->cas.OrdlmbQty);
	if (strcmp(select, "CASTimestamp") == 0)
		return sprintf(buffer, "%d", s->cas.CASTimestamp);
	return 0;
	*/
}

