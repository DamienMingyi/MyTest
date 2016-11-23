/*
 * Stock2LineReply.cpp
 *
 *  Created on: Apr 14, 2014
 *      Author: level2
 */

#include "Stock2LineReply.h"
#include "../../public/stock2/StockSnap2.h"

COL_DESC_ST Stock2LineReply::stock2LineDesc[] =
{
    //colName,  dataType,   offset, plevel, structName, formatFunc, calcRule
    /* 时间 */
    { "time", DATATYPE_INT64, OFFSET(STOCKLINE2, dateTime), 0, NULL, default_format_func, NULL, NULL },
    /* 开盘价 */
    { "open_price", DATATYPE_INT64, OFFSET(STOCKLINE2, openPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 最高价 */
    { "high_price", DATATYPE_INT64, OFFSET(STOCKLINE2, highPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 最低价 */
    { "low_price", DATATYPE_INT64, OFFSET(STOCKLINE2, lowPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 最新价 */
    { "price", DATATYPE_INT64, OFFSET(STOCKLINE2, lastPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 均价 */
    { "avg_price", DATATYPE_INT64, OFFSET(STOCKLINE2, avgPx), 0, NULL, default_format_func, calcRule[0], NULL },
    /* 成交量 */
    { "volume", DATATYPE_INT64, OFFSET(STOCKLINE2, volume), 0, NULL, default_format_func, NULL, NULL },
    /* 成交金额 */
    { "amount", DATATYPE_INT64, OFFSET(STOCKLINE2, amount), 0, NULL, default_format_func, NULL, NULL },
    /* 涨跌 ,需要计算字段*/
    { "change", DATATYPE_INT64, OFFSET(STOCKLINE2, change), 0, NULL, NULL, NULL, NULL },
    /* 涨跌幅 ,需要计算字段*/
    { "chg_rate", DATATYPE_INT64, OFFSET(STOCKLINE2, amountOfIncrease), 0, NULL, NULL, NULL, NULL },
    /* 振幅 ,需要计算字段*/
    { "amp_rate", DATATYPE_INT64, OFFSET(STOCKLINE2, amplitude), 0, NULL, NULL, NULL, NULL }
};


Stock2LineReply::Stock2LineReply() {
	// TODO Auto-generated constructor stub
    if (this->hTblColDes)
    {
        HTableFree(this->hTblColDes, NULL);
        this->hTblColDes = NULL;
    }
}

Stock2LineReply::~Stock2LineReply() {
	// TODO Auto-generated destructor stub
}

void Stock2LineReply::registerCol()
{
    if (this->hTblColDes == NULL)
    {
        HTableCreate(&this->hTblColDes);
        int n = sizeof(stock2LineDesc) / sizeof(COL_DESC_ST);
        for (int i = 0; i < n; i++)
        {
            COL_DESC_ST *description = &stock2LineDesc[i];
            HTableEnter(this->hTblColDes, description->key,
                    (char *) description, NULL);
        }
    }
}
int Stock2LineReply::request(const char* script,const char* query,char* buffer,int& len)
{
	if (!LineReply::parse(script,query))
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	registerCol();
	SNAP_INDEX* si=(SNAP_INDEX*)(market)->getSnapIndex(code.c_str());
	if (!si)
		return HttpReply::replyError(HttpStatus::NotFound,buffer,len);

	line.clear();
	STOCKSNAP2* snap=(STOCKSNAP2*)si->snap_item;
	date=snap->date;
	time=snap->time;
	prev_close=snap->prevClosePx;
	CShmVector* lineVector=si->lineVector;
	total=si->lineVector->size();
	if (!LineReply::parseBeginEnd(0,-1))
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	HttpReply::parseSelect("price,avg_price,volume");
	if (total>0)
	{
		for (int i=begin;i<end;++i)
		{
			char* ln=&lineVector->getItem(i);
			if (ln!=NULL)
			{
				line.push_back(ln);
			}
			else
			{
				total=0;
				begin=0;
				end=0;
			}
		}
	}
	return LineReply::reply(buffer,len);
}

int  Stock2LineReply::getLineString(char* buffer,const char* select)
{
	STOCKLINE2* ln=(STOCKLINE2*)curLine;

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
	/*
	if (strcmp(select,"price")==0)
		return sprintf(buffer,"%s",getFloat(ln->lastPx));
	if (strcmp(select,"avg_price")==0)
		return sprintf(buffer,"%s",getFloat(ln->avgPx));
	if (strcmp(select,"volume")==0)
		return sprintf(buffer,"%ld",ln->volume);
	if (strcmp(select, "amount") == 0)
		return sprintf(buffer, "%ld", ln->amount);
	if (strcmp(select,"time")==0)
		return sprintf(buffer,"%ld",ln->dateTime);
	*/
	return 0;
}
