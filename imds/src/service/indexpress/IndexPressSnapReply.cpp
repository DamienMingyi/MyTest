/*
 * IndexPressSnapReply.cpp
 *
 *  Created on: Sep 9, 2015
 *      Author: level2
 */

#include "IndexPressSnapReply.h"
#include "../../public/market/Market.h"
#include "../../public/indexpress/IndexPressSnap.h"
#include <cmath>

COL_DESC_ST IndexPressSnapReply::indexSnapDesc[] =
{
    //colName,  dataType,   offset, plevel, structName, formatFunc, calcRule
    /* 日期 */
    { "date", DATATYPE_INT32, OFFSET(INDEXSNAP, date), 0, NULL, default_format_func, NULL, NULL },
    /* 时间 */
    { "time", DATATYPE_INT32, OFFSET(INDEXSNAP, time), 0, NULL, default_format_func, NULL, NULL },
    /* 交易所代码 */
    { "exc_code", DATATYPE_STR, OFFSET(INDEXSNAP, exchangeCode), 0, NULL, default_format_func, NULL, NULL },
    /* 市场代码 */
    { "mktcode", DATATYPE_STR, OFFSET(INDEXSNAP, marketCode), 0, NULL, default_format_func, NULL, NULL },
    /* 证券代码 */
    { "code", DATATYPE_STR, OFFSET(INDEXSNAP, securityID), 0, NULL, default_format_func, NULL, NULL },
    /* 证券名称*/
    { "name", DATATYPE_STR, OFFSET(INDEXSNAP, symbol), 0, NULL, default_format_func, NULL, NULL },
    /* 事务类型 */
    { "sectype", DATATYPE_CHAR, OFFSET(INDEXSNAP, sectiontype), 0, NULL, default_format_func, NULL, NULL },
    /* 分时线时间 */
    { "line_time", DATATYPE_INT32, OFFSET(INDEXSNAP, lineTime), 0, NULL, default_format_func, NULL, NULL },
    /* 位置 */
    { "pos", DATATYPE_INT32, OFFSET(INDEXSNAP, pos), 0, NULL, default_format_func, NULL, NULL },
    /* 昨收 */
    { "prev_close", DATATYPE_INT64, OFFSET(INDEXSNAP, prevClosePx), 0, NULL, default_format_func, calcRule[0] },
    /* 开盘价 */
    { "open", DATATYPE_INT64, OFFSET(INDEXSNAP, openPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 最高价 */
    { "high", DATATYPE_INT64, OFFSET(INDEXSNAP, highPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 最低价 */
    { "low", DATATYPE_INT64, OFFSET(INDEXSNAP, lowPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 最新价 */
    { "last", DATATYPE_INT64, OFFSET(INDEXSNAP, lastPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 均价 */
    { "avg_px", DATATYPE_INT64, OFFSET(INDEXSNAP, avgPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 成交量 */
    { "volume", DATATYPE_INT64, OFFSET(INDEXSNAP, volume), 0, NULL, default_format_func, NULL, NULL },
    /* 成交金额 */
    { "amount", DATATYPE_INT64, OFFSET(INDEXSNAP, amount), 0, NULL, default_format_func, NULL, NULL },
    /* 涨跌,需要计算字段*/
    { "change", DATATYPE_INT64, OFFSET(INDEXSNAP, change), 0, NULL, NULL, NULL, NULL },
    /* 涨跌幅, 需要计算字段*/
    { "chg_rate", DATATYPE_INT64, OFFSET(INDEXSNAP, amountOfIncrease), 0, NULL, NULL, NULL, NULL },
    /* 振幅, 需要计算字段 */
    { "amp_rate", DATATYPE_INT64, OFFSET(INDEXSNAP, amplitude), 0, NULL, NULL, NULL, NULL },
    //利率
    { "int_rate", DATATYPE_INT64, OFFSET(INDEXSNAP, interestRate), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 前成交量 */
    { "pre_vol", DATATYPE_INT64, OFFSET(INDEXSNAP, prevVolume), 0, NULL, default_format_func, NULL, NULL },
    /* 前成交金额 */
    { "pre_amt", DATATYPE_INT64, OFFSET(INDEXSNAP, prevAmount), 0, NULL, default_format_func, NULL, NULL },
    /* 前快照最高价 */
    { "pre_snp_high", DATATYPE_INT64, OFFSET(INDEXSNAP, prevSnapHigh), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 前快照最低价 */
    { "pre_snp_low", DATATYPE_INT64, OFFSET(INDEXSNAP, prevSnapLow), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 前分时线量 */
    { "pre_ln_vol", DATATYPE_INT64, OFFSET(INDEXSNAP, prevLineVolume), 0, NULL, default_format_func, NULL, NULL },
    /* 前分时线金额 */
    { "pre_ln_amt", DATATYPE_INT64, OFFSET(INDEXSNAP, prevLineAmount), 0, NULL, default_format_func, NULL, NULL },
    /* 市场 */
    { "market", DATATYPE_STR, OFFSET(INDEXSNAP, market), 0, NULL, default_format_func, NULL, NULL },
    /* 汇率, 需要计算字段*/
    { "exchange_rate", DATATYPE_INT64, OFFSET(INDEXSNAP, exchangeRate), 0, NULL, NULL, NULL, NULL },
    /* 指数展示序号 */
    { "index_seq", DATATYPE_INT64, OFFSET(INDEXSNAP, indexSeq), 0, NULL, default_format_func, NULL, NULL },
    /*当日收盘值2*/
    { "last2", DATATYPE_INT64, OFFSET(INDEXSNAP, lastIndex2), 0, NULL, default_format_func, calcRule[0], NULL },
    /*当日收盘值2*/
    { "last3", DATATYPE_INT64, OFFSET(INDEXSNAP, lastIndex3), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 币种 */
    { "currency", DATATYPE_STR, OFFSET(INDEXSNAP, market), 0, NULL, default_format_func, NULL, NULL },
    /* 现手 */
    { "now_vol", DATATYPE_INT64, OFFSET(INDEXSNAP, hand), 0, NULL, default_format_func, NULL, NULL }
};
IndexPressSnapReply::IndexPressSnapReply() {
	// TODO Auto-generated constructor stub
	dot=4;
}

IndexPressSnapReply::~IndexPressSnapReply() {
	// TODO Auto-generated destructor stub
    if (this->hTblColDes)
    {
        HTableFree(this->hTblColDes, NULL);
        this->hTblColDes = NULL;
    }
}

void IndexPressSnapReply::registerCol()
{
    if (this->hTblColDes == NULL)
    {
        HTableCreate(&this->hTblColDes);
        int n = sizeof(indexSnapDesc) / sizeof(COL_DESC_ST);
        for (int i = 0; i < n; i++)
        {
            COL_DESC_ST *description = &indexSnapDesc[i];
            HTableEnter(this->hTblColDes, description->key,
                    (char *) description, NULL);
        }
    }
}

int IndexPressSnapReply::request(const char* script,const char* query,char* buffer,int& len)
{
	if (!SnapReply::parse(script,query))
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	registerCol();
	snap=market->getSnap(code.c_str());
	if (!snap)
	{
		if (code.compare("000000")==0)
			snap=market->getSnap(0);
		if (!snap)
			return HttpReply::replyError(HttpStatus::NotFound,buffer,len);
	}
	INDEXSNAP* s=(INDEXSNAP*)snap;
	date=s->date;
	time=s->time;

	HttpReply::parseSelect("name,prev_close,open,high,low,last,change,chg_rate,volume,amount,now_vol");

	if (paramSelect.empty())
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	return SnapReply::reply(buffer,len);
}

int IndexPressSnapReply::getSnapString(char* buffer,const char* select)
{
	INDEXSNAP* s=(INDEXSNAP*)snap;

    //需要计算字段
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
    if (strcmp(select, "exchange_rate") == 0)
        return sprintf(buffer, "%.8lf", s->exchangeRate / 100000000.0);

    return getString( buffer, select, snap);


	if (strcmp(select,"code")==0)
		return sprintf(buffer,"\"%s\"",s->securityID);
	if (strcmp(select,"name")==0)
		return sprintf(buffer,"\"%s\"",s->symbol);
	if (strcmp(select,"prev_close")==0)
		return sprintf(buffer,"%s",getFloat(s->prevClosePx));		//previous close price
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
		return sprintf(buffer,"%s",getFloat(static_cast<int64_t>(s->change)));
	if (strcmp(select,"chg_rate")==0)
		return sprintf(buffer, "%s", getFloat(static_cast<int64_t>(s->exchangeRate)));
	if (strcmp(select,"volume")==0)
		return sprintf(buffer,"%ld",s->volume);
	if (strcmp(select,"amount")==0)
		return sprintf(buffer,"%ld", s->amount);
	if (strcmp(select,"now_vol")==0)
		return sprintf(buffer,"%ld",s->hand);
	if (strcmp(select, "exchange_rate") == 0)
		return sprintf(buffer, "%.8lf", s->exchangeRate / 100000000.0);
	if (strcmp(select, "currency") == 0)
		return sprintf(buffer, "\"%s\"", s->currency);
	if (strcmp(select, "market") == 0)
		return sprintf(buffer, "\"%s\"", s->market);
	if (strcmp(select, "last2") == 0)
		return sprintf(buffer, "%s", getFloat(s->lastIndex2));
	if (strcmp(select, "last3") == 0)
		return sprintf(buffer, "%s", getFloat(s->lastIndex3));
	/*
	if (strcmp(select, "impact") == 0)
		return sprintf(buffer, "%s", getFloat(static_cast<int64_t>(s->impact)));
	if (strcmp(select, "weight") == 0)
		return sprintf(buffer, "%s", getFloat(s->weight, 5));
	*/
	return 0;
}

