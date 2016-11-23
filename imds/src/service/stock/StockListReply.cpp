/*
 * StockListReply.cpp
 *
 *  Created on: Mar 17, 2014
 *      Author: level2
 */

#include "StockListReply.h"
#include "../../public/stock/StockSnap.h"

StockListReply::StockListReply()
{
}

StockListReply::~StockListReply()
{
}

int StockListReply::request(const char* script, const char* query, char* buffer, int& len)
{
    if (!ListReply::parse(script, query))
        return HttpReply::replyError(HttpStatus::BadRequest, buffer, len);

    HttpReply::parseSelect("code,name,prev_close");

    date = ((STOCKSNAP*) list[0].snap)->date;
    time = ((STOCKSNAP*) list[0].snap)->time;
    return ListReply::reply(buffer, len);
}

bool StockListReply::getSelfList()
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

bool StockListReply::getTypeList()
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

bool StockListReply::isExchangeSubType(string& subType, char* snap)
{
//	int id=atoi(securityid);
//	if (listSubType.find("ashare")!=string::npos)
//		return (id>=600000&& id<=609999);
//	if (listSubType.find("bshare")!=string::npos)
//		return	(id>=900000&& id<=909999);
//	if (listSubType.find("fund")!=string::npos)
//		return (id>=500000&& id<=510999);
//	if (listSubType.find("bond")!=string::npos)
//		return (id>=9000&& id<=299999);
//	if (listSubType.find("index")!=string::npos)
//		return (id<=999);
//	return false;
    STOCKSNAP* s = (STOCKSNAP*) snap;
    if (listSubType.find("amarket") != string::npos)
        return strcmp(s->marketCode, "ASHR") == 0;
    if (listSubType.find("bmarket") != string::npos)
        return strcmp(s->marketCode, "BSHR") == 0;
    if (listSubType.find("cmarket") != string::npos)
        return strcmp(s->marketCode, "CSHR") == 0;

    if (listSubType.find("ashare") != string::npos)
        return strcmp(s->basicinfo.securitySubType, "ASH") == 0;
    if (listSubType.find("bshare") != string::npos)
        return strcmp(s->basicinfo.securitySubType, "BSH") == 0;
    if (listSubType.find("cshare") != string::npos)
        return strcmp(s->basicinfo.securitySubType, "CSH") == 0;

    if (listSubType.find("equity") != string::npos)
        return strcmp(s->basicinfo.securityType, "ES") == 0;
    if (listSubType.find("fund") != string::npos)
        return strcmp(s->basicinfo.securityType, "EU") == 0;
    if (listSubType.find("bond") != string::npos)
        return strcmp(s->basicinfo.securityType, "D") == 0;
    if (listSubType.find("warrant") != string::npos)
        return strcmp(s->basicinfo.securityType, "RWS") == 0;
    if (listSubType.find("future") != string::npos)
        return strcmp(s->basicinfo.securityType, "FF") == 0;
    if (listSubType.find("index") != string::npos)
    {
        int id = atoi(s->securityID);
        return (id <= 999);
    }

    if (listSubType.find("gbf") != string::npos)
        return strcmp(s->basicinfo.securitySubType, "GBF") == 0;
    if (listSubType.find("gbz") != string::npos)
        return strcmp(s->basicinfo.securitySubType, "GBZ") == 0;
    if (listSubType.find("dst") != string::npos)
        return strcmp(s->basicinfo.securitySubType, "DST") == 0;
    if (listSubType.find("dvp") != string::npos)
        return strcmp(s->basicinfo.securitySubType, "DVP") == 0;
    if (listSubType.find("cbf") != string::npos)
        return strcmp(s->basicinfo.securitySubType, "CBF") == 0;
    if (listSubType.find("ccf") != string::npos)
        return strcmp(s->basicinfo.securitySubType, "CCF") == 0;
    if (listSubType.find("cpf") != string::npos)
        return strcmp(s->basicinfo.securitySubType, "CPF") == 0;
    if (listSubType.find("fbf") != string::npos)
        return strcmp(s->basicinfo.securitySubType, "FBF") == 0;
    if (listSubType.find("crp") != string::npos)
        return strcmp(s->basicinfo.securitySubType, "CRP") == 0;
    if (listSubType.find("brp") != string::npos)
        return strcmp(s->basicinfo.securitySubType, "BRP") == 0;
    if (listSubType.find("orp") != string::npos)
        return strcmp(s->basicinfo.securitySubType, "ORP") == 0;
    if (listSubType.find("cbd") != string::npos)
        return strcmp(s->basicinfo.securitySubType, "CBD") == 0;
    if (listSubType.find("obd") != string::npos)
        return strcmp(s->basicinfo.securitySubType, "OBD") == 0;
    if (listSubType.find("cef") != string::npos)
        return strcmp(s->basicinfo.securitySubType, "CEF") == 0;
    if (listSubType.find("oef") != string::npos)
        return strcmp(s->basicinfo.securitySubType, "OEF") == 0;
    if (listSubType.find("ebs") != string::npos)
        return strcmp(s->basicinfo.securitySubType, "EBS") == 0;
    if (listSubType.find("fbl") != string::npos)
        return strcmp(s->basicinfo.securitySubType, "FBL") == 0;
    if (listSubType.find("ofn") != string::npos)
        return strcmp(s->basicinfo.securitySubType, "OFN") == 0;
    if (listSubType.find("ash") != string::npos)
        return strcmp(s->basicinfo.securitySubType, "ASH") == 0;
    if (listSubType.find("bsh") != string::npos)
        return strcmp(s->basicinfo.securitySubType, "BSH") == 0;
    if (listSubType.find("csh") != string::npos)
        return strcmp(s->basicinfo.securitySubType, "CSH") == 0;
    if (listSubType.find("oeq") != string::npos)
        return strcmp(s->basicinfo.securitySubType, "OEQ") == 0;
    if (listSubType.find("ciw") != string::npos)
        return strcmp(s->basicinfo.securitySubType, "CIW") == 0;
    if (listSubType.find("cov") != string::npos)
        return strcmp(s->basicinfo.securitySubType, "COV") == 0;
    if (listSubType.find("feq") != string::npos)
        return strcmp(s->basicinfo.securitySubType, "FEQ") == 0;
    if (listSubType.find("fbd") != string::npos)
        return strcmp(s->basicinfo.securitySubType, "FBD") == 0;
    if (listSubType.find("oft") != string::npos)
        return strcmp(s->basicinfo.securitySubType, "OFT") == 0;
    if (listSubType.find("amp") != string::npos)
        return strcmp(s->basicinfo.securitySubType, "AMP") == 0;
    if (listSubType.find("wit") != string::npos)
        return strcmp(s->basicinfo.securitySubType, "WIT") == 0;
    if (listSubType.find("lof") != string::npos)
        return strcmp(s->basicinfo.securitySubType, "LOF") == 0;
    if (listSubType.find("ops") != string::npos)
        return strcmp(s->basicinfo.securitySubType, "OPS") == 0;
    if (listSubType.find("pps") != string::npos)
        return strcmp(s->basicinfo.securitySubType, "PPS") == 0;
    if (listSubType.find("qrp") != string::npos)
        return strcmp(s->basicinfo.securitySubType, "QRP") == 0;
    return false;
}
bool StockListReply::getOrder(ListCmp& cmp)
{
    STOCKSNAP* s = (STOCKSNAP*) cmp.snap;

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
                cmp.lCmp = s->lastPx - s->prevClosePx;
            else
                cmp.lCmp = 0;
            return true;
        }
        else if (order.find("chg_rate") != string::npos)
        {
            cmp.type = 3;
            if (s->prevClosePx > 0 && s->lastPx > 0)
                cmp.dCmp = (s->lastPx - s->prevClosePx) * 100.0 / s->prevClosePx;
            else
                cmp.dCmp = 0;
            return true;
        }
        else if (order.find("amp_rate") != string::npos)
        {
            cmp.type = 3;
            if (s->prevClosePx > 0 && s->lastPx > 0)
                cmp.dCmp = (s->highPx - s->lowPx) * 100.0 / s->prevClosePx;
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
        else if (order.find("up_limit") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->cpxx.upLimitPx;
            return true;
        }
        else if (order.find("down_limit") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->cpxx.downLimitPx;
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
            cmp.type = 1;
            cmp.iCmp = s->pin;
            return true;
        }
        else if (order.find("num_trades") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->numTrades;
            return true;
        }
        else if (order.find("iopv") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->iopv;
            return true;
        }
        else if (order.find("nav") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->nav;
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
        else if (order.find("pinyin") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->basicinfo.phoneticize);
            return true;
        }
        else if (order.find("ename") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->basicinfo.englishName);
            return true;
        }
        else if (order.find("isin") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->basicinfo.isin);
            return true;
        }
        else if (order.find("type") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->basicinfo.securityType);
            return true;
        }
        else if (order.find("subtype") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->basicinfo.securitySubType);
            return true;
        }
        else if (order.find("indcode") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->basicinfo.industyCode);
            return true;
        }
        else if (order.find("regcode") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->basicinfo.regionCode);
            return true;
        }
        else if (order.find("board") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->basicinfo.exchangeBoard);
            return true;
        }
        else if (order.find("selfboard") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->basicinfo.selfBoard);
            return true;
        }
        else if (order.find("currency") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->basicinfo.currency);
            return true;
        }
        //股本数据
        else if (order.find("capadjdat") != string::npos)
        {
            cmp.type = 1;
            cmp.iCmp = s->capitaldata.adjustingDate;
            return true;
        }
        else if (order.find("capmktcap") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->capitaldata.totalCapital;
            return true;
        }
        else if (order.find("capnontrshr") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->capitaldata.nonTradableShare;
            return true;
        }
        else if (order.find("captrshr") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->capitaldata.tradableShare;
            return true;
        }
        else if (order.find("capashr") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->capitaldata.aTradableShare;
            return true;
        }
        else if (order.find("capbshr") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->capitaldata.bTradableShare;
            return true;
        }
        else if (order.find("caphshr") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->capitaldata.hTradableShare;
            return true;
        }
        //财务摘要
        else if (order.find("digetolast") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialDigest.totalAsset;
            return true;
        }
        else if (order.find("digeliqast") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialDigest.liquidAsset;
            return true;
        }
        else if (order.find("digefixast") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialDigest.fixedAsset;
            return true;
        }
        else if (order.find("digeltast") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialDigest.longTermAsset;
            return true;
        }
        else if (order.find("digeinast") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialDigest.intangibleAsset;
            return true;
        }
        else if (order.find("digeltdeb") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialDigest.longTermDebt;
            return true;
        }
        else if (order.find("digecurliab") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialDigest.currentLiabilities;
            return true;
        }
        else if (order.find("digehoeqty") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialDigest.shareHolderEquity;
            return true;
        }
        else if (order.find("digecapres") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialDigest.capitalReserve;
            return true;
        }
        else if (order.find("digebin") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialDigest.mainBusinessIncome;
            return true;
        }
        else if (order.find("digebpro") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialDigest.mainBusinessProfit;
            return true;
        }
        else if (order.find("digetolpro") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialDigest.totalProfit;
            return true;
        }
        else if (order.find("digenetpro") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialDigest.netProfit;
            return true;
        }
        else if (order.find("digeundpro") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialDigest.undistributedProfit;
            return true;
        }
        else if (order.find("digeopcaf") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialDigest.operatingNetCashFlow;
            return true;
        }
        else if (order.find("digeinvcaf") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialDigest.investingNetCashFlow;
            return true;
        }
        else if (order.find("digecaf") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialDigest.financingNetCashFlow;
            return true;
        }
        else if (order.find("digecafinc") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialDigest.cashFlowIncrease;
            return true;
        }
        else if (order.find("digeshdernum") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialDigest.shareholderNumber;
            return true;
        }
        //财务指标
        else if (order.find("indeapsh") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialIndex.earningPerShare;
            return true;
        }
        else if (order.find("indnetastpersh") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialIndex.netAssetPerShare;
            return true;
        }
        else if (order.find("indoptpersh") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialIndex.operatingCashFlowPerShare;
            return true;
        }
        else if (order.find("indundspersh") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialIndex.undistributedProfitPerShare;
            return true;
        }
        else if (order.find("indopnetprora") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialIndex.operatingNetProfitRate;
            return true;
        }
        else if (order.find("indopgosprora") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialIndex.operatingGrossProfitRate;
            return true;
        }
        else if (order.find("indprora") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialIndex.mainBusinessProfitRate;
            return true;
        }
        else if (order.find("indnetprora") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialIndex.netProfitRate;
            return true;
        }
        else if (order.find("indnetrtnast") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialIndex.netReturnOnAssets;
            return true;
        }
        else if (order.find("indcurra") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialIndex.currentRatio;
            return true;
        }
        else if (order.find("indqukra") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialIndex.quickRatio;
            return true;
        }
        else if (order.find("indastliara") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialIndex.assetLiabilityRatio;
            return true;
        }
        else if (order.find("inddeb2equra") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialIndex.debt2EquityRatio;
            return true;
        }
        else if (order.find("indequra") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialIndex.equityRatio;
            return true;
        }
        else if (order.find("indturovacc") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialIndex.turnoverOfAccountReceivable;
            return true;
        }
        else if (order.find("indturov") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialIndex.stockTurnover;
            return true;
        }
        else if (order.find("indrevgrwra") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialIndex.mainRevenueGrowthRate;
            return true;
        }
        else if (order.find("indprogrwra") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialIndex.netProfitGrowthRate;
            return true;
        }
        else if (order.find("indepsg") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialIndex.EPSG;
            return true;
        }
        else if (order.find("indgrwrashder") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialIndex.growthRateOfShareholder;
            return true;
        }
        else if (order.find("indmpip") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialIndex.MPIP;
            return true;
        }
        else if (order.find("indtolastgrwra") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->financialIndex.totalAssetsGrowthRate;
            return true;
        }
        //权息指标
        else if (order.find("wgtexdat") != string::npos)
        {
            cmp.type = 1;
            cmp.iCmp = s->weightedData.date;
            return true;
        }
        else if (order.find("wgttype") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->weightedData.type);
            return true;
        }
        else if (order.find("wgtrate") != string::npos)
        {
            cmp.type = 1;
            cmp.iCmp = s->weightedData.rate;
            return true;
        }
        else if (order.find("wgtpx") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->weightedData.price;
            return true;
        }
        //产品信息
        else if (order.find("cpxxcode") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->cpxx.basicSecurityCode);
            return true;
        }
        else if (order.find("cpxxmkttype") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->cpxx.marketType);
            return true;
        }
        else if (order.find("cpxxtype") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->cpxx.type);
            return true;
        }
        else if (order.find("cpxxsubtype") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->cpxx.subtype);
            return true;
        }
        else if (order.find("cpxxcy") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->cpxx.currency);
            return true;
        }
        else if (order.find("cpxxparval") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->cpxx.parValue;
            return true;
        }
        else if (order.find("cpxxlastrdat") != string::npos)
        {
            cmp.type = 1;
            cmp.iCmp = s->cpxx.lastTradingDate;
            return true;
        }
        else if (order.find("cpxxbuynum") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->cpxx.buyNum;
            return true;
        }
        else if (order.find("cpxxsellnum") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->cpxx.sellNum;
            return true;
        }
        else if (order.find("cpxxappuplmt") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->cpxx.applyUpimit;
            return true;
        }
        else if (order.find("cpxxappdwnlmt") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->cpxx.applyDownlimit;
            return true;
        }
        else if (order.find("cpxxpxlev") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->cpxx.priceLevel;
            return true;
        }
        else if (order.find("cpxxlisdat") != string::npos)
        {
            cmp.type = 1;
            cmp.iCmp = s->cpxx.listingDate;
            return true;
        }
        else if (order.find("cpxxxrrate") != string::npos)
        {
            cmp.type = 1;
            cmp.iCmp = s->cpxx.XRRate;
            return true;
        }
        else if (order.find("cpxxxdamt") != string::npos)
        {
            cmp.type = 1;
            cmp.iCmp = s->cpxx.XDAmount;
            return true;
        }
        else if (order.find("cpxxfinatag") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->cpxx.financialTag);
            return true;
        }
        else if (order.find("cpxxlendtag") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->cpxx.securityLendingTag);
            return true;
        }
        else if (order.find("cpxxprodusta") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->cpxx.productStatus);
            return true;
        }
        else if (order.find("cpxxlmttype") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->cpxx.upDownLimitType);
            return true;
        }
        else if (order.find("cpxxuplmtpx") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->cpxx.upLimitPx;
            return true;
        }
        else if (order.find("cpxxdwnlmtpx") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->cpxx.downLimitPx;
            return true;
        }
        //国债利息
        else if (order.find("gzlxintdat") != string::npos)
        {
            cmp.type = 1;
            cmp.iCmp = s->gzlx.interestDate;
            return true;
        }
        else if (order.find("gzlxintper100") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->gzlx.interestPerHundred;
            return true;
        }
        else if (order.find("gzlxintday") != string::npos)
        {
            cmp.type = 1;
            cmp.iCmp = s->gzlx.daysOfInterest;
            return true;
        }
        else if (order.find("gzlxcpnra") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->gzlx.couponRate;
            return true;
        }
        //非交易
        else if (order.find("fjynontrtype") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->fjy.nonTradeType);
            return true;
        }
        else if (order.find("fjyordbegdat") != string::npos)
        {
            cmp.type = 1;
            cmp.iCmp = s->fjy.nonTradeOrderBeginDate;
            return true;
        }
        else if (order.find("fjyordenddat") != string::npos)
        {
            cmp.type = 1;
            cmp.iCmp = s->fjy.nonTradeOrderEndDate;
            return true;
        }
        else if (order.find("fjynontrpx") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->fjy.nonTradePx;
            return true;
        }
        else if (order.find("fjyipoqutm") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->fjy.ipoQuantum;
            return true;
        }
        else if (order.find("fjyipodismetd") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->fjy.ipoDistributeMethod);
            return true;
        }
        else if (order.find("fjyipodwnlmtpx") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->fjy.ipoDownLimitPx;
            return true;
        }
        else if (order.find("fjyipouplmtpx") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->fjy.ipoUpLimitPx;
            return true;
        }
        else if (order.find("fjyregidat") != string::npos)
        {
            cmp.type = 1;
            cmp.iCmp = s->fjy.registerDate;
            return true;
        }
        else if (order.find("fjydrdat") != string::npos)
        {
            cmp.type = 1;
            cmp.iCmp = s->fjy.DRDate;
            return true;
        }
        else if (order.find("fjyissura") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->fjy.rightIssueRate;
            return true;
        }
        else if (order.find("fjyissuqutm") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->fjy.rightIssueQuantum;
            return true;
        }
        else if (order.find("fjyt2iopv") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->fjy.t2iopv;
            return true;
        }
        else if (order.find("fjyt1iopv") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->fjy.t1iopv;
            return true;
        }
        else if (order.find("fjyoffmetd") != string::npos)
        {
            cmp.type = 0;
            strcpy(cmp.charCmp, s->fjy.offeringMethod);
            return true;
        }
        else if (order.find("dbpfina") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->dbp.finance;
            return true;
        }
        else if (order.find("dbplend") != string::npos)
        {
            cmp.type = 2;
            cmp.lCmp = s->dbp.securityLending;
            return true;
        }
    }

    order.clear();
    return false;
}

