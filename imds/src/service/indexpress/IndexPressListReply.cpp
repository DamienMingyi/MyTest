/*
 * IndexPressListReply.cpp
 *
 *  Created on: Sep 9, 2015
 *      Author: level2
 */

#include "IndexPressListReply.h"
#include "../../public/indexpress/IndexPressSnap.h"

COL_DESC_ST IndexPressListReply::indexListDesc[] =
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
    { "prev_close", DATATYPE_INT64, OFFSET(INDEXSNAP, prevClosePx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 开盘价 */
    { "open", DATATYPE_INT64, OFFSET(INDEXSNAP, openPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 最高价 */
    { "high", DATATYPE_INT64, OFFSET(INDEXSNAP, highPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 最低价 */
    { "low", DATATYPE_INT64, OFFSET(INDEXSNAP, lowPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 最新价 */
    { "last", DATATYPE_INT64, OFFSET(INDEXSNAP, lastPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 均价 */
    { "avg", DATATYPE_INT64, OFFSET(INDEXSNAP, avgPx), 0, NULL, default_format_func, calcRule[0], NULL },
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

IndexPressListReply::IndexPressListReply()
{
    // TODO Auto-generated constructor stub
    dot = 4;
}

IndexPressListReply::~IndexPressListReply()
{
    // TODO Auto-generated destructor stub
    if (this->hTblColDes)
    {
        HTableFree(this->hTblColDes, NULL);
        this->hTblColDes = NULL;
    }
}

void IndexPressListReply::registerCol()
{
    if (this->hTblColDes == NULL)
    {
        HTableCreate(&this->hTblColDes);
        int n = sizeof(indexListDesc) / sizeof(COL_DESC_ST);
        for (int i = 0; i < n; i++)
        {
            COL_DESC_ST *description = &indexListDesc[i];
            HTableEnter(this->hTblColDes, description->key,
                    (char *) description, NULL);
        }
    }
}

int IndexPressListReply::request(const char* script,const char* query,char* buffer,int& len)
{
    if (!ListReply::parse(script, query))
        return HttpReply::replyError(HttpStatus::BadRequest, buffer, len);

    registerCol();
    HttpReply::parseSelect("code,name,prev_close");

    date = ((INDEXSNAP*) list[0].snap)->date;
    time = ((INDEXSNAP*) list[0].snap)->time;
    return ListReply::reply(buffer, len);
}

bool IndexPressListReply::getSelfList()
{
    list.clear();
    ListCmp cmp;
    if ((order.find("ASE") != string::npos) || (order.find("ase") != string::npos))
        cmp.order = false;
    else
        cmp.order = true;
    for (unsigned int i = 0; i < self.size(); ++i)
    {
        cmp.snap = this->market->getSnap(self[i].c_str());
        if (cmp.snap)
        {
            getOrder(cmp);
            list.push_back(cmp);
        }
    }
    return !list.empty();
}

bool IndexPressListReply::getTypeList()
{
    list.clear();
    ListCmp cmp;
    if ((order.find("ASE") != string::npos) || (order.find("ase") != string::npos))
        cmp.order = false;
    else
        cmp.order = true;
    if (listType.find("exchange") != string::npos)
    {
        int st = this->market->getSnapCount();
        for (int i = 1; i < st; ++i)
        {
            cmp.snap = this->market->getSnap(i);
            if (cmp.snap == NULL)
                break;

            bool isList = false;
            if (listSubType.find("all") != string::npos)
                isList = true;
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

bool IndexPressListReply::isExchangeSubType(string& subType,char* snap)
{
    /*
     INDEXSNAP* s = (INDEXSNAP*)snap;
     int id=atoi(s->type);

     if (listSubType.find("quotation")!=string::npos)
     return id == 1;
     if (listSubType.find("weight")!=string::npos)
     return id == 2;
     if (listSubType.find("iopv")!=string::npos)
     return id == 3;
     //	if (listSubType.find("sh") != string::npos)
     //		return strcmp(s->market, "1") == 0;
     //	if (listSubType.find("sz") != string::npos)
     //		return strcmp(s->market, "2") == 0;
     //	if (listSubType.find("hs") != string::npos)
     //		return strcmp(s->market, "3") == 0;
     //	if (listSubType.find("hk") != string::npos)
     //		return strcmp(s->market, "4") == 0;
     //	if (listSubType.find("ap") != string::npos)
     //		return strcmp(s->market, "5") == 0;
     //	if (listSubType.find("bond") != string::npos)
     //		return strcmp(s->market, "6") == 0;
     //	if (listSubType.find("others") != string::npos)
     //		return strcmp(s->market, "9") == 0;
     //	if (listSubType.find("global") != string::npos)
     //		return strcmp(s->market, "0") == 0;
     * */

    return true;
}
bool IndexPressListReply::getOrder(ListCmp& cmp)
{
    INDEXSNAP* s = (INDEXSNAP*) cmp.snap;
    if (!order.empty())
    {

        int n = sizeof(indexListDesc) / sizeof(COL_DESC_ST);
        for (int i = 0; i < n; i++)
        {
            COL_DESC_ST *description = &indexListDesc[i];
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
            cmp.type = 2;
            cmp.lCmp = s->prevClosePx;
            return true;
        }
        else if (order.find("open") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->openPx;
            return true;
        }
        else if (order.find("high") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->highPx;
            return true;
        }
        else if (order.find("low") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->lowPx;
            return true;
        }
        else if (order.find("last") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->lastPx;
            return true;
        }
        else if (order.find("change") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = static_cast<int64_t>(s->change);
            return true;
        }
        else if (order.find("chg_rate") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = static_cast<int64_t>(s->exchangeRate);
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
        else if (order.find("now_vol") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->hand;
            return true;
        }
        */
    }
    order.clear();
    return false;
}

int IndexPressListReply::getSnapString(char* buffer,const char* select)
{
    INDEXSNAP* s = (INDEXSNAP*) curSnap;

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

    return getString( buffer, select, curSnap);
}




