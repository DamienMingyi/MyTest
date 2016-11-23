/*
 * OptionListReply.cpp
 *
 *  Created on: Mar 17, 2014
 *      Author: level2
 */

#include "OptionListReply.h"
#include "../../public/option/OptionSnap.h"
#include <cmath>

OptionListReply::OptionListReply()
{
    dot = 4;
}

OptionListReply::~OptionListReply()
{
}

int OptionListReply::request(const char* script, const char* query, char* buffer,
        int& len)
{
    if (!ListReply::parse(script, query))
        return HttpReply::replyError(HttpStatus::BadRequest, buffer, len);

    HttpReply::parseSelect("code,name,prev_close");

    date = ((SHOSNAP*) list[0].snap)->date;
    time = ((SHOSNAP*) list[0].snap)->time;
    return ListReply::reply(buffer, len);
}

bool OptionListReply::getSelfList()
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

bool OptionListReply::getTypeList()
{
    list.clear();
    ListCmp cmp;
    if ((order.find("ASE") != string::npos) || (order.find("ase") != string::npos))
        cmp.order = false;
    else
        cmp.order = true;
    if (listType.compare("tstyle") == 0)
    {
        if (listSubType.empty())
            return false;
        split('_', listSubType.c_str(), self);
        if (self.size() != 2)
            return false;
    }
    int st = this->market->getSnapCount();
    map<int, char*> mapUnderly;
    for (int i = 1; i < st; ++i)
    {
        cmp.snap = this->market->getSnap(i);
        if (cmp.snap == NULL)
            break;
        bool isList = false;
        if (listType.compare("exchange") == 0)
        {
            if (listSubType.compare("all") == 0)
            {
                isList = true;
            }
            else if (listSubType.compare("underlyingstock") == 0)
            {
                mapUnderly[atoi(((SHOSNAP*) cmp.snap)->underlyingSecurityID)] = cmp.snap;
            }
            else if (listSubType.compare("expiremonth") == 0)
            {
                /*
                 * 2014-12-31
                 * new requirement: add year
                 */
                mapUnderly[(((SHOSNAP*) cmp.snap)->expireDate) / 100] = cmp.snap;
            }
            /**
             * 2015-5-20
             * new requirement: new interface for underlying stock and corresponding expire month
             */
            else if (listSubType.compare("stockexpire") == 0)
                mapUnderly[atoi(((SHOSNAP*) cmp.snap)->underlyingSecurityID) * 100
                        + (((SHOSNAP*) cmp.snap)->expireDate) % 10000 / 100] = cmp.snap;

        }
        else if (listType.compare("tstyle") == 0)
        {
            int month = atoi(self[1].c_str());
            isList = (((self[0].compare(((SHOSNAP*) cmp.snap)->underlyingSecurityID) == 0)
                    && month == (((SHOSNAP*) cmp.snap)->expireDate) % 10000 / 100));
        }
        if (isList)
        {
            getOrder(cmp);
            list.push_back(cmp);
        }
    }
    if (listSubType.compare("underlyingstock") == 0
            || listSubType.compare("expiremonth") == 0
            || listSubType.compare("stockexpire") == 0)
    {
        map<int, char*>::iterator it;
        for (it = mapUnderly.begin(); it != mapUnderly.end(); ++it)
        {
            cmp.snap = it->second;
            getOrder(cmp);
            list.push_back(cmp);
        }
    }
    return !list.empty();
}

