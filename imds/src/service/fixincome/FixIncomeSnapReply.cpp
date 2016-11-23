/*
 * FixIncomeSnapReply.cpp
 *
 *  Created on: Aug 31, 2015
 *      Author: level2
 */

#include "FixIncomeSnapReply.h"
#include "../../public/market/Market2.h"
#include "../../public/fixincome/FixIncomeSnap.h"


COL_DESC_ST FixIncomeSnapReply::fixIncomeSnapDesc[] =
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

FixIncomeSnapReply::FixIncomeSnapReply()
{
    // TODO Auto-generated constructor stub

}

FixIncomeSnapReply::~FixIncomeSnapReply()
{
    // TODO Auto-generated destructor stub
    if (this->hTblColDes)
    {
        HTableFree(this->hTblColDes, NULL);
        this->hTblColDes = NULL;
    }
}

void FixIncomeSnapReply::registerCol()
{
    if (this->hTblColDes == NULL)
    {
        HTableCreate(&this->hTblColDes);
        int n = sizeof(fixIncomeSnapDesc) / sizeof(COL_DESC_ST);
        for (int i = 0; i < n; i++)
        {
            COL_DESC_ST *description = &fixIncomeSnapDesc[i];
            HTableEnter(this->hTblColDes, description->key,
                    (char *) description, NULL);
        }
    }
}

int FixIncomeSnapReply::request(const char* script,const char* query,char* buffer,int& len)
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
	SHFISNAP* s=(SHFISNAP*)snap;
	date=s->date;
	time=s->time;

	HttpReply::parseSelect
	("name,prev_close,open,high,low,last,change,chg_rate,volume,amount,now_vol,buy_vol,sell_vol,bid,ask");

	if (paramSelect.empty())
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	return SnapReply::reply(buffer,len);
}

