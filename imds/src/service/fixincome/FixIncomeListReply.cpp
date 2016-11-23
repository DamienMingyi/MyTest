/*
 * FixIncomeListReply.cpp
 *
 *  Created on: Aug 31, 2015
 *      Author: level2
 */

#include "FixIncomeListReply.h"
#include "../../public/fixincome/FixIncomeSnap.h"

COL_DESC_ST FixIncomeListReply::fixIncomeListDesc[] =
{
    //colName,  dataType,   offset, plevel, structName, formatFunc, calcRule
    /* 日期 */
    { "date", DATATYPE_INT32, OFFSET(SHFISNAP, date), 0, NULL, default_format_func, NULL, NULL },
    /* 时间 */
    { "time", DATATYPE_INT32, OFFSET(SHFISNAP, time), 0, NULL, default_format_func, NULL, NULL },
    /* 交易所代码 */
    { "exch_code", DATATYPE_STR, OFFSET(SHFISNAP, exchangeCode), 0, NULL, default_format_func, NULL, NULL },
    /* 市场代码 */
    { "mktcode", DATATYPE_STR, OFFSET(SHFISNAP, marketCode), 0, NULL, default_format_func, NULL, NULL },
    /* 证券代码 */
    { "code", DATATYPE_STR, OFFSET(SHFISNAP, securityID), 0, NULL, default_format_func, NULL, NULL },
    /* 证券名称*/
    { "name", DATATYPE_STR, OFFSET(SHFISNAP, symbol), 0, NULL, default_format_func, NULL, NULL },
    /* 事务类型 */
    { "sectype", DATATYPE_CHAR, OFFSET(SHFISNAP, sectiontype), 0, NULL, default_format_func, NULL, NULL },
    /* 分时线时间 */
    { "line_time", DATATYPE_INT32, OFFSET(SHFISNAP, lineTime), 0, NULL, default_format_func, NULL, NULL },
    /* 位置 */
    { "pos", DATATYPE_INT32, OFFSET(SHFISNAP, pos), 0, NULL, default_format_func, NULL, NULL },
    /* 昨收 */
    { "prev_close", DATATYPE_INT64, OFFSET(SHFISNAP, prevClosePx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 开盘价 */
    { "open", DATATYPE_INT64, OFFSET(SHFISNAP, openPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 最高价 */
    { "high", DATATYPE_INT64, OFFSET(SHFISNAP, highPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 最低价 */
    { "low", DATATYPE_INT64, OFFSET(SHFISNAP, lowPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 最新价 */
    { "last", DATATYPE_INT64, OFFSET(SHFISNAP, lastPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 均价 */
    { "avg_px", DATATYPE_INT64, OFFSET(SHFISNAP, avgPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 成交量 */
    { "volume", DATATYPE_INT64, OFFSET(SHFISNAP, volume), 0, NULL, default_format_func, NULL, NULL },
    /* 成交金额 */
    { "amount", DATATYPE_INT64, OFFSET(SHFISNAP, amount), 0, NULL, default_format_func, NULL, NULL },
    /* 涨跌,需要计算字段*/
    { "change", DATATYPE_INT64, OFFSET(SHFISNAP, change), 0, NULL, NULL, NULL, NULL },
    /* 涨跌幅, 需要计算字段*/
    { "chg_rate", DATATYPE_INT64, OFFSET(SHFISNAP, amountOfIncrease), 0, NULL, NULL, NULL, NULL },
    /* 振幅, 需要计算字段 */
    { "amp_rate", DATATYPE_INT64, OFFSET(SHFISNAP, amplitude), 0, NULL, NULL, NULL, NULL },
    //利率
    { "int_rate", DATATYPE_INT64, OFFSET(SHFISNAP, interestRate), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 前成交量 */
    { "pre_vol", DATATYPE_INT64, OFFSET(SHFISNAP, prevVolume), 0, NULL, default_format_func, NULL, NULL },
    /* 前成交金额 */
    { "pre_amt", DATATYPE_INT64, OFFSET(SHFISNAP, prevAmount), 0, NULL, default_format_func, NULL, NULL },
    /* 前快照最高价 */
    { "pre_snp_high", DATATYPE_INT64, OFFSET(SHFISNAP, prevSnapHigh), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 前快照最低价 */
    { "pre_snp_low", DATATYPE_INT64, OFFSET(SHFISNAP, prevSnapLow), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 前分时线量 */
    { "pre_ln_vol", DATATYPE_INT64, OFFSET(SHFISNAP, prevLineVolume), 0, NULL, default_format_func, NULL, NULL },
    /* 前分时线金额 */
    { "pre_ln_amt", DATATYPE_INT64, OFFSET(SHFISNAP, prevLineAmount), 0, NULL, default_format_func, NULL, NULL },

    /* 成交时间 */
    { "trade_time", DATATYPE_INT32, OFFSET(SHFISNAP, tradeTime), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "now_vol", DATATYPE_INT64, OFFSET(SHFISNAP, hand), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "sell_vol", DATATYPE_INT64, OFFSET(SHFISNAP, in), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "buy_vol", DATATYPE_INT64, OFFSET(SHFISNAP, out), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "pin", DATATYPE_INT32, OFFSET(SHFISNAP, pin), 0, NULL, default_format_func, NULL, NULL },

    //static
    /* */
    { "product", DATATYPE_STR, OFFSET(SHFISNAP, tradeProduct), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "attribute", DATATYPE_STR, OFFSET(SHFISNAP, attribute), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "status", DATATYPE_STR, OFFSET(SHFISNAP, status), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "pledge", DATATYPE_STR, OFFSET(SHFISNAP, pledgeCode), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "openTime", DATATYPE_INT32, OFFSET(SHFISNAP, openTime), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "closeTime", DATATYPE_INT32, OFFSET(SHFISNAP, closeTime), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "offeringWay", DATATYPE_STR, OFFSET(SHFISNAP, offeringWay), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "denomination", DATATYPE_INT32, OFFSET(SHFISNAP, denomination), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "offeringPx", DATATYPE_INT64, OFFSET(SHFISNAP, offeringPx), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "interestType", DATATYPE_STR, OFFSET(SHFISNAP, interestType), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "payingFrequence", DATATYPE_STR, OFFSET(SHFISNAP, payingFrequence), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "nomial", DATATYPE_INT32, OFFSET(SHFISNAP, nomialInterstRate), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "basicInterestRate", DATATYPE_INT32, OFFSET(SHFISNAP, basicInterestRate), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "basicInterestMargin", DATATYPE_INT32, OFFSET(SHFISNAP, basicInterestMargin), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "deadline", DATATYPE_INT32, OFFSET(SHFISNAP, deadline), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "offeringVolume", DATATYPE_INT32, OFFSET(SHFISNAP, offeringVolume), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "offeringBeginDate", DATATYPE_INT32, OFFSET(SHFISNAP, offeringBeginDate), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "offeringEndDate", DATATYPE_INT32, OFFSET(SHFISNAP, offeringEndDate), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "listDate", DATATYPE_INT32, OFFSET(SHFISNAP, listingDate), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "expireDate", DATATYPE_INT32, OFFSET(SHFISNAP, expiringDate), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "bondType", DATATYPE_STR, OFFSET(SHFISNAP, bondType), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "offeringMethod", DATATYPE_STR, OFFSET(SHFISNAP, offeringMethod), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "transMarket", DATATYPE_STR, OFFSET(SHFISNAP, transMarket), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "short", DATATYPE_STR, OFFSET(SHFISNAP, sellShort), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "shortLimit", DATATYPE_INT32, OFFSET(SHFISNAP, totalShortLimit), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "traderShortLimit", DATATYPE_INT32, OFFSET(SHFISNAP, traderShortLimit), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "preWeightedAvgPx", DATATYPE_INT32, OFFSET(SHFISNAP, preWeightedAvgPx), 0, NULL, default_format_func, calcRule[0], NULL },

    //real time
    /* */
    { "imageStatus", DATATYPE_INT32, OFFSET(SHFISNAP, imageStatus), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "weightedAvgPx", DATATYPE_INT32, OFFSET(SHFISNAP, weightedAvgPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 成交笔数*/
    { "num_trades", DATATYPE_INT64, OFFSET(SHFISNAP,totalNumTrades), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "preCloseYield", DATATYPE_INT32, OFFSET(SHFISNAP, preCloseYield), 0, NULL, default_format_func, calcRule[0], calcParams[4] },
    /* */
    { "preWeightedAvgYield", DATATYPE_INT32, OFFSET(SHFISNAP, preWeightedAvgYield), 0, NULL, default_format_func, calcRule[0], calcParams[4] },
    /* */
    { "openYield", DATATYPE_INT32, OFFSET(SHFISNAP, openYield), 0, NULL, default_format_func, calcRule[0], calcParams[4] },
    /* */
    { "highYield", DATATYPE_INT32, OFFSET(SHFISNAP, highYield), 0, NULL, default_format_func, calcRule[0], calcParams[4] },
    /* */
    { "lowYield", DATATYPE_INT32, OFFSET(SHFISNAP, lowYield), 0, NULL, default_format_func, calcRule[0], calcParams[4] },
    /* */
    { "lastYield", DATATYPE_INT32, OFFSET(SHFISNAP, lastYield), 0, NULL, default_format_func, calcRule[0], calcParams[4] },
    /* */
    { "weightedAvgYield", DATATYPE_INT32, OFFSET(SHFISNAP, weightedAvgYield), 0, NULL, default_format_func, calcRule[0], calcParams[4] },
    /* */
    { "accuredInterestAmt", DATATYPE_INT32, OFFSET(SHFISNAP, accruedInterestAmt), 0, NULL, default_format_func, calcRule[0], calcParams[4] },

    { "bid", DATATYPE_STRUCT, OFFSET(SHFISNAP, bid), 0, "LEVEL", NULL, NULL, NULL },
    { "ask", DATATYPE_STRUCT, OFFSET(SHFISNAP, offer), 0, "LEVEL", NULL, NULL, NULL },
    { "bidpx1", DATATYPE_INT64, OFFSET(SHFISNAP, bid[0].netPx), 0, NULL, default_format_func, calcRule[0], NULL },
    { "askpx1", DATATYPE_INT64, OFFSET(SHFISNAP, offer[0].netPx), 0, NULL, default_format_func, calcRule[0], NULL },

    /* */
    { "tradeNetPrice", DATATYPE_INT32, OFFSET(SHFISNAP, tradeNetPrice), 0, NULL, default_format_func, calcRule[0], NULL },
    /* */
    { "accurateInterestAmt", DATATYPE_INT32, OFFSET(SHFISNAP, accurateInterestAmt), 0, NULL, default_format_func, calcRule[0], NULL },
    /* */
    { "tradeFullPrice", DATATYPE_INT32, OFFSET(SHFISNAP, tradeFullPrice), 0, NULL, default_format_func, calcRule[0], NULL },
    /* */
    { "yield2Marturity", DATATYPE_INT32, OFFSET(SHFISNAP, yield2Marturity), 0, NULL, default_format_func, calcRule[0], NULL },
    /* */
    { "accurateInterestAmt", DATATYPE_INT32, OFFSET(SHFISNAP, tradeQty), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 成交量 */
    { "trade_vol", DATATYPE_INT64, OFFSET(SHFISNAP, tradeQty), 0, NULL, default_format_func, NULL, NULL },
    /* 成交金额 */
    { "trade_amt", DATATYPE_INT64, OFFSET(SHFISNAP, tradeMoney), 0, NULL, default_format_func, NULL, NULL },
    /* */
    { "trade_mthd", DATATYPE_STR, OFFSET(SHFISNAP, tradeMethod), 0, NULL, default_format_func, NULL, NULL },
    /* 丢包判断 */
    { "corrupted", DATATYPE_BOOL, OFFSET(SHFISNAP, noPacketdrop), 0, NULL, default_format_func, NULL, NULL },
};


FixIncomeListReply::FixIncomeListReply()
{
    // TODO Auto-generated constructor stub

}

FixIncomeListReply::~FixIncomeListReply()
{
    // TODO Auto-generated destructor stub
    if (this->hTblColDes)
    {
        HTableFree(this->hTblColDes, NULL);
        this->hTblColDes = NULL;
    }
}

void FixIncomeListReply::registerCol()
{
    if (this->hTblColDes == NULL)
    {
        HTableCreate(&this->hTblColDes);
        int n = sizeof(fixIncomeListDesc) / sizeof(COL_DESC_ST);
        for (int i = 0; i < n; i++)
        {
            COL_DESC_ST *description = &fixIncomeListDesc[i];
            HTableEnter(this->hTblColDes, description->key,
                    (char *) description, NULL);
        }
    }
}

int FixIncomeListReply::request(const char* script,const char* query,char* buffer,int& len)
{
	if (!ListReply::parse(script,query))
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	HttpReply::parseSelect("code,name,prev_close");

	date=((SHFISNAP*)list[0].snap)->date;
	time=((SHFISNAP*)list[0].snap)->time;
	return ListReply::reply(buffer,len);
}

bool FixIncomeListReply::getSelfList()
{
	list.clear();
	ListCmp cmp;
	if ((order.find("ASE")!=string::npos) || (order.find("ase")!=string::npos))
		cmp.order=false;
	else
		cmp.order=true;
	for (unsigned int i=0;i<self.size();++i)
	{
		cmp.snap=((Market*)(this->market))->getSnap(self[i].c_str());
		if (cmp.snap)
		{
			getOrder(cmp);
			list.push_back(cmp);
		}
	}
	return !list.empty();
}

bool FixIncomeListReply::getTypeList()
{
	list.clear();
	ListCmp cmp;
	if ((order.find("ASE")!=string::npos) || (order.find("ase")!=string::npos))
		cmp.order=false;
	else
		cmp.order=true;
	if (listType.find("exchange")!=string::npos)
	{
		int st=((Market*)(this->market))->getSnapCount();
		for (int i=1;i<st;++i)
		{
			cmp.snap=this->market->getSnap(i);
			if (cmp.snap==NULL)
				break;
			bool isList=false;
			if (listSubType.find("all")!=string::npos)
			{
				isList=true;
			}
			else
				isList=isExchangeSubType(listSubType, cmp.snap);
			if (isList)
			{
				getOrder(cmp);
				list.push_back(cmp);
			}
		}
	}
	return !list.empty();
}

bool FixIncomeListReply::isExchangeSubType(string& subType,char* snap)
{
    SHFISNAP* s=(SHFISNAP*)snap;
	if (listSubType.find("treasury_cash")!=string::npos)
		return strcmp(s->tradeProduct, "01") == 0;
	if (listSubType.find("corporate_cash")!=string::npos)
		return strcmp(s->tradeProduct, "03") == 0;
	if (listSubType.find("repo")!=string::npos)
		return strcmp(s->tradeProduct, "05") == 0;
	if (listSubType.find("psme_bonds")!=string::npos)
		return strcmp(s->tradeProduct, "06") == 0;
	if (listSubType.find("treasury")!=string::npos)
		return strcmp(s->attribute, "N") == 0;
	if (listSubType.find("corporate")!=string::npos)
		return strcmp(s->attribute, "E") == 0;
	if (listSubType.find("inscribed")!=string::npos)
		return strcmp(s->attribute, "A") == 0;
	if (listSubType.find("physical")!=string::npos)
		return strcmp(s->attribute, "R") == 0;
	if (listSubType.find("certificate")!=string::npos)
		return strcmp(s->attribute, "V") == 0;
	return false;
}


bool FixIncomeListReply::getOrder(ListCmp& cmp)
{
    SHFISNAP* s = (SHFISNAP*) cmp.snap;
    if (!order.empty())
    {
        if (order.find("change") != string::npos)
        {
            cmp.type = 1;
            if (s->lastPx > 0)
                cmp.iCmp = static_cast<int32_t>(s->lastPx - s->prevClosePx);
            else
                cmp.iCmp = 0;
            return true;
        }
        else if (order.find("chg_rate") != string::npos)
        {
            cmp.type = 3;
            if (s->prevClosePx != 0 && s->lastPx > 0)
                cmp.dCmp = static_cast<int32_t>((s->lastPx - s->prevClosePx)) * 100.0
                        / s->prevClosePx;
            else
                cmp.dCmp = 0;
            return true;
        }
        else if (order.find("amp_rate") != string::npos)
        {
            cmp.type = 3;
            if (s->prevClosePx != 0 && s->lastPx > 0)
                cmp.dCmp = static_cast<int32_t>((s->highPx - s->lowPx)) * 100.0
                        / s->prevClosePx;
            else
                cmp.dCmp = 0;
            return true;
        }
        int n = sizeof(fixIncomeListDesc) / sizeof(COL_DESC_ST);
        for (int i = 0; i < n; i++)
        {
            COL_DESC_ST *description = &fixIncomeListDesc[i];
            if (order.find(description->key) != string::npos)
            {
                return setOrder((char *)description, cmp.snap, cmp);
            }
        }
        /*
        if (order.find("code") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->securityID);
            return true;
        }
        else if (order.find("name") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->symbol);
            return true;
        }
        else if (order.find("prev_close") != string::npos)
        {
            cmp.type = 1;
            cmp.iCmp = s->prevClosePx;
            return true;
        }
        else if (order.find("open") != string::npos)
        {
            cmp.type = 1;
            cmp.iCmp = s->openPx;
            return true;
        }
        else if (order.find("high") != string::npos)
        {
            cmp.type = 1;
            cmp.iCmp = s->highPx;
            return true;
        }
        else if (order.find("low") != string::npos)
        {
            cmp.type = 1;
            cmp.iCmp = s->lowPx;
            return true;
        }
        else if (order.find("last") != string::npos)
        {
            cmp.type = 1;
            cmp.iCmp = s->lastPx;
            return true;
        }
        else if (order.find("change") != string::npos)
        {
            cmp.type = 1;
            if (s->lastPx > 0)
                cmp.iCmp = static_cast<int32_t>(s->lastPx - s->prevClosePx);
            else
                cmp.iCmp = 0;
            return true;
        }
        else if (order.find("chg_rate") != string::npos)
        {
            cmp.type = 3;
            if (s->prevClosePx != 0 && s->lastPx > 0)
                cmp.dCmp = static_cast<int32_t>((s->lastPx - s->prevClosePx)) * 100.0
                        / s->prevClosePx;
            else
                cmp.dCmp = 0;
            return true;
        }
        else if (order.find("amp_rate") != string::npos)
        {
            cmp.type = 3;
            if (s->prevClosePx != 0 && s->lastPx > 0)
                cmp.dCmp = static_cast<int32_t>((s->highPx - s->lowPx)) * 100.0
                        / s->prevClosePx;
            else
                cmp.dCmp = 0;
            return true;
        }
        else if (order.find("volume") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->volume;
            return true;
        }
        else if (order.find("amount") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->amount;
            return true;
        }
        else if (order.find("bidpx1") != string::npos)
        {
            cmp.type = 1;
            cmp.lCmp = s->bid[0].netPx;
            return true;
        }
        else if (order.find("askpx1") != string::npos)
        {
            cmp.type = 1;
            cmp.lCmp = s->offer[0].netPx;
            return true;
        }
        else if (order.find("now_vol") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->hand;
            return true;
        }
        else if (order.find("buy_vol") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->out;
            return true;
        }
        else if (order.find("sell_vol") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->in;
            return true;
        }
        */
    }
    order.clear();
    return false;
}

int FixIncomeListReply::getSnapString(char* buffer,const char* select)
{
    SHFISNAP* s = (SHFISNAP*) curSnap;
    if (strcmp(select, "change") == 0)
    {
        if (s->lastPx > 0)
        {
            return sprintf(buffer, "%s",
                    getFloat(static_cast<int32_t>(s->lastPx - s->prevClosePx)));
        }
        else
            return sprintf(buffer, "0.000");
    }
    if (strcmp(select, "chg_rate") == 0)
    {
        if (s->prevClosePx != 0 && s->lastPx > 0)
            return sprintf(buffer, "%.2f",
                    static_cast<int32_t>((s->lastPx - s->prevClosePx)) * 100.0
                            / s->prevClosePx);
        else
            return sprintf(buffer, "0.00");
    }
    if (strcmp(select, "amp_rate") == 0)
    {
        if (s->prevClosePx != 0 && s->lastPx > 0)
            return sprintf(buffer, "%.2f",
                    static_cast<int32_t>((s->highPx - s->lowPx)) * 100.0 / s->prevClosePx);
        else
            return sprintf(buffer, "0.00");
    }
    return getString( buffer, select, curSnap);
    /*
    if (strcmp(select, "code") == 0)
        return sprintf(buffer, "\"%s\"", s->securityID);
    if (strcmp(select, "name") == 0)
        return sprintf(buffer, "\"%s\"", s->symbol);
    if (strcmp(select, "prev_close") == 0)
        return sprintf(buffer, "%s", getFloat(s->prevClosePx));
    if (strcmp(select, "open") == 0)
        return sprintf(buffer, "%s", getFloat(s->openPx));
    if (strcmp(select, "high") == 0)
        return sprintf(buffer, "%s", getFloat(s->highPx));
    if (strcmp(select, "low") == 0)
        return sprintf(buffer, "%s", getFloat(s->lowPx));
    if (strcmp(select, "last") == 0)
        return sprintf(buffer, "%s", getFloat(s->lastPx));
    if (strcmp(select, "change") == 0)
    {
        if (s->lastPx > 0)
        {
            return sprintf(buffer, "%s",
                    getFloat(static_cast<int32_t>(s->lastPx - s->prevClosePx)));
        }
        else
            return sprintf(buffer, "0.000");
    }
    if (strcmp(select, "chg_rate") == 0)
    {
        if (s->prevClosePx != 0 && s->lastPx > 0)
            return sprintf(buffer, "%.2f",
                    static_cast<int32_t>((s->lastPx - s->prevClosePx)) * 100.0
                            / s->prevClosePx);
        else
            return sprintf(buffer, "0.00");
    }
    if (strcmp(select, "amp_rate") == 0)
    {
        if (s->prevClosePx != 0 && s->lastPx > 0)
            return sprintf(buffer, "%.2f",
                    static_cast<int32_t>((s->highPx - s->lowPx)) * 100.0 / s->prevClosePx);
        else
            return sprintf(buffer, "0.00");
    }
    if (strcmp(select, "volume") == 0)
        return sprintf(buffer, "%ld", s->volume);
    if (strcmp(select, "amount") == 0)
        return sprintf(buffer, "%ld", s->amount);
    if (strcmp(select, "now_vol") == 0)
        return sprintf(buffer, "%ld", s->hand);
    if (strcmp(select, "buy_vol") == 0)
        return sprintf(buffer, "%ld", s->out);
    if (strcmp(select, "sell_vol") == 0)
        return sprintf(buffer, "%ld", s->in);
    if (strcmp(select, "bidpx1") == 0)
        return sprintf(buffer, "%s", getFloat(s->bid[0].netPx));
    if (strcmp(select, "askpx1") == 0)
        return sprintf(buffer, "%s", getFloat(s->offer[0].netPx));
    if (strcmp(select, "preWeightedAvgYield") == 0)
        return sprintf(buffer, "%s",
                getFloat(static_cast<int32_t>(s->preWeightedAvgYield), 4));
    if (strcmp(select, "openYield") == 0)
        return sprintf(buffer, "%s", getFloat(static_cast<int32_t>(s->openYield), 4));
    if (strcmp(select, "highYield") == 0)
        return sprintf(buffer, "%s", getFloat(static_cast<int32_t>(s->highYield), 4));
    if (strcmp(select, "lowYield") == 0)
        return sprintf(buffer, "%s", getFloat(static_cast<int32_t>(s->lowYield), 4));
    if (strcmp(select, "lastYield") == 0)
        return sprintf(buffer, "%s", getFloat(static_cast<int32_t>(s->lastYield), 4));
    if (strcmp(select, "weightedAvgYield") == 0)
        return sprintf(buffer, "%s",
                getFloat(static_cast<int32_t>(s->weightedAvgYield), 4));
    if (strcmp(select, "trade_time") == 0)
        return sprintf(buffer, "%d", s->tradeTime);
    if (strcmp(select, "num_trades") == 0)
        return sprintf(buffer, "%ld", s->totalNumTrades);
    return 0;
    */
}