bool OptionListReply::getOrder(ListCmp& cmp)
{
    SHOSNAP* s = (SHOSNAP*) cmp.snap;
    if (!order.empty())
    {
        if (order.find("exc_code") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->exchangeCode);
            return true;
        }
        else if (order.find("mktcode") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->marketCode);
            return true;
        }
        else if (order.find("code") != string::npos)
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
        else if (order.find("line_time") != string::npos)
        {
            cmp.type = 1;
            cmp.iCmp = s->lineTime;
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
        else if (order.find("avgpx") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->avgPx;
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
        else if (order.find("change") != string::npos)
        {
            cmp.type = 2;
            if (s->lastPx > 0)
                cmp.lCmp = s->lastPx - s->prevSettlePrice;
            else
                cmp.lCmp = 0;
            return true;
        }
        else if (order.find("chg_rate") != string::npos)
        {
            cmp.type = 3;
            if (s->prevSettlePrice > 0 && s->lastPx > 0)
                cmp.dCmp = (s->lastPx - s->prevSettlePrice) * 100.0 / s->prevSettlePrice;
            else
                cmp.dCmp = 0;
            return true;
        }
        else if (order.find("amp_rate") != string::npos)
        {
            cmp.type = 3;
            if (s->prevSettlePrice > 0 && s->lastPx > 0)
                cmp.dCmp = (s->highPx - s->lowPx) * 100.0 / s->prevSettlePrice;
            else
                cmp.dCmp = 0;
            return true;
        }
        else if (order.find("interestrate") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->interestRate;
            return true;
        }
        else if (order.find("pre_vol") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->prevVolume;
            return true;
        }
        else if (order.find("pre_amt") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->prevAmount;
            return true;
        }
        else if (order.find("pre_snp_high") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->prevSnapHigh;
            return true;
        }
        else if (order.find("pre_snp_low") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->prevSnapLow;
            return true;
        }
        else if (order.find("pre_ln_vol") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->prevLineVolume;
            return true;
        }
        else if (order.find("pre_ln_amt") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->prevLineAmount;
            return true;
        }
        else if (order.find("bidpx1") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->bid[0].price;
            return true;
        }
        else if (order.find("askpx1") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->offer[0].price;
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
        else if (order.find("pin") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->pin;
            return true;
        }
        else if (order.find("rfsid") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->RFStreamID);
            return true;
        }
        else if (order.find("contractid") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->contractID);
            return true;
        }
        else if (order.find("stockid") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->underlyingSecurityID);
            return true;
        }
        else if (order.find("stocksymbol") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->underlyingSymbol);
            return true;
        }
        else if (order.find("stocktype") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->underlyingType);
            return true;
        }
        else if (order.find("opttype") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->optionType);
            return true;
        }
        else if (order.find("cop") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->callOrPut);
            return true;
        }
        else if (order.find("unit") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->contractMultiplierUnit;
            return true;
        }
        else if (order.find("exepx") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->exercisePrice;
            return true;
        }
        else if (order.find("startdate") != string::npos)
        {
            cmp.type = 1;
            cmp.iCmp = s->startDate;
            return true;
        }
        else if (order.find("enddate") != string::npos)
        {
            cmp.type = 1;
            cmp.iCmp = s->endDate;
            return true;
        }
        else if (order.find("exedate") != string::npos)
        {
            cmp.type = 1;
            cmp.iCmp = s->exerciseDate;
            return true;
        }
        else if (order.find("delidate") != string::npos)
        {
            cmp.type = 1;
            cmp.iCmp = s->deliveryDate;
            return true;
        }
        else if (order.find("expdate") != string::npos)
        {
            cmp.type = 1;
            cmp.iCmp = s->expireDate;
            return true;
        }
        else if (order.find("version") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->updateVersion);
            return true;
        }
        else if (order.find("pre_interest") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->totalLongPositon;
            return true;
        }
        else if (order.find("presetpx") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->prevSettlePrice;
            return true;
        }
        else if (order.find("stockclose") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->underlyingClosePx;
            return true;
        }
        else if (order.find("pxlmtype") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->priceLimitType);
            return true;
        }
        else if (order.find("uplimit") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->dailyPriceUpLimit;
            return true;
        }
        else if (order.find("downlimit") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->dailyPriceDownLimit;
            return true;
        }
        else if (order.find("marginunit") != string::npos)
        {
            cmp.type = 2;
            cmp.dCmp = s->marginUnit / 100.00;
            return true;
        }
        else if (order.find("margratpar1") != string::npos)
        {
            cmp.type = 1;
            cmp.iCmp = s->marginRatioParam1;
            return true;
        }
        else if (order.find("margratpar2") != string::npos)
        {
            cmp.type = 1;
            cmp.iCmp = s->marginRatioParam2;
            return true;
        }
        else if (order.find("roundlot") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->roundLot;
            return true;
        }
        else if (order.find("lmtordmin") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->lmtOrdMinFloor;
            return true;
        }
        else if (order.find("lmtordmax") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->lmtOrdMaxFloor;
            return true;
        }
        else if (order.find("mktordmin") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->mktOrdMinFloor;
            return true;
        }
        else if (order.find("mktordmax") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->mktOrdMaxFloor;
            return true;
        }
        else if (order.find("ticksize") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->tickSize;
            return true;
        }
        else if (order.find("stocksta") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->securityStatusFlag);
            return true;
        }
        else if (order.find("stocklast") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->stockLastPx;
            return true;
        }
        else if (order.find("setpx") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->settlePx;
            return true;
        }
        else if (order.find("aucpx") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->auctionPx;
            return true;
        }
        else if (order.find("aucqty") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->auctionQty;
            return true;
        }
        else if (order.find("open_interest") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->openInterest;
            return true;
        }
        else if (order.find("num_trades") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->numTrades;
            return true;
        }
        else if (order.find("lastvolume") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->lastVolume;
            return true;
        }
        else if (order.find("tradephase") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->tradingPhaseCode);
            return true;
        }
        else if (order.find("tradesession") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->tradingSessionID);
            return true;
        }
        else if (order.find("leverage") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->leverageRatio;
            return true;
        }
        else if (order.find("premium") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->premiumRate;
            return true;
        }
        else if (order.find("remaindate") != string::npos)
        {
            cmp.type = 1;
            cmp.iCmp = s->remainingDays;
            return true;
        }
        else if (order.find("lastenquirytime") != string::npos)
        {
            cmp.type = 1;
            cmp.iCmp = s->lastestEnquiryTime;
            return true;
        }
        else if (order.find("in_value") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->inValue;
            return true;
        }
        else if (order.find("time_value") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->timeValue;
            return true;
        }
        else if (order.find("expiremonth") != string::npos)
        {
            cmp.type = 1;
            cmp.iCmp = (s->expireDate) / 100;
            return true;
        }
        else if (order.find("y_volatility") != string::npos)
        {
            cmp.type = 3;
            cmp.dCmp = s->impliedVolatility;
            return true;
        }
        else if (order.find("s_volatility") != string::npos)
        {
            cmp.type = 3;
            cmp.dCmp = s->impliedVolatility / 2;
            return true;
        }
        else if (order.find("m_volatility") != string::npos)
        {
            cmp.type = 3;
            cmp.dCmp = s->impliedVolatility / sqrt(12.0);
            return true;
        }
        else if (order.find("w_volatility") != string::npos)
        {
            cmp.type = 3;
            cmp.dCmp = s->impliedVolatility / sqrt(52.0);
            return true;
        }
        else if (order.find("d_volatility") != string::npos)
        {
            cmp.type = 3;
            cmp.dCmp = s->impliedVolatility / 16;
            return true;
        }
        else if (order.find("interest_rate") != string::npos)
        {
            cmp.type = 3;
            cmp.dCmp = s->riskFreeInterestRate;
            return true;
        }
        else if (order.find("delta") != string::npos)
        {
            cmp.type = 3;
            cmp.dCmp = s->delta;
            return true;
        }
        else if (order.find("vega") != string::npos)
        {
            cmp.type = 3;
            cmp.dCmp = s->vega;
            return true;
        }
        else if (order.find("theta") != string::npos)
        {
            cmp.type = 3;
            cmp.dCmp = s->theta;
            return true;
        }
        else if (order.find("gamma") != string::npos)
        {
            cmp.type = 3;
            cmp.dCmp = s->gamma;
            return true;
        }
        else if (order.find("rho") != string::npos)
        {
            cmp.type = 3;
            cmp.dCmp = s->rho;
            return true;
        }
    }
    order.clear();
    return false;
}

