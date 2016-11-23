/*
 * IndexPressLineReply.cpp
 *
 *  Created on: Sep 9, 2015
 *      Author: level2
 */

#include "IndexPressLineReply.h"
#include "../../public/indexpress/IndexPressSnap.h"

COL_DESC_ST IndexPressLineReply::indexLineDesc[] =
{
    //colName,  dataType,   offset, plevel, structName, formatFunc, calcRule
    /* 时间 */
    { "time", DATATYPE_INT64, OFFSET(INDEXLINE, dateTime), 0, NULL, default_format_func, NULL, NULL },
    /* 开盘价 */
    { "open_price", DATATYPE_INT64, OFFSET(INDEXLINE, openPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 最高价 */
    { "high_price", DATATYPE_INT64, OFFSET(INDEXLINE, highPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 最低价 */
    { "low_price", DATATYPE_INT64, OFFSET(INDEXLINE, lowPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 最新价 */
    { "price", DATATYPE_INT64, OFFSET(INDEXLINE, lastPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 均价 */
    { "avg_price", DATATYPE_INT64, OFFSET(INDEXLINE, avgPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 成交量 */
    { "volume", DATATYPE_INT64, OFFSET(INDEXLINE, volume), 0, NULL, default_format_func, NULL, NULL },
    /* 成交金额 */
    { "amount", DATATYPE_INT64, OFFSET(INDEXLINE, amount), 0, NULL, default_format_func, NULL, NULL },
    /* 涨跌 ,需要计算字段*/
    { "change", DATATYPE_INT64, OFFSET(INDEXLINE, change), 0, NULL, NULL, NULL, NULL },
    /* 涨跌幅 ,需要计算字段*/
    { "chg_rate", DATATYPE_INT64, OFFSET(INDEXLINE, amountOfIncrease), 0, NULL, NULL, NULL, NULL },
    /* 振幅 ,需要计算字段*/
    { "amp_rate", DATATYPE_INT64, OFFSET(INDEXLINE, amplitude), 0, NULL, NULL, NULL, NULL }
};

IndexPressLineReply::IndexPressLineReply()
{
    // TODO Auto-generated constructor stub
    dot = 4;
}

IndexPressLineReply::~IndexPressLineReply()
{
    // TODO Auto-generated destructor stub
    if (this->hTblColDes)
    {
        HTableFree(this->hTblColDes, NULL);
        this->hTblColDes = NULL;
    }
}

void IndexPressLineReply::registerCol()
{
    if (this->hTblColDes == NULL)
    {
        HTableCreate(&this->hTblColDes);
        int n = sizeof(indexLineDesc)/sizeof(COL_DESC_ST);
        for (int i = 0; i < n; i++)
        {
            COL_DESC_ST *description = &indexLineDesc[i];
            HTableEnter(this->hTblColDes, description->key,
                    (char *) description, NULL);
        }
    }
}

int IndexPressLineReply::request(const char* script, const char* query, char* buffer,
        int& len)
{
    if (!LineReply::parse(script, query))
        return HttpReply::replyError(HttpStatus::BadRequest, buffer, len);

    registerCol();
    SNAP_INDEX* si = (SNAP_INDEX*) (market)->getSnapIndex(code.c_str());
    if (!si)
        return HttpReply::replyError(HttpStatus::NotFound, buffer, len);

    line.clear();
    INDEXSNAP* snap = (INDEXSNAP*) si->snap_item;
    date = snap->date;
    time = snap->time;
    prev_close = snap->prevClosePx;
    CShmVector* lineVector = si->lineVector;
    total = si->lineVector->size();
    if (!LineReply::parseBeginEnd(0, -1))
        return HttpReply::replyError(HttpStatus::BadRequest, buffer, len);

    HttpReply::parseSelect("price,avg_price,volume");
    if (total > 0)
    {
        for (int i = begin; i < end; ++i)
        {
            char* ln = &lineVector->getItem(i);
            if (ln != NULL)
            {
                line.push_back(ln);
            }
            else
            {
                total = 0;
                begin = 0;
                end = 0;
            }
        }
    }
    return LineReply::reply(buffer, len);
}

int IndexPressLineReply::getLineString(char* buffer, const char* select)
{
    INDEXLINE* ln = (INDEXLINE*) curLine;

    //需要计算字段
    if (strcmp(select, "change") == 0)
    {
        //return sprintf(buffer, "%ld", ln->change);
        if (ln->lastPx > 0)
            return sprintf(buffer, "%s", getFloat(ln->lastPx - prev_close));
        else
            return sprintf(buffer, "0.000");
    }
    if (strcmp(select, "chg_rate") == 0)
    {
        //return sprintf(buffer, "%ld", ln->amountOfIncrease);
        if (prev_close > 0 && ln->lastPx > 0)
            return sprintf(buffer, "%.2f", (ln->lastPx - prev_close) * 100.0 / prev_close);
        else
            return sprintf(buffer, "0.00");
    }
    if (strcmp(select, "amp_rate") == 0)
    {
        //return sprintf(buffer, "%ld", ln->amplitude);
        if (prev_close > 0 && ln->lastPx > 0)
            return sprintf(buffer, "%.2f", (ln->highPx - ln->lowPx) * 100.0 / prev_close);
        else
            return sprintf(buffer, "0.00");
    }

    return getString( buffer, select, curLine);

}

