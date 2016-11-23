/*
 * OptionSnapReply.cpp
 *
 *  Created on: Mar 17, 2014
 *      Author: level2
 */

#include "OptionSnapReply.h"
#include "../../public/market/Market.h"
#include "../../public/option/OptionSnap.h"
#include <cmath>
OptionSnapReply::OptionSnapReply()
{
    dot = 4;
}

OptionSnapReply::~OptionSnapReply()
{
    // TODO Auto-generated destructor stub
}

int OptionSnapReply::request(const char* script, const char* query, char* buffer,
        int& len)
{
    if (!SnapReply::parse(script, query))
        return HttpReply::replyError(HttpStatus::BadRequest, buffer, len);

    snap = market->getSnap(code.c_str());
    if (!snap)
    {
        if (code.compare("00000000") == 0)
            snap = market->getSnap(0);
        if (!snap)
            return HttpReply::replyError(HttpStatus::NotFound, buffer, len);
    }
    SHOSNAP* s = (SHOSNAP*) snap;
    date = s->date;
    time = s->time;

    HttpReply::parseSelect(
            "name,presetpx,open,high,low,last,change,chg_rate,volume,amount,now_vol,buy_vol,sell_vol,bid,ask");

    if (paramSelect.empty())
        return HttpReply::replyError(HttpStatus::BadRequest, buffer, len);

    return SnapReply::reply(buffer, len);
}

