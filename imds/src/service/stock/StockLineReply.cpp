/*
 * StockLineReply.cpp
 *
 *  Created on: Mar 17, 2014
 *      Author: level2
 */

#include "StockLineReply.h"
#include "../../public/stock/StockSnap.h"

StockLineReply::StockLineReply()
{
}

StockLineReply::~StockLineReply()
{
}

int StockLineReply::request(const char* script,const char* query,char* buffer,int& len)
{
	if (!LineReply::parse(script,query))
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	SNAP_INDEX* si=market->getSnapIndex(code.c_str());
	if (!si)
		return HttpReply::replyError(HttpStatus::NotFound,buffer,len);

	line.clear();
	STOCKSNAP* snap=(STOCKSNAP*)si->snap_item;
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

int StockLineReply::getLineString(char* buffer,const char* select)
{
	STOCKLINE* ln=(STOCKLINE*)curLine;

    if (strcmp(select, "time") == 0)
        return sprintf(buffer, "%ld", ln->dateTime);
    if (strcmp(select, "open_price") == 0)
        return sprintf(buffer, "%s", getFloat(ln->openPx));
    if (strcmp(select, "high_price") == 0)
        return sprintf(buffer, "%s", getFloat(ln->highPx));
    if (strcmp(select, "low_price") == 0)
        return sprintf(buffer, "%s", getFloat(ln->lowPx));
    if (strcmp(select, "price") == 0)
        return sprintf(buffer, "%s", getFloat(ln->lastPx));
    if (strcmp(select, "avg_price") == 0)
        return sprintf(buffer, "%s", getFloat(ln->avgPx));
    if (strcmp(select, "volume") == 0)
        return sprintf(buffer, "%ld", ln->volume);
    if (strcmp(select, "amount") == 0)
        return sprintf(buffer, "%ld", ln->amount);
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

	return 0;
}