int FixIncomeSnapReply::getSnapString(char* buffer,const char* select)
{
    SHFISNAP* s=(SHFISNAP*)snap;

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
    int slen=0;
    if (strcmp(select, "bid") == 0)
    {
        slen = sprintf(buffer + slen, "[");
        for (int i = 0; i < 10; ++i)
        {
            if (i > 0)
                slen += sprintf(buffer + slen, ",");
            int orders;
            if (i == 0)
                orders = s->bid[i].ordersNum <= 50 ? s->bid[i].ordersNum : 50;
            else
                orders = 0;
            slen += sprintf(buffer + slen, "%s,%ld,%d,%d", getFloat(s->bid[i].netPx),
                    s->bid[i].volume, s->bid[i].ordersNum, orders);

            slen += sprintf(buffer + slen, ",[");
            for (int j = 0; j < orders; ++j)
            {
                if (j > 0)
                    slen += sprintf(buffer + slen, ",");
                slen += sprintf(buffer + slen, "%ld,%d,\"%s\",%ld,%s",
                        s->bid[i].orderQueue[j].orderNo,
                        s->bid[i].orderQueue[j].auctionTime,
                        s->bid[i].orderQueue[j].auctioneer,
                        //strlen(s->bid[0].orderQueue[i].auctioneer)?s->bid[0].orderQueue[i].auctioneer:"NULL",
                        s->bid[i].orderQueue[j].orderQty,
                        getFloat(s->bid[i].orderQueue[j].yield2Maturity, 4));
            }
            slen += sprintf(buffer + slen, "]");
        }
//      slen+=sprintf(buffer+slen,"],[");
//      for (int i = 0; i < 50; ++ i)
//      {
//          if (i > 0)
//              slen += sprintf(buffer + slen, ",");
//          slen += sprintf(buffer + slen, "%ld,%d,\"%s\",%ld,%s", s->bid[0].orderQueue[i].orderNo,
//                  s->bid[i].orderQueue[i].auctionTime,s->bid[0].orderQueue[i].auctioneer,
//                  //strlen(s->bid[0].orderQueue[i].auctioneer)?s->bid[0].orderQueue[i].auctioneer:"NULL",
//                  s->bid[0].orderQueue[i].orderQty, getFloat(s->bid[0].orderQueue[i].yield2Maturity, 4));
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
                orders = s->offer[i].ordersNum <= 50 ? s->offer[i].ordersNum : 50;
            else
                orders = 0;
            slen += sprintf(buffer + slen, "%s,%ld,%d,%d", getFloat(s->offer[i].netPx),
                    s->offer[i].volume, s->offer[i].ordersNum, orders);
            slen += sprintf(buffer + slen, ",[");
            for (int j = 0; j < orders; ++j)
            {
                if (j > 0)
                    slen += sprintf(buffer + slen, ",");
                slen += sprintf(buffer + slen, "%ld,%d,\"%s\",%ld,%s",
                        s->offer[i].orderQueue[j].orderNo,
                        s->offer[i].orderQueue[j].auctionTime,
                        s->offer[i].orderQueue[j].auctioneer,
                        //strlen(s->offer[0].orderQueue[i].auctioneer)?s->offer[0].orderQueue[i].auctioneer:"NULL",
                        s->offer[i].orderQueue[j].orderQty,
                        getFloat(s->offer[i].orderQueue[j].yield2Maturity, 4));
            }
            slen += sprintf(buffer + slen, "]");
        }
        slen += sprintf(buffer + slen, "]");
        return slen;
    }
    return getString( buffer, select, snap);
    /*
	if (strcmp(select,"code")==0)
		return sprintf(buffer,"\"%s\"",s->securityid);
	if (strcmp(select,"name")==0)
		return sprintf(buffer,"\"%s\"",s->symbol);
	if (strcmp(select,"pinyin")==0)
		return sprintf(buffer,"\"%s\"",s->py);
	if (strcmp(select,"prev_close")==0)
		return sprintf(buffer,"%s",getFloat(s->preclosepx));
	if (strcmp(select,"open")==0)
		return sprintf(buffer,"%s",getFloat(s->openpx));
	if (strcmp(select,"high")==0)
		return sprintf(buffer,"%s",getFloat(s->highpx));
	if (strcmp(select,"low")==0)
		return sprintf(buffer,"%s",getFloat(s->lowpx));
	if (strcmp(select,"last")==0)
		return sprintf(buffer,"%s",getFloat(s->lastpx));
	if (strcmp(select,"avg_px")==0)
		return sprintf(buffer,"%s",getFloat(s->avgpx));
	if (strcmp(select,"change")==0)
	{
		if (s->lastpx>0)
			return sprintf(buffer,"%s",getFloat(static_cast<int32_t>(s->lastpx-s->preclosepx)));
		else
			return sprintf(buffer,"0.000");
	}
	if (strcmp(select,"chg_rate")==0)
	{
		if (s->preclosepx!=0 && s->lastpx>0)
			return sprintf(buffer,"%.2f",static_cast<int32_t>((s->lastpx-s->preclosepx))*100.0/s->preclosepx);
		else
			return sprintf(buffer,"0.00");
	}
	if (strcmp(select,"amp_rate")==0)
	{
		if (s->preclosepx!=0 && s->lastpx>0)
			return sprintf(buffer,"%.2f",static_cast<int32_t>((s->highpx-s->lowpx))*100.0/s->preclosepx);
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


	if (strcmp(select, "trade_time") == 0)
		return sprintf(buffer, "%d", s->tradeTime);
	if (strcmp(select, "preWeightedAvgPx") == 0)
		return sprintf(buffer, "%s", getFloat(s->preWeightedAvgPx));
	if (strcmp(select, "weightedAvgPx") == 0)
		return sprintf(buffer, "%s", getFloat(s->weightedAvgPx));
	if (strcmp(select, "num_trades") == 0)
		return sprintf(buffer, "%ld", s->totalNumTrades);
	if (strcmp(select, "preCloseYield") == 0)
		return sprintf(buffer, "%s", getFloat(static_cast<int32_t>(s->preCloseYield), 4));
	if (strcmp(select, "preWeightedAvgYield") == 0)
		return sprintf(buffer, "%s", getFloat(static_cast<int32_t>(s->preWeightedAvgYield), 4));
	if (strcmp(select, "openYield") == 0)
		return sprintf(buffer, "%s", getFloat(static_cast<int32_t>(s->openYield), 4));
	if (strcmp(select, "highYield") == 0)
		return sprintf(buffer, "%s", getFloat(static_cast<int32_t>(s->highYield), 4));
	if (strcmp(select, "lowYield") == 0)
		return sprintf(buffer, "%s", getFloat(static_cast<int32_t>(s->lowYield), 4));
	if (strcmp(select, "lastYield") == 0)
		return sprintf(buffer, "%s", getFloat(static_cast<int32_t>(s->lastYield), 4));
	if (strcmp(select, "weightedAvgYield") == 0)
		return sprintf(buffer, "%s", getFloat(static_cast<int32_t>(s->weightedAvgYield), 4));
	if (strcmp(select, "accuredInterestAmt") == 0)
		return sprintf(buffer, "%s", getFloat(s->accruedInterestAmt, 4));
	if (strcmp(select, "product") == 0)
		return sprintf(buffer, "\"%s\"", s->tradeProduct);
	if (strcmp(select, "attribute") == 0)
		return sprintf(buffer, "\"%s\"", s->attribute);
	if (strcmp(select, "status") == 0)
		return sprintf(buffer, "\"%s\"", s->status);
	if (strcmp(select, "pledge") == 0)
		return sprintf(buffer, "\"%s\"", s->pledgeCode);
	if (strcmp(select, "openTime") == 0)
		return sprintf(buffer, "%d", s->openTime);
	if (strcmp(select, "closeTime") == 0)
		return sprintf(buffer, "%d", s->closeTime);
	if (strcmp(select, "offeringWay") == 0)
		return sprintf(buffer, "\"%s\"", s->offeringWay);
	if (strcmp(select, "denomination") == 0)
		return sprintf(buffer, "%d", s->denomination);
	if (strcmp(select, "offeringPx") == 0)
		return sprintf(buffer, "%d", s->offeringPx);
	if (strcmp(select, "interestType") == 0)
		return sprintf(buffer, "\"%s\"", s->interestType);
	if (strcmp(select, "payingFrequence") == 0)
		return sprintf(buffer, "\"%s\"", s->payingFrequence);
	if (strcmp(select, "nomial") == 0)
		return sprintf(buffer, "%d", s->nomialInterstRate);
	if (strcmp(select, "basicInterestRate") == 0)
		return sprintf(buffer, "%d", static_cast<int32_t>(s->basicInterestRate));
	if (strcmp(select, "basicInterestMargin") == 0)
		return sprintf(buffer, "%d", static_cast<int32_t>(s->basicInterestMargin));
	if (strcmp(select, "deadline") == 0)
		return sprintf(buffer, "%d", s->deadline);
	if (strcmp(select, "offeringVolume") == 0)
		return sprintf(buffer, "%d", s->offeringVolume);
	if (strcmp(select, "offeringBeginDate") == 0)
		return sprintf(buffer, "%d", s->offeringBeginDate);
	if (strcmp(select, "offeringEndDate") == 0)
		return sprintf(buffer, "%d", s->offeringEndDate);
	if (strcmp(select, "listDate") == 0)
		return sprintf(buffer, "%d", s->listingDate);
	if (strcmp(select, "expireDate") == 0)
		return sprintf(buffer, "%d", s->expiringDate);
	if (strcmp(select, "bondType") == 0)
		return sprintf(buffer, "\"%s\"", s->bondType);
	if (strcmp(select, "offeringMethod") == 0)
		return sprintf(buffer, "\"%s\"", s->offeringMethod);
	if (strcmp(select, "transMarket") == 0)
		return sprintf(buffer, "\"%s\"", s->transMarket);
	if (strcmp(select, "short") == 0)
		return sprintf(buffer, "\"%s\"", s->sellShort);
	if (strcmp(select, "shortLimit") == 0)
		return sprintf(buffer, "%d", s->totalShortLimit);
	if (strcmp(select, "traderShortLimit") == 0)
		return sprintf(buffer, "%d", s->traderShortLimit);
	return 0;
	*/
}