int OptionSnapReply::getSnapString(char* buffer, const char* select)
{
    SHOSNAP* s = (SHOSNAP*) snap;
    if (strcmp(select, "code") == 0)
        return sprintf(buffer, "\"%s\"", s->securityID);
    if (strcmp(select, "name") == 0)
        return sprintf(buffer, "\"%s\"", s->symbol);
    if (strcmp(select, "prev_close") == 0)
        return sprintf(buffer, "%s", getFloat(s->prevClosePx));		//previous close price
    if (strcmp(select, "presetpx") == 0)
        return sprintf(buffer, "%s", getFloat(s->prevSettlePrice));	//previous settle price
    if (strcmp(select, "open") == 0)
        return sprintf(buffer, "%s", getFloat(s->openPx));
    if (strcmp(select, "high") == 0)
        return sprintf(buffer, "%s", getFloat(s->highPx));
    if (strcmp(select, "low") == 0)
        return sprintf(buffer, "%s", getFloat(s->lowPx));
    if (strcmp(select, "last") == 0)
        return sprintf(buffer, "%s", getFloat(s->lastPx));
    if (strcmp(select, "avg_px") == 0)
        return sprintf(buffer, "%s", getFloat(s->avgPx));
    if (strcmp(select, "change") == 0)
    {
        if (s->prevSettlePrice != 0)
            return sprintf(buffer, "%s", getFloat(s->lastPx - s->prevSettlePrice));
        else
            return sprintf(buffer, "0.000");
    }
    if (strcmp(select, "chg_rate") == 0)
    {
        if (s->prevSettlePrice != 0)
            return sprintf(buffer, "%.2f",
                    (s->lastPx - s->prevSettlePrice) * 100.0 / s->prevSettlePrice);
        else
            return sprintf(buffer, "0.00");
    }
    if (strcmp(select, "amp_rate") == 0)
    {
        if (s->prevSettlePrice != 0)
            return sprintf(buffer, "%.2f",
                    (s->highPx - s->lowPx) * 100.0 / s->prevSettlePrice);
        else
            return sprintf(buffer, "0.00");
    }
    if (strcmp(select, "volume") == 0)
    {
        return sprintf(buffer, "%ld", s->volume);
    }
    if (strcmp(select, "amount") == 0)
    {
        return sprintf(buffer, "%ld", s->amount);
    }
    if (strcmp(select, "now_vol") == 0)
    {
        return sprintf(buffer, "%ld", s->hand);
    }
    if (strcmp(select, "buy_vol") == 0)
    {
        return sprintf(buffer, "%ld", s->out);
    }
    if (strcmp(select, "sell_vol") == 0)
    {
        return sprintf(buffer, "%ld", s->in);
    }
    ///成交方向
    if (strcmp(select, "pin") == 0)
    {
        return sprintf(buffer, "%d", s->pin);
    }
    int slen = 0;
    if (strcmp(select, "bid") == 0)
    {
        slen = sprintf(buffer + slen, "[");
        for (int i = 0; i < 5; ++i)
        {
            if (i > 0)
                slen += sprintf(buffer + slen, ",");
            slen += sprintf(buffer + slen, "%s,%ld", getFloat(s->bid[i].price),
                    s->bid[i].size);
        }
        slen += sprintf(buffer + slen, "]");
        return slen;
    }
    if (strcmp(select, "ask") == 0)
    {
        slen = sprintf(buffer + slen, "[");
        for (int i = 0; i < 5; ++i)
        {
            if (i > 0)
                slen += sprintf(buffer + slen, ",");
            slen += sprintf(buffer + slen, "%s,%ld", getFloat(s->offer[i].price),
                    s->offer[i].size);
        }
        slen += sprintf(buffer + slen, "]");
        return slen;
    }
    if (strcmp(select, "contractid") == 0 || strcmp(select, "constractid") == 0)
        return sprintf(buffer, "\"%s\"", s->contractID);
    if (strcmp(select, "stockid") == 0)
        return sprintf(buffer, "\"%s\"", s->underlyingSecurityID);
    if (strcmp(select, "stocksymbol") == 0)
        return sprintf(buffer, "\"%s\"", s->underlyingSymbol);
    if (strcmp(select, "stocktype") == 0)
        return sprintf(buffer, "\"%s\"", s->underlyingType);
    if (strcmp(select, "unit") == 0)
        return sprintf(buffer, "%d", s->contractMultiplierUnit);
    if (strcmp(select, "exepx") == 0)
        return sprintf(buffer, "%s", getFloat(s->exercisePrice));
    if (strcmp(select, "startdate") == 0)
        return sprintf(buffer, "%d", s->startDate);
    if (strcmp(select, "enddate") == 0)
        return sprintf(buffer, "%d", s->endDate);
    if (strcmp(select, "exedate") == 0)
        return sprintf(buffer, "%d", s->exerciseDate);
    if (strcmp(select, "delidate") == 0)
        return sprintf(buffer, "%d", s->deliveryDate);
    if (strcmp(select, "expdate") == 0)
        return sprintf(buffer, "%d", s->expireDate);
    if (strcmp(select, "version") == 0)
        return sprintf(buffer, "\"%s\"", s->updateVersion);
    if (strcmp(select, "setpx") == 0)
        return sprintf(buffer, "%s", getFloat(s->settlePx));
    if (strcmp(select, "stockclose") == 0)
        return sprintf(buffer, "%s", getFloat(s->underlyingClosePx));
    if (strcmp(select, "stocklast") == 0)
        return sprintf(buffer, "%s", getFloat(s->stockLastPx));

    ///<波动性中断参考价
    if (strcmp(select, "aucpx") == 0)
        return sprintf(buffer, "%s", getFloat(s->auctionPx));
    ///<波动性中断集合竞价虚拟匹配量
    if (strcmp(select, "aucqty") == 0)
        return sprintf(buffer, "%ld", s->auctionQty);

    if (strcmp(select, "islimit") == 0)
        return sprintf(buffer, "\"%s\"", s->priceLimitType);
    if (strcmp(select, "uplimit") == 0)
        return sprintf(buffer, "%s", getFloat(s->dailyPriceUpLimit));
    if (strcmp(select, "downlimit") == 0)
        return sprintf(buffer, "%s", getFloat(s->dailyPriceDownLimit));
    if (strcmp(select, "marginunit") == 0)
        return sprintf(buffer, "%d.%02d", s->marginUnit / 100, s->marginUnit % 100);
    if (strcmp(select, "roundlot") == 0)
        return sprintf(buffer, "%d", s->roundLot);
    if (strcmp(select, "in_val") == 0)
        return sprintf(buffer, "%s", getFloat(s->inValue));
    if (strcmp(select, "time_val") == 0)
        return sprintf(buffer, "%s", getFloat(s->timeValue));
    if (strcmp(select, "pre_interest") == 0)
        return sprintf(buffer, "%d", s->totalLongPositon);
    if (strcmp(select, "open_interest") == 0)
        return sprintf(buffer, "%d", s->openInterest);
    if (strcmp(select, "tradephase") == 0)
        return sprintf(buffer, "\"%s\"", s->tradingPhaseCode);
    if (strcmp(select, "tradesession") == 0)
        return sprintf(buffer, "\"%s\"", s->tradingSessionID);
    if (strcmp(select, "remaindate") == 0)
        return sprintf(buffer, "%d", s->remainingDays);
    if (strcmp(select, "leverage") == 0)
        return sprintf(buffer, "%d", s->leverageRatio / 100);
    if (strcmp(select, "premium") == 0)
        return sprintf(buffer, "%s", getFloat(s->premiumRate, 2));
    /**	2015-1-9 new requirement	*/
    if (strcmp(select, "interest_rate") == 0)
        return sprintf(buffer, "%.4f", s->riskFreeInterestRate);

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

    if (strcmp(select, "y_volatility") == 0)
        return sprintf(buffer, "%.2f", s->impliedVolatility);
    if (strcmp(select, "s_volatility") == 0)
        return sprintf(buffer, "%.2f", s->impliedVolatility / 2);
    if (strcmp(select, "m_volatility") == 0)
        return sprintf(buffer, "%.2f", s->impliedVolatility / sqrt(12.0));
    if (strcmp(select, "w_volatility") == 0)
        return sprintf(buffer, "%.2f", s->impliedVolatility / sqrt(52.0));
    if (strcmp(select, "d_volatility") == 0)
        return sprintf(buffer, "%.2f", s->impliedVolatility / 16);

    /**	2015-4-15 new requirement	*/
    if (strcmp(select, "delta") == 0)
        return sprintf(buffer, "%.4f", s->delta);
    if (strcmp(select, "gamma") == 0)
        return sprintf(buffer, "%.4f", s->gamma);
    if (strcmp(select, "theta") == 0)
        return sprintf(buffer, "%.4f", s->theta);
    if (strcmp(select, "vega") == 0)
        return sprintf(buffer, "%.4f", s->vega);
    if (strcmp(select, "rho") == 0)
        return sprintf(buffer, "%.4f", s->rho);
    if (strcmp(select, "lastEnquiryTime") == 0)
        return sprintf(buffer, "%d", s->lastestEnquiryTime);
    return 0;
}