int OptionListReply::getSnapString(char* buffer, const char* select)
{
    SHOSNAP* s = (SHOSNAP*) curSnap;

    ///交易所代码
    if (strcmp(select, "exc_code") == 0)
        return sprintf(buffer, "\"%s\"", s->exchangeCode);
    ///市场代码
    if (strcmp(select, "mktcode") == 0)
        return sprintf(buffer, "\"%s\"", s->marketCode);
    ///证券代码
    if (strcmp(select, "code") == 0)
        return sprintf(buffer, "\"%s\"", s->securityID);
    ///symbol[48];
    if (strcmp(select, "name") == 0)
        return sprintf(buffer, "\"%s\"", s->symbol);
    ///sectiontype
    if (strcmp(select, "sectype") == 0)
        return sprintf(buffer, "\"%c\"", s->sectiontype);
    ///分时线时间
    if (strcmp(select, "line_time") == 0)
        return sprintf(buffer, "%d", s->lineTime);
    ///昨收
    if (strcmp(select, "prev_close") == 0)
        return sprintf(buffer, "%s", getFloat(s->prevClosePx));     //previous close price
    ///开盘价
    if (strcmp(select, "open") == 0)
        return sprintf(buffer, "%s", getFloat(s->openPx));
    ///最高价
    if (strcmp(select, "high") == 0)
        return sprintf(buffer, "%s", getFloat(s->highPx));
    ///最低价
    if (strcmp(select, "low") == 0)
        return sprintf(buffer, "%s", getFloat(s->lowPx));
    ///最新价
    if (strcmp(select, "last") == 0)
        return sprintf(buffer, "%s", getFloat(s->lastPx));
    ///均价
    if (strcmp(select, "avgpx") == 0)
        return sprintf(buffer, "%s", getFloat(s->avgPx));
    ///成交量
    if (strcmp(select, "volume") == 0)
        return sprintf(buffer, "%ld", s->volume);
    ///成交金额
    if (strcmp(select, "amount") == 0)
        return sprintf(buffer, "%ld", s->amount);
    ///涨跌
    if (strcmp(select, "change") == 0)
    {
        if (s->lastPx > 0)
            return sprintf(buffer, "%s", getFloat(s->lastPx - s->prevSettlePrice));
        else
            return sprintf(buffer, "0.000");
    }
    ///涨跌幅
    if (strcmp(select, "chg_rate") == 0)
    {
        if (s->prevSettlePrice > 0 && s->lastPx > 0)
            return sprintf(buffer, "%.2f",
                    (s->lastPx - s->prevSettlePrice) * 100.0 / s->prevSettlePrice);
        else
            return sprintf(buffer, "0.00");
    }
    ///振幅
    if (strcmp(select, "amp_rate") == 0)
    {
        if (s->prevSettlePrice > 0 && s->lastPx > 0)
            return sprintf(buffer, "%.2f",
                    (s->highPx - s->lowPx) * 100.0 / s->prevSettlePrice);
        else
            return sprintf(buffer, "0.00");
    }
    if (strcmp(select, "interestrate") == 0)
        return sprintf(buffer, "%ld", s->interestRate);
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
    ///int64_t prevLineVolume; /* 前分时线量 */
    if (strcmp(select, "pre_ln_vol") == 0)
        return sprintf(buffer, "%ld", s->prevLineVolume);
    ///int64_t prevLineAmount; /* 前分时线金额 */
    if (strcmp(select, "pre_ln_amt") == 0)
        return sprintf(buffer, "%ld", s->prevLineAmount);
    ///LEVEL bid[5];
    if (strcmp(select, "bidpx1") == 0)
        return sprintf(buffer, "%s", getFloat(s->bid[0].price));
    ///LEVEL offer[5];
    if (strcmp(select, "askpx1") == 0)
        return sprintf(buffer, "%s", getFloat(s->offer[0].price));

    ///现手
    if (strcmp(select, "now_vol") == 0)
        return sprintf(buffer, "%ld", s->hand);
    ///外盘
    if (strcmp(select, "buy_vol") == 0)
        return sprintf(buffer, "%ld", s->out);
    ///内盘
    if (strcmp(select, "sell_vol") == 0)
        return sprintf(buffer, "%ld", s->in);
    ///成交方向
    if (strcmp(select, "pin") == 0)
        return sprintf(buffer, "%d", s->pin);
    ///<参考数据类型标识符
    if (strcmp(select, "rfsid") == 0)
        return sprintf(buffer, "\"%s\"", s->RFStreamID);
    ///<期权合约代码
    if (strcmp(select, "contractid") == 0)
        return sprintf(buffer, "\"%s\"", s->contractID);
    ///<标的证券代码
    if (strcmp(select, "stockid") == 0)
        return sprintf(buffer, "\"%s\"", s->underlyingSecurityID);
    ///<基础证券证券名称
    if (strcmp(select, "stocksymbol") == 0)
        return sprintf(buffer, "\"%s\"", s->underlyingSymbol);
    ///<标的证券类型
    if (strcmp(select, "stocktype") == 0)
        return sprintf(buffer, "\"%s\"", s->underlyingType);
    ///<欧式美式
    if (strcmp(select, "opttype") == 0)
        return sprintf(buffer, "\"%s\"", s->optionType);
    ///<认购认沽
    if (strcmp(select, "cop") == 0)
        return sprintf(buffer, "\"%s\"", s->callOrPut);
    ///<合约单位
    if (strcmp(select, "unit") == 0)
        return sprintf(buffer, "%ld", s->contractMultiplierUnit);
    ///<期权行权价
    if (strcmp(select, "exepx") == 0)
        return sprintf(buffer, "%s", getFloat(s->exercisePrice));
    ///<首个交易日
    if (strcmp(select, "startdate") == 0)
        return sprintf(buffer, "%d", s->startDate);
    ///<最后交易日
    if (strcmp(select, "enddate") == 0)
        return sprintf(buffer, "%d", s->endDate);
    ///<期权行权日
    if (strcmp(select, "exedate") == 0)
        return sprintf(buffer, "%d", s->exerciseDate);
    ///<行权交割日
    if (strcmp(select, "delidate") == 0)
        return sprintf(buffer, "%d", s->deliveryDate);
    ///<期权到期日
    if (strcmp(select, "expdate") == 0)
        return sprintf(buffer, "%d", s->expireDate);
    ///<合约版本号
    if (strcmp(select, "version") == 0)
        return sprintf(buffer, "\"%s\"", s->updateVersion);
    ///<当前合约未平仓数
    if (strcmp(select, "pre_interest") == 0)
        return sprintf(buffer, "%d", s->totalLongPositon);
    ///<合约前结算价
    if (strcmp(select, "presetpx") == 0)
        return sprintf(buffer, "%s", getFloat(s->prevSettlePrice));	//previous settle pricess
    ///<标的证券前收盘
    if (strcmp(select, "stockclose") == 0)
        return sprintf(buffer, "%s", getFloat(s->underlyingClosePx));
    ///<涨跌幅限制类型
    if (strcmp(select, "pxlmtype") == 0)
        return sprintf(buffer, "\"%s\"", s->priceLimitType);
    ///<涨幅上限价格
    if (strcmp(select, "uplimit") == 0)
        return sprintf(buffer, "%s", getFloat(s->dailyPriceUpLimit));
    ///<跌幅下限价格
    if (strcmp(select, "downlimit") == 0)
        return sprintf(buffer, "%s", getFloat(s->dailyPriceDownLimit));
    ///<单位保证金
    if (strcmp(select, "marginunit") == 0)
        return sprintf(buffer, "%d.%02d", s->marginUnit / 100, s->marginUnit % 100);
    ///<保证金计算比例参数一
    if (strcmp(select, "margratpar1") == 0)
        return sprintf(buffer, "%d", s->marginRatioParam1);
    ///<保证金计算比例参数二
    if (strcmp(select, "margratpar2") == 0)
        return sprintf(buffer, "%d", s->marginRatioParam2);
    ///<整手数
    if (strcmp(select, "roundlot") == 0)
        return sprintf(buffer, "%d", s->roundLot);
    ///<单笔限价申报下限
    if (strcmp(select, "lmtordmin") == 0)
        return sprintf(buffer, "%s", getFloat(s->lmtOrdMinFloor));
    ///<单笔限价申报上限
    if (strcmp(select, "lmtordmax") == 0)
        return sprintf(buffer, "%s", getFloat(s->lmtOrdMaxFloor));
    ///<单笔市价申报下限
    if (strcmp(select, "mktordmin") == 0)
        return sprintf(buffer, "%s", getFloat(s->mktOrdMinFloor));
    ///<单笔市价申报上限
    if (strcmp(select, "mktordmax") == 0)
        return sprintf(buffer, "%s", getFloat(s->mktOrdMaxFloor));
    ///<最小报价单位
    if (strcmp(select, "ticksize") == 0)
        return sprintf(buffer, "%ld", s->tickSize);
    ///<期权合约状态信息标签
    if (strcmp(select, "stocksta") == 0)
        return sprintf(buffer, "%s", s->securityStatusFlag);
    ///<标的现价
    if (strcmp(select, "stocklast") == 0)
        return sprintf(buffer, "%s", getFloat(s->stockLastPx));
    ///<今日结算价
    if (strcmp(select, "setpx") == 0)
        return sprintf(buffer, "%s", getFloat(s->settlePx));
    ///<波动性中断参考价
    if (strcmp(select, "aucpx") == 0)
        return sprintf(buffer, "%s", getFloat(s->auctionPx));
    ///<波动性中断集合竞价虚拟匹配量
    if (strcmp(select, "aucqty") == 0)
        return sprintf(buffer, "%ld", s->auctionQty);
    ///<总持仓量
    if (strcmp(select, "open_interest") == 0)
        return sprintf(buffer, "%ld", s->openInterest);
    if (strcmp(select, "num_trades") == 0)
        return sprintf(buffer, "%ld", s->numTrades);
    if (strcmp(select, "lastvolume") == 0)
        return sprintf(buffer, "%ld", s->lastVolume);
    ///<市场状态
    if (strcmp(select, "tradephase") == 0)
        return sprintf(buffer, "\"%s\"", s->tradingPhaseCode);
    if (strcmp(select, "tradesession") == 0)
        return sprintf(buffer, "\"%s\"", s->tradingSessionID);

    ///<杠杆比率
    if (strcmp(select, "leverage") == 0)
        return sprintf(buffer, "%d", s->leverageRatio / 100);
    ///<溢价率
    if (strcmp(select, "premium") == 0)
        return sprintf(buffer, "%s", getFloat(s->premiumRate, 2));
    ///<剩余天数
    if (strcmp(select, "remaindate") == 0)
        return sprintf(buffer, "%d", s->remainingDays);
    ///<最新询价时间
    if (strcmp(select, "lastenquirytime") == 0)
        return sprintf(buffer, "%d", s->lastestEnquiryTime);

    if (strcmp(select, "in_value") == 0)
        return sprintf(buffer, "%s", getFloat(s->inValue));

    if (strcmp(select, "time_value") == 0)
        return sprintf(buffer, "%s", getFloat(s->timeValue));

    /**
     * 2014-12-31
     * new requirement: add year
     */
    if (strcmp(select, "expiremonth") == 0)
        return sprintf(buffer, "%d", (s->expireDate) / 100);
    ///<隐含波动率
    /**
     * 2015-5-15
     * new requirement: add implied volatility, delta, vega, theta, gamma, rho
     */
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

    ///<利率
    if (strcmp(select, "interest_rate") == 0)
        return sprintf(buffer, "%.4f", s->riskFreeInterestRate);

    if (strcmp(select, "delta") == 0)
        return sprintf(buffer, "%.4f", s->delta);
    if (strcmp(select, "vega") == 0)
        return sprintf(buffer, "%.4f", s->vega);
    if (strcmp(select, "theta") == 0)
        return sprintf(buffer, "%.4f", s->theta);
    if (strcmp(select, "gamma") == 0)
        return sprintf(buffer, "%.4f", s->gamma);
    if (strcmp(select, "rho") == 0)
        return sprintf(buffer, "%.4f", s->rho);
    return 0;
}