int StockListReply::getSnapString(char* buffer, const char* select)
{
    STOCKSNAP* s = (STOCKSNAP*) curSnap;

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
            return sprintf(buffer, "%s", getFloat(s->lastPx - s->prevClosePx));
        else
            return sprintf(buffer, "0.000");
    }
    ///涨跌幅
    if (strcmp(select, "chg_rate") == 0)
    {
        if (s->prevClosePx > 0 && s->lastPx > 0)
            return sprintf(buffer, "%.2f",
                    (s->lastPx - s->prevClosePx) * 100.0 / s->prevClosePx);
        else
            return sprintf(buffer, "0.00");
    }
    ///振幅
    if (strcmp(select, "amp_rate") == 0)
    {
        if (s->prevClosePx > 0 && s->lastPx > 0)
            return sprintf(buffer, "%.2f",
                    (s->highPx - s->lowPx) * 100.0 / s->prevClosePx);
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
    if (strcmp(select, "num_trades") == 0)
        return sprintf(buffer, "%ld", s->numTrades);
    if (strcmp(select, "iopv") == 0)
        return sprintf(buffer, "%s", getFloat(s->iopv));
    if (strcmp(select, "nav") == 0)
        return sprintf(buffer, "%s", getFloat(s->nav));
    if (strcmp(select, "tradephase") == 0)
        return sprintf(buffer, "\"%s\"", s->tradingPhaseCode);
    if (strcmp(select, "tradesession") == 0)
        return sprintf(buffer, "\"%s\"", s->tradingSessionID);

    //证券基本信息
    ///拼音
    if (strcmp(select, "pinyin") == 0)
        return sprintf(buffer, "\"%s\"", s->basicinfo.phoneticize);
    ///英语简称
    if (strcmp(select, "ename") == 0)
        return sprintf(buffer, "\"%s\"", s->basicinfo.englishName);
    ///
    if (strcmp(select, "isin") == 0)
        return sprintf(buffer, "\"%s\"", s->basicinfo.isin);
    ///证券类型
    if (strcmp(select, "type") == 0)
        return sprintf(buffer, "\"%s\"", s->basicinfo.securityType);
    ///证券子类型
    if (strcmp(select, "subtype") == 0)
        return sprintf(buffer, "\"%s\"", s->basicinfo.securitySubType);
    ///行情代码
    if (strcmp(select, "indcode") == 0)
        return sprintf(buffer, "\"%s\"", s->basicinfo.industyCode);
    ///地区代码
    if (strcmp(select, "regcode") == 0)
        return sprintf(buffer, "\"%s\"", s->basicinfo.regionCode);
    ///交易所板块
    if (strcmp(select, "board") == 0)
        return sprintf(buffer, "\"%s\"", s->basicinfo.exchangeBoard);
    ///自选股板块
    if (strcmp(select, "selfboard") == 0)
        return sprintf(buffer, "\"%s\"", s->basicinfo.selfBoard);
    ///货币
    if (strcmp(select, "currency") == 0)
        return sprintf(buffer, "\"%s\"", s->basicinfo.currency);

    //股本数据
    ///变动日期
    if (strcmp(select, "capadjdat") == 0)
        return sprintf(buffer, "%d", s->capitaldata.adjustingDate);
    ///总股本market capitalization
    if (strcmp(select, "capmktcap") == 0)
        return sprintf(buffer, "%ld", s->capitaldata.totalCapital);
    ///非流通股
    if (strcmp(select, "capnontrshr") == 0)
        return sprintf(buffer, "%ld", s->capitaldata.nonTradableShare);
    ///流通股本
    if (strcmp(select, "captrshr") == 0)
        return sprintf(buffer, "%ld", s->capitaldata.tradableShare);
    ///A股
    if (strcmp(select, "capashr") == 0)
        return sprintf(buffer, "%ld", s->capitaldata.aTradableShare);
    ///B股
    if (strcmp(select, "capbshr") == 0)
        return sprintf(buffer, "%ld", s->capitaldata.bTradableShare);
    ///H股
    if (strcmp(select, "caphshr") == 0)
        return sprintf(buffer, "%ld", s->capitaldata.hTradableShare);

    //财务摘要
    /* 总资产 */
    if (strcmp(select, "digetolast") == 0)
        return sprintf(buffer, "%ld", s->financialDigest.totalAsset);
    /* 流动资产 */
    if (strcmp(select, "digeliqast") == 0)
        return sprintf(buffer, "%ld", s->financialDigest.liquidAsset);
    /* 固定资产 */
    if (strcmp(select, "digefixast") == 0)
        return sprintf(buffer, "%ld", s->financialDigest.fixedAsset);
    /* 长期投资 */
    if (strcmp(select, "digeltast") == 0)
        return sprintf(buffer, "%ld", s->financialDigest.longTermAsset);
    /* 其他无形资产 */
    if (strcmp(select, "digeinast") == 0)
        return sprintf(buffer, "%ld", s->financialDigest.intangibleAsset);
    /* 长期负债 */
    if (strcmp(select, "digeltdeb") == 0)
        return sprintf(buffer, "%ld", s->financialDigest.longTermDebt);
    /* 流动负债 */
    if (strcmp(select, "digecurliab") == 0)
        return sprintf(buffer, "%ld", s->financialDigest.currentLiabilities);
    /* 股东权益 */
    if (strcmp(select, "digehoeqty") == 0)
        return sprintf(buffer, "%ld", s->financialDigest.shareHolderEquity);
    /* 资本公积金 */
    if (strcmp(select, "digecapres") == 0)
        return sprintf(buffer, "%ld", s->financialDigest.capitalReserve);
    /* 主营业务收入 */
    if (strcmp(select, "digebin") == 0)
        return sprintf(buffer, "%ld", s->financialDigest.mainBusinessIncome);
    /* 主营业务利润 */
    if (strcmp(select, "digebpro") == 0)
        return sprintf(buffer, "%ld", s->financialDigest.mainBusinessProfit);
    /* 利润总额 */
    if (strcmp(select, "digetolpro") == 0)
        return sprintf(buffer, "%ld", s->financialDigest.totalProfit);
    /* 净利润 */
    if (strcmp(select, "digenetpro") == 0)
        return sprintf(buffer, "%ld", s->financialDigest.netProfit);
    /* 未分配利润 */
    if (strcmp(select, "digeundpro") == 0)
        return sprintf(buffer, "%ld", s->financialDigest.undistributedProfit);
    /* 经营现金净流量 */
    if (strcmp(select, "digeopcaf") == 0)
        return sprintf(buffer, "%ld", s->financialDigest.operatingNetCashFlow);
    /* 投资现金净流量 */
    if (strcmp(select, "digeinvcaf") == 0)
        return sprintf(buffer, "%ld", s->financialDigest.investingNetCashFlow);
    /* 筹资现金净流量 */
    if (strcmp(select, "digecaf") == 0)
        return sprintf(buffer, "%ld", s->financialDigest.financingNetCashFlow);
    /* 现金流量净增加额 */
    if (strcmp(select, "digecafinc") == 0)
        return sprintf(buffer, "%ld", s->financialDigest.cashFlowIncrease);
    /* 股东人数 */
    if (strcmp(select, "digeshdernum") == 0)
        return sprintf(buffer, "%ld", s->financialDigest.shareholderNumber);

    //财务指标
    /* 每股收益 */
    if (strcmp(select, "indeapsh") == 0)
        return sprintf(buffer, "%ld", s->financialIndex.earningPerShare);
    /* 每股净资产 */
    if (strcmp(select, "indnetastpersh") == 0)
        return sprintf(buffer, "%ld", s->financialIndex.netAssetPerShare);
    /* 每股营业现金流量 */
    if (strcmp(select, "indoptpersh") == 0)
        return sprintf(buffer, "%ld", s->financialIndex.operatingCashFlowPerShare);
    /* 每股未分利润  */
    if (strcmp(select, "indundspersh") == 0)
        return sprintf(buffer, "%ld", s->financialIndex.undistributedProfitPerShare);
    /* 经营净利率 */
    if (strcmp(select, "indopnetprora") == 0)
        return sprintf(buffer, "%ld", s->financialIndex.operatingNetProfitRate);
    /* 经营毛利率 */
    if (strcmp(select, "indopgosprora") == 0)
        return sprintf(buffer, "%ld", s->financialIndex.operatingGrossProfitRate);
    /* 主营业务利润率 */
    if (strcmp(select, "indprora") == 0)
        return sprintf(buffer, "%ld", s->financialIndex.mainBusinessProfitRate);
    /* 净利润率 */
    if (strcmp(select, "indnetprora") == 0)
        return sprintf(buffer, "%ld", s->financialIndex.netProfitRate);
    /* 净资产收益率 */
    if (strcmp(select, "indnetrtnast") == 0)
        return sprintf(buffer, "%ld", s->financialIndex.netReturnOnAssets);
    /* 流动比率 */
    if (strcmp(select, "indcurra") == 0)
        return sprintf(buffer, "%ld", s->financialIndex.currentRatio);
    /* 速动比率 */
    if (strcmp(select, "indqukra") == 0)
        return sprintf(buffer, "%ld", s->financialIndex.quickRatio);
    /* 资产负债比率 */
    if (strcmp(select, "indastliara") == 0)
        return sprintf(buffer, "%ld", s->financialIndex.assetLiabilityRatio);
    /* 负债权益比率 */
    if (strcmp(select, "inddeb2equra") == 0)
        return sprintf(buffer, "%ld", s->financialIndex.debt2EquityRatio);
    /* 股东权益比率 */
    if (strcmp(select, "indequra") == 0)
        return sprintf(buffer, "%ld", s->financialIndex.equityRatio);
    /* 应收帐款周转率 */
    if (strcmp(select, "indturovacc") == 0)
        return sprintf(buffer, "%ld", s->financialIndex.turnoverOfAccountReceivable);
    /* 存货周转率 */
    if (strcmp(select, "indturov") == 0)
        return sprintf(buffer, "%ld", s->financialIndex.stockTurnover);
    /* 主营收入增长率 */
    if (strcmp(select, "indrevgrwra") == 0)
        return sprintf(buffer, "%ld", s->financialIndex.mainRevenueGrowthRate);
    /* 净利润增长率 */
    if (strcmp(select, "indprogrwra") == 0)
        return sprintf(buffer, "%ld", s->financialIndex.netProfitGrowthRate);
    /* 每股收益增长率  */
    if (strcmp(select, "indepsg") == 0)
        return sprintf(buffer, "%ld", s->financialIndex.EPSG);
    /* 股东权益增长率 */
    if (strcmp(select, "indgrwrashder") == 0)
        return sprintf(buffer, "%ld", s->financialIndex.growthRateOfShareholder);
    /* 主营利润增长率 */
    if (strcmp(select, "indmpip") == 0)
        return sprintf(buffer, "%ld", s->financialIndex.MPIP);
    /* 总资产增长率  */
    if (strcmp(select, "indtolastgrwra") == 0)
        return sprintf(buffer, "%ld", s->financialIndex.totalAssetsGrowthRate);

    //权息指标
    /* 除权日期 ex-dividend*/
    if (strcmp(select, "wgtexdat") == 0)
        return sprintf(buffer, "%d", s->weightedData.date);
    /* 类型 */
    if (strcmp(select, "wgttype") == 0)
        return sprintf(buffer, "%s", s->weightedData.type);
    /* 比例 */
    if (strcmp(select, "wgtrate") == 0)
        return sprintf(buffer, "%s", getFloat(s->weightedData.rate));
    /* 价格 */
    if (strcmp(select, "wgtpx") == 0)
        return sprintf(buffer, "%s", getFloat(s->weightedData.price));

    //产品信息
    if (strcmp(select, "cpxxcode") == 0)
        return sprintf(buffer, "%s", s->cpxx.basicSecurityCode);
    /* 市场种类 */
    if (strcmp(select, "cpxxmkttype") == 0)
        return sprintf(buffer, "%s", s->cpxx.marketType);
    /* 证券类别 */
    if (strcmp(select, "cpxxtype") == 0)
        return sprintf(buffer, "%s", s->cpxx.type);

    if (strcmp(select, "cpxxsubtype") == 0)
        return sprintf(buffer, "%s", s->cpxx.subtype);

    if (strcmp(select, "cpxxcy") == 0)
        return sprintf(buffer, "%s", s->cpxx.currency);
    /* 面值 */
    if (strcmp(select, "cpxxparval") == 0)
        return sprintf(buffer, "%ld", s->cpxx.parValue);
    /* 最后交易日期 */
    if (strcmp(select, "cpxxlastrdat") == 0)
        return sprintf(buffer, "%d", s->cpxx.lastTradingDate);

    if (strcmp(select, "cpxxbuynum") == 0)
        return sprintf(buffer, "%ld", s->cpxx.buyNum);

    if (strcmp(select, "cpxxsellnum") == 0)
        return sprintf(buffer, "%ld", s->cpxx.sellNum);

    if (strcmp(select, "cpxxappuplmt") == 0)
        return sprintf(buffer, "%ld", s->cpxx.applyUpimit);

    if (strcmp(select, "cpxxappdwnlmt") == 0)
        return sprintf(buffer, "%ld", s->cpxx.applyDownlimit);

    if (strcmp(select, "cpxxpxlev") == 0)
        return sprintf(buffer, "%ld", s->cpxx.priceLevel);
    /* 上市日期 */
    if (strcmp(select, "cpxxlisdat") == 0)
        return sprintf(buffer, "%d", s->cpxx.listingDate);
    /* 除权比例 */
    if (strcmp(select, "cpxxxrrate") == 0)
        return sprintf(buffer, "%d", s->cpxx.XRRate);
    /* 除息金额 */
    if (strcmp(select, "cpxxxdamt") == 0)
        return sprintf(buffer, "%d", s->cpxx.XDAmount);
    /* 融资标的标志 */
    if (strcmp(select, "cpxxfinatag") == 0)
        return sprintf(buffer, "%s", s->cpxx.financialTag);
    /* 融券标的标志 */
    if (strcmp(select, "cpxxlendtag") == 0)
        return sprintf(buffer, "%s", s->cpxx.securityLendingTag);
    /* 产品状态标志 */
    if (strcmp(select, "cpxxprodusta") == 0)
        return sprintf(buffer, "%s", s->cpxx.productStatus);
    /* 涨跌幅限制类型 */
    if (strcmp(select, "cpxxlmttype") == 0)
        return sprintf(buffer, "%s", s->cpxx.upDownLimitType);
    /* 涨幅上限价格 */
    if (strcmp(select, "cpxxuplmtpx") == 0)
        return sprintf(buffer, "%s", getFloat(s->cpxx.upLimitPx));
    /* 跌幅下限价格 */
    if (strcmp(select, "cpxxdwnlmtpx") == 0)
        return sprintf(buffer, "%s", getFloat(s->cpxx.downLimitPx));

    //国债利息
    /* 计息日期 */
    if (strcmp(select, "gzlxintdat") == 0)
        return sprintf(buffer, "%d", s->gzlx.interestDate);
    /* 每百元应计利息额 */
    if (strcmp(select, "gzlxintper100") == 0)
        return sprintf(buffer, "%s", getFloat(s->gzlx.interestPerHundred));
    /* 利息天数 */
    if (strcmp(select, "gzlxintday") == 0)
        return sprintf(buffer, "%d", s->gzlx.daysOfInterest);
    /* 票面利率 */
    if (strcmp(select, "gzlxcpnra") == 0)
        return sprintf(buffer, "%s", getFloat(s->gzlx.couponRate));

    //非交易
    /* 非交易业务类型 */
    if (strcmp(select, "fjynontrtype") == 0)
        return sprintf(buffer, "%s", s->fjy.nonTradeType);
    /* 非交易订单输入开始日期 */
    if (strcmp(select, "fjyordbegdat") == 0)
        return sprintf(buffer, "%d", s->fjy.nonTradeOrderBeginDate);
    /* 非交易订单输入结束日期 */
    if (strcmp(select, "fjyordenddat") == 0)
        return sprintf(buffer, "%d", s->fjy.nonTradeOrderEndDate);

    /* 非交易价格 */
    if (strcmp(select, "fjynontrpx") == 0)
        return sprintf(buffer, "%s", getFloat(s->fjy.nonTradePx));
    /* IPO总量 */
    if (strcmp(select, "fjyipoqutm") == 0)
        return sprintf(buffer, "%ld", s->fjy.ipoQuantum);
    /* IPO分配方法 */
    if (strcmp(select, "fjyipodismetd") == 0)
        return sprintf(buffer, "%s", s->fjy.ipoDistributeMethod);
    /* IPO申购价格区间下限 */
    if (strcmp(select, "fjyipodwnlmtpx") == 0)
        return sprintf(buffer, "%s", getFloat(s->fjy.ipoDownLimitPx));
    /* IPO申购价格区间上限 */
    if (strcmp(select, "fjyipouplmtpx") == 0)
        return sprintf(buffer, "%s", getFloat(s->fjy.ipoUpLimitPx));
    /* 配股股权登记日 */
    if (strcmp(select, "fjyregidat") == 0)
        return sprintf(buffer, "%d", s->fjy.registerDate);
    /* 配股除权日 */
    if (strcmp(select, "fjydrdat") == 0)
        return sprintf(buffer, "%d", s->fjy.DRDate);
    /* 配股比例 */
    if (strcmp(select, "fjyissura") == 0)
        return sprintf(buffer, "%ld", s->fjy.rightIssueRate);
    /* 配股总量 */
    if (strcmp(select, "fjyissuqutm") == 0)
        return sprintf(buffer, "%ld", s->fjy.rightIssueQuantum);
    /* T-2日基金收益/基金净值 */
    if (strcmp(select, "fjyt2iopv") == 0)
        return sprintf(buffer, "%s", getFloat(s->fjy.t2iopv));
    /* T-1日基金收益/基金净值 */
    if (strcmp(select, "fjyt1iopv") == 0)
        return sprintf(buffer, "%s", getFloat(s->fjy.t1iopv));
    /* 发行方式 */
    if (strcmp(select, "fjyoffmetd") == 0)
        return sprintf(buffer, "%s", s->fjy.offeringMethod);

    //担保品
    /* 融资余额 */
    if (strcmp(select, "dbpfina") == 0)
        return sprintf(buffer, "%ld", s->dbp.finance);
    /* 融券余量 */
    if (strcmp(select, "dbplend") == 0)
        return sprintf(buffer, "%ld", s->dbp.securityLending);
    return 0;
}

