/*
 * StockSnapReply.cpp
 *
 *  Created on: Mar 17, 2014
 *      Author: level2
 */

#include "StockSnapReply.h"
#include "../../public/market/Market.h"
#include "../../public/stock/StockSnap.h"

StockSnapReply::StockSnapReply()
{
}

StockSnapReply::~StockSnapReply()
{
}

int StockSnapReply::request(const char* script,const char* query,char* buffer,int& len)
{
	if (!SnapReply::parse(script,query))
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	snap=market->getSnap(code.c_str());
	if (!snap)
	{
		if (code.compare("000000")==0)
			snap=market->getSnap(0);
		if (!snap)
			return HttpReply::replyError(HttpStatus::NotFound,buffer,len);
	}
	STOCKSNAP* s=(STOCKSNAP*)snap;
	date=s->date;
	time=s->time;

	HttpReply::parseSelect("name,prev_close,open,high,low,last,change,chg_rate,volume,amount,now_vol,buy_vol,sell_vol,bid,ask");

	if (paramSelect.empty())
		return HttpReply::replyError(HttpStatus::BadRequest,buffer,len);

	return SnapReply::reply(buffer,len);
}

int StockSnapReply::getSnapString(char* buffer,const char* select)
{
	STOCKSNAP* s=(STOCKSNAP*)snap;
	if (strcmp(select,"code")==0)
		return sprintf(buffer,"\"%s\"",s->securityID);
	if (strcmp(select,"name")==0)
		return sprintf(buffer,"\"%s\"",s->symbol);
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
		if (s->lastPx!=0 && s->prevClosePx!=0)
			return sprintf(buffer,"%.2f",(s->lastPx-s->prevClosePx)*100.0/s->prevClosePx);
		else
			return sprintf(buffer,"0.00");
	}
	if (strcmp(select,"amp_rate")==0)
	{
		if (s->prevClosePx!=0 && s->lastPx!=0)
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
    if (strcmp(select, "pin") == 0)
    {
        return sprintf(buffer, "%d", s->pin);
    }
	int slen=0;
	if (strcmp(select,"bid")==0)
	{
		slen=sprintf(buffer+slen,"[");
		for (int i=0;i<5;++i)
		{
			if (i>0)
				slen+=sprintf(buffer+slen,",");
			slen+=sprintf(buffer+slen,"%s,%ld",getFloat(s->bid[i].price),s->bid[i].size);
		}
		slen+=sprintf(buffer+slen,"]");
		return slen;
	}
	if (strcmp(select,"ask")==0)
	{
		slen=sprintf(buffer+slen,"[");
		for (int i=0;i<5;++i)
		{
			if (i>0)
				slen+=sprintf(buffer+slen,",");
			slen+=sprintf(buffer+slen,"%s,%ld",getFloat(s->offer[i].price),s->offer[i].size);
		}
		slen+=sprintf(buffer+slen,"]");
		return slen;
	}
	/**
	 * 2015-5-19
	 * new requirement: price up/down limit
	 */
	if (strcmp(select, "up_limit") == 0)
		return sprintf(buffer, "%s", getFloat(s->cpxx.upLimitPx));
	if (strcmp(select, "down_limit") == 0)
		return sprintf(buffer, "%s", getFloat(s->cpxx.downLimitPx));
	/**
	 * 2015-6-9
	 * level-1 fast
	 */
	if (strcmp(select, "num_trades") == 0)
		return sprintf(buffer,"%ld",s->numTrades);
	if (strcmp(select, "iopv") == 0)
		return sprintf(buffer, "%s", getFloat(s->iopv));
	if (strcmp(select, "nav") == 0)
		return sprintf(buffer, "%s", getFloat(s->nav));
	if (strcmp(select, "tradephase") == 0)
		return sprintf(buffer, "\"%s\"", s->tradingPhaseCode);
	if (strcmp(select, "market") == 0)
		return sprintf(buffer, "\"%s\"", s->marketCode);
	if (strcmp(select, "type") == 0)
		return sprintf(buffer, "\"%s\"", s->basicinfo.securityType);
	if (strcmp(select, "sub_type") == 0)
		return sprintf(buffer, "\"%s\"", s->basicinfo.securitySubType);

    ///前成交量
    if (strcmp(select, "pre_vol") == 0)
        return sprintf(buffer, "%ld", s->prevVolume);
    ///前成交金额
    if (strcmp(select, "pre_amt") == 0)
        return sprintf(buffer, "%ld", s->prevAmount);
    ///int64_t prevSnapHigh;
    if (strcmp(select, "pre_snp_high") == 0)
        return sprintf(buffer, "%ld", s->prevSnapHigh);
    ///int64_t prevSnapLow;
    if (strcmp(select, "pre_snp_low") == 0)
        return sprintf(buffer, "%ld", s->prevSnapLow);
	return 0;
}

