/*
 * Sz1v5Reference.cpp
 *
 *  Created on: May 31, 2016
 *      Author: level2
 */

#include "Sz1v5Reference.h"
#include "../../util/shm/CShmEnv.h"
#include <time.h>
#include <iconv.h>
#include "../../public/program/Logger.h"
#include "../../public/program/Profile.h"
#include <stdlib.h>
#include "../../public/szstock/SZSnap.h"

Sz1v5Reference::Sz1v5Reference() {
	// TODO Auto-generated constructor stub
	setting = 0;
}

Sz1v5Reference::~Sz1v5Reference() {
	// TODO Auto-generated destructor stub
}

bool Sz1v5Reference::initial(const char* referencePath, const char* shm)
{
	time_t tt = time(NULL);
	tm *time = localtime(&tt);
	//current date and time
	date = (time->tm_year + 1900) * 10000 + (time->tm_mon + 1) * 100 + time->tm_mday;
	initime = time->tm_hour * 10000 + time->tm_min * 100 + time->tm_sec;

	char securities[256];
	memset(securities, 0, sizeof(securities));
	char indexInfo[256];
	memset(indexInfo, 0, sizeof(indexInfo));
	char stat[256];
	memset(stat, 0, sizeof(stat));
	char cashAuctionParams[256] = {0};
	memset(cashAuctionParams, 0, sizeof(cashAuctionParams));
	char zqjcxx[256];
	memset(zqjcxx, 0, sizeof(zqjcxx));
	char gbsj[256];
	memset(gbsj, 0, sizeof(gbsj));
	char cwzy[256];
	memset(cwzy, 0, sizeof(cwzy));
	char cwzb[256];
	memset(cwzb, 0, sizeof(cwzb));
	char qxsj[256];
	memset(qxsj, 0, sizeof(qxsj));
	char dqpz[256];
	memset(dqpz, 0, sizeof(dqpz));
	char hypz[256];
	memset(hypz, 0, sizeof(hypz));
	char gnpz[256];
	memset(gnpz, 0, sizeof(gnpz));

	sprintf(securities, "%s/securities_%d.xml", referencePath, date);
	sprintf(indexInfo, "%s/indexinfo_%d.xml", referencePath, date);
	sprintf(stat, "%s/stat_%d.xml", referencePath, date);
	sprintf(cashAuctionParams, "%s/cashauctionparams_%d.xml", referencePath, date);
	sprintf(zqjcxx,"%s/SZ_ZQJCXX.txt",referencePath);
	sprintf(gbsj,"%s/SZ_GBSJ.txt",referencePath);
	sprintf(cwzy,"%s/SZ_CWZY.txt",referencePath);
	sprintf(cwzb,"%s/SZ_CWZB.txt",referencePath);
	sprintf(qxsj,"%s/SZ_QXSJ.txt",referencePath);
	sprintf(dqpz,"%s/DQPZ.txt",referencePath);
	sprintf(hypz,"%s/HYPZ.txt",referencePath);
	sprintf(gnpz,"%s/GNPZ.txt",referencePath);

	if(!parseStaticFile(zqjcxx, gbsj, cwzy, cwzb, qxsj))
		return false;
	if(!plateFileParse(gnpz, ' '))
	{
		ERROR("gnpz parse failed");
		return false;
	}
	INFO("gnpz parse success");
	if (!readXML(securities, indexInfo, stat, cashAuctionParams))
		return false;
	if(!plateFileParse(dqpz,'A'))
	{
		ERROR("dqpz parse failed");
		return false;
	}
	INFO("dqpz parse success");
	if(!plateFileParse(hypz,'I'))
	{
		ERROR("hypz parse success");
		return false;
	}
	INFO("hypz parse success");
	if(!plateFileParse(gnpz,'C'))
	{
		ERROR("gnpz parse failed");
		return false;
	}
	INFO("gnpz parse success");

	SZSNAP szsnap;
	memset(&szsnap, 0, sizeof(szsnap));
	mszsnap["000000"] = szsnap;
	strncpy(szsnap.securityID, "000000", sizeof(szsnap.securityID));
	if (mszsnap.empty())
	{
		ERROR("no elements in the map");
		return false;
	}

	CShmEnv shmEnv;
	if (shmEnv.openShmEnv(shm) != SUCCEED)
	{
		ERROR("openShmEnv %s failed!", shm);
		return false;

	}
	SZSnap szSnap;
	Market market("sz1", &shmEnv, &szSnap);

	if (!market.load(sizeof(SZSNAP), sizeof(SZ1LINE), sizeof(SZ1TRADE)))
	{
		ERROR("market load error!");
		return false;
	}
	int pos = 1;
	for (MSZSNAP::iterator it = mszsnap.begin(); it != mszsnap.end(); ++ it, ++ pos)
		market.initSnap(pos, (char*)&(it->second));


	ST_SHM_ENV_INFO shmEnvInfo;
	SHM_OBJ_INFO_LIST shmObjInfoList;
	if (shmEnv.getEnvInfo(shmEnvInfo) && shmEnv.getShmObjInfo(shmObjInfoList))
	{
		mszsnap["000000"].hand = shmEnvInfo.objsSize * 100 / shmEnvInfo.objsCapacity;
		mszsnap["000000"].out = shmObjInfoList.size();
	}
	mszsnap["000000"].date = date;
	mszsnap["000000"].time = initime;
	mszsnap["000000"].amount = mszsnap.size();
	strncpy(mszsnap["000000"].securityID, "000000", 6);
	market.initSnap(0,(char*)&mszsnap["000000"]);
	market.refresh();
	return true;
}

bool Sz1v5Reference::readXML(char* securities, char* indexInfo, char* stat, char* cashAuctionParams)
{
	if (!parse(securities))
		return false;
	INFO("parse %s successful!", securities);
	if (!parse(indexInfo))
		return false;
	INFO("parse %s successful!", indexInfo);
	if (!parse(stat))
		return false;
	INFO("parse %s successful!", stat);
	if (!parse(cashAuctionParams))
		return false;
	INFO("parse %s successfule", cashAuctionParams);
	return true;
}

bool Sz1v5Reference::parse(char* file)
{
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(file) != 0)
	{
		ERROR("loading %s failed!", file);
		return false;
	}
	tinyxml2::XMLElement *scene = doc.RootElement();
	nodeParse(scene, 0, 1);
	return true;
}


std::string Sz1v5Reference::trim(const std::string str)
{
	return boost::trim_right_copy(str);
}


void Sz1v5Reference::nodeParse(tinyxml2::XMLElement *node, uint32_t depth, uint32_t width)
{
	tinyxml2::XMLElement *nodeChild = node->FirstChildElement();
    ++ depth;
    while (nodeChild)
    {
    	std::string name(nodeChild->Name());
    	std::string text;
    	if (nodeChild->GetText() != 0)
    		text = nodeChild->GetText();
    	if (name == "SecurityID" && nodeChild->GetText() != 0)
    	{
    		code = trim(text);
    		if (mszsnap.find(code) == mszsnap.end())
    		{
    			SZSNAP szsnap;
    			memset(&szsnap, 0, sizeof(szsnap));
    			mszsnap[code] = szsnap;
    			strncpy(mszsnap[code].securityID, code.c_str(), sizeof(mszsnap[code].securityID));
    			if(mBasicInfo.find(code) != mBasicInfo.end())
    			{
    				strncpy(mszsnap[code].basicinfo.phoneticize,mBasicInfo[code].phoneticize,sizeof(mBasicInfo[code].phoneticize)-1);
    				strncpy(mszsnap[code].basicinfo.isin,mBasicInfo[code].isin,sizeof(mBasicInfo[code].isin)-1);
    				strncpy(mszsnap[code].basicinfo.industyCode,mBasicInfo[code].industyCode,sizeof(mBasicInfo[code].industyCode)-1);
    				strncpy(mszsnap[code].basicinfo.regionCode,mBasicInfo[code].regionCode,sizeof(mBasicInfo[code].regionCode)-1);
    				//strncpy(mszsnap[code].basicinfo.exchangeBoard,mBasicInfo[code].exchangeBoard,sizeof(mBasicInfo[code].exchangeBoard)-1);
    				//strncpy(mszsnap[code].basicinfo.selfBoard,mBasicInfo[code].selfBoard,sizeof(mBasicInfo[code].selfBoard)-1);
    				//strncpy(mszsnap[code].basicinfo.currency,mBasicInfo[code].currency,sizeof(mBasicInfo[code].currency)-1);
    			}
    			if(mgnpz.find(code) != mgnpz.end())
    			{
    				strncpy(mszsnap[code].basicinfo.conceptCode, mgnpz[code].c_str(), strlen(mgnpz[code].c_str())-2);
    			}
    			if(mCapitalData.find(code) != mCapitalData.end())
    			{
    				mszsnap[code].capitaldata.adjustingDate = mCapitalData[code].adjustingDate;
    				mszsnap[code].capitaldata.totalCapital = mCapitalData[code].totalCapital;
    				mszsnap[code].capitaldata.nonTradableShare = mCapitalData[code].nonTradableShare;
    				mszsnap[code].capitaldata.tradableShare = mCapitalData[code].tradableShare;
    				mszsnap[code].capitaldata.aTradableShare = mCapitalData[code].aTradableShare;
    				mszsnap[code].capitaldata.bTradableShare = mCapitalData[code].bTradableShare;
    				mszsnap[code].capitaldata.hTradableShare = mCapitalData[code].hTradableShare;
    			}
    			if(mFinancialDigest.find(code) != mFinancialDigest.end())
    			{
    				mszsnap[code].financialDigest.totalAsset = mFinancialDigest[code].totalAsset;
    				mszsnap[code].financialDigest.liquidAsset = mFinancialDigest[code].liquidAsset;
    				mszsnap[code].financialDigest.fixedAsset = mFinancialDigest[code].fixedAsset;
    				//mszsnap[code].financialDigest.longTermAsset = mFinancialDigest[code].longTermAsset;
    				mszsnap[code].financialDigest.intangibleAsset = mFinancialDigest[code].intangibleAsset;
    				mszsnap[code].financialDigest.longTermDebt = mFinancialDigest[code].longTermDebt;
    				mszsnap[code].financialDigest.currentLiabilities = mFinancialDigest[code].currentLiabilities;
    				mszsnap[code].financialDigest.shareHolderEquity = mFinancialDigest[code].shareHolderEquity;
    				mszsnap[code].financialDigest.capitalReserve = mFinancialDigest[code].capitalReserve;
    				mszsnap[code].financialDigest.mainBusinessIncome = mFinancialDigest[code].mainBusinessIncome;
    				mszsnap[code].financialDigest.mainBusinessProfit = mFinancialDigest[code].mainBusinessProfit;
    				mszsnap[code].financialDigest.totalProfit = mFinancialDigest[code].totalProfit;
    				mszsnap[code].financialDigest.netProfit = mFinancialDigest[code].netProfit;
    				mszsnap[code].financialDigest.undistributedProfit = mFinancialDigest[code].undistributedProfit;
    				mszsnap[code].financialDigest.operatingNetCashFlow = mFinancialDigest[code].operatingNetCashFlow;
    				mszsnap[code].financialDigest.investingNetCashFlow = mFinancialDigest[code].investingNetCashFlow;
    				mszsnap[code].financialDigest.financingNetCashFlow = mFinancialDigest[code].financingNetCashFlow;
    				mszsnap[code].financialDigest.cashFlowIncrease = mFinancialDigest[code].cashFlowIncrease;
    				//mszsnap[code].financialDigest.shareholderNumber = mFinancialDigest[code].shareholderNumber;
    			}
    			if(mFinancialIndex.find(code) != mFinancialIndex.end())
    			{
    				mszsnap[code].financialIndex.earningPerShare = mFinancialIndex[code].earningPerShare;
    				mszsnap[code].financialIndex.netAssetPerShare = mFinancialIndex[code].netAssetPerShare;
    				mszsnap[code].financialIndex.operatingCashFlowPerShare = mFinancialIndex[code].operatingCashFlowPerShare;
    				mszsnap[code].financialIndex.undistributedProfitPerShare = mFinancialIndex[code].undistributedProfitPerShare;
    				mszsnap[code].financialIndex.operatingNetProfitRate = mFinancialIndex[code].operatingNetProfitRate;
    				mszsnap[code].financialIndex.operatingGrossProfitRate = mFinancialIndex[code].operatingGrossProfitRate;
    				mszsnap[code].financialIndex.mainBusinessProfitRate = mFinancialIndex[code].mainBusinessProfitRate;
    				mszsnap[code].financialIndex.netProfitRate = mFinancialIndex[code].netProfitRate;
    				mszsnap[code].financialIndex.netReturnOnAssets = mFinancialIndex[code].netReturnOnAssets;
    				mszsnap[code].financialIndex.currentRatio = mFinancialIndex[code].currentRatio;
    				mszsnap[code].financialIndex.quickRatio = mFinancialIndex[code].quickRatio;
    				mszsnap[code].financialIndex.assetLiabilityRatio = mFinancialIndex[code].assetLiabilityRatio;
    				//mszsnap[code].financialIndex.debt2EquityRatio = mFinancialIndex[code].debt2EquityRatio;
    				mszsnap[code].financialIndex.equityRatio = mFinancialIndex[code].equityRatio;
    				mszsnap[code].financialIndex.turnoverOfAccountReceivable = mFinancialIndex[code].turnoverOfAccountReceivable;
    				mszsnap[code].financialIndex.stockTurnover = mFinancialIndex[code].stockTurnover;
    				mszsnap[code].financialIndex.mainRevenueGrowthRate = mFinancialIndex[code].mainRevenueGrowthRate;
    				mszsnap[code].financialIndex.netProfitGrowthRate = mFinancialIndex[code].netProfitGrowthRate;
    				mszsnap[code].financialIndex.EPSG = mFinancialIndex[code].EPSG;
    				mszsnap[code].financialIndex.growthRateOfShareholder = mFinancialIndex[code].growthRateOfShareholder;
    				mszsnap[code].financialIndex.MPIP = mFinancialIndex[code].MPIP;
    				mszsnap[code].financialIndex.totalAssetsGrowthRate = mFinancialIndex[code].totalAssetsGrowthRate;
    			}
    			/*
    			if(mWeightData.find(code) != mWeightData.end())
    			{
    				mszsnap[code].weightedData.date = mWeightData[code].date;
    				memcpy(mszsnap[code].weightedData.type, mWeightData[code].type,sizeof(mWeightData[code].type));
    				mszsnap[code].weightedData.rate = mWeightData[code].rate;
    				mszsnap[code].weightedData.price = mWeightData[code].price;
    			}
    			*/
    		}
    	}
    	else if (name == "SecurityIDSource" && nodeChild->GetText() != 0)
    	{
    		strncpy(mszsnap[code].securityIDSource, text.c_str(), sizeof(mszsnap[code].securityIDSource));
    	}
    	else if (name == "Symbol" && nodeChild->GetText() != 0)
    	{
    		char symbol[sizeof(mszsnap[code].symbol)];
    		char sym[sizeof(mszsnap[code].symbol)];
    		memset(sym, 0, sizeof(mszsnap[code].symbol));
    		strncpy(sym, text.c_str(), text.length());
    		cv.convert(sym, sizeof(mszsnap[code].symbol), symbol, sizeof(mszsnap[code].symbol));
    		strncpy(mszsnap[code].symbol, trim(std::string(symbol)).c_str(), sizeof(mszsnap[code].symbol));
    	}
    	else if (name == "EnglishName" && nodeChild->GetText() != 0)
    	{
    		strncpy(mszsnap[code].basicinfo.englishName, trim(text).c_str(), sizeof(mszsnap[code].basicinfo.englishName));
    	}
    	else if (name == "ISIN" && nodeChild->GetText() != 0)
    	{
    		strncpy(mszsnap[code].basicinfo.isin, trim(text).c_str(), sizeof(mszsnap[code].basicinfo.isin));
    	}
    	else if (name == "UnderlyingSecurityID" && nodeChild->GetText() != 0)
    	{
    		strncpy(mszsnap[code].underlyingSecurityID, trim(text).c_str(), sizeof(mszsnap[code].underlyingSecurityID));
    	}
    	else if (name == "UnderlyingSecurityIDSource" && nodeChild->GetText() != 0)
    	{
    		strncpy(mszsnap[code].underlyingSecurityID, trim(text).c_str(), sizeof(mszsnap[code].underlyingSecurityID));
    	}
    	else if (name == "ListDate" && nodeChild->GetText() != 0)
    	{
    		mszsnap[code].listDate = atoi(text.c_str());
    	}
    	else if (name == "SecurityType" && nodeChild->GetText() != 0)
    	{
    		mszsnap[code].securityType = atoi(text.c_str());
    	}
    	else if (name == "Currency" && nodeChild->GetText() != 0)
    	{
    		strncpy(mszsnap[code].currency, trim(text).c_str(), sizeof(mszsnap[code].currency));
    	}
    	else if (name == "QtyUnit" && nodeChild->GetText() != 0)
    	{
			ps.parse(text);
			mszsnap[code].qtyUnit = ps.getMantissa() * pow(10.0L, 2 + ps.getExponent());
    	}
    	else if (name == "DayTrading" && nodeChild->GetText() != 0)
    	{
    		strncpy(mszsnap[code].dayTrading, trim(text).c_str(), sizeof(mszsnap[code].dayTrading));
    	}
    	else if (name == "PrevClosePx" && nodeChild->GetText() != 0)
    	{
			ps.parse(text);
			mszsnap[code].prevClosePx = ps.getMantissa() * pow(10.0L, 3 + ps.getExponent());
    	}
    	else if (name == "Status" && nodeChild->GetText() != 0)
    	{
    		mszsnap[code].status[atoi(text.c_str()) - 1] = '1';
    	}
    	else if (name == "OutstandingShare" && nodeChild->GetText() != 0)
    	{
			ps.parse(text);
			mszsnap[code].outstandingShare = ps.getMantissa() * pow(10.0L, 2 + ps.getExponent());
    	}
    	else if (name == "PublicFloatShareQuantity" && nodeChild->GetText() != 0)
    	{
			ps.parse(text);
			mszsnap[code].publicFloatShareQuantity = ps.getMantissa() * pow(10.0L, 2 + ps.getExponent());
    	}
    	else if (name == "ParValue" && nodeChild->GetText() != 0)
    	{
			ps.parse(text);
			mszsnap[code].parValue = ps.getMantissa() * pow(10.0L, 4 + ps.getExponent());
    	}
    	else if (name == "GageFlag" && nodeChild->GetText() != 0)
    	{
    		strncpy(mszsnap[code].gageFlag, trim(text).c_str(), sizeof(mszsnap[code].gageFlag));
    	}
    	else if (name == "GageRatio" && nodeChild->GetText() != 0)
    	{
			ps.parse(text);
			mszsnap[code].gageRatio = ps.getMantissa() * pow(10.0L, 2 + ps.getExponent());
    	}
    	else if (name == "CrdBuyUnderlying" && nodeChild->GetText() != 0)
    	{
    		strncpy(mszsnap[code].crdBuyUnderlying, trim(text).c_str(), sizeof(mszsnap[code].crdBuyUnderlying));
    	}
    	else if (name == "CrdSellUnderlying" && nodeChild->GetText() != 0)
    	{
    		strncpy(mszsnap[code].crdSellUnderlying, trim(text).c_str(), sizeof(mszsnap[code].crdSellUnderlying));
    	}
    	else if (name == "PriceCheckMode" && nodeChild->GetText() != 0)
    	{
    		mszsnap[code].priceCheckMode = atoi(text.c_str());
    	}
    	else if (name == "PledgeFlag" && nodeChild->GetText() != 0)
    	{
    		strncpy(mszsnap[code].pledgeFlag, trim(text).c_str(), sizeof(mszsnap[code].pledgeFlag));
    	}
    	else if (name == "ContractMultiplier" && nodeChild->GetText() != 0)
    	{
			ps.parse(text);
			mszsnap[code].contractMultiplier = ps.getMantissa() * pow(10.0L, 4 + ps.getExponent());
    	}
    	else if (name == "RegularShare" && nodeChild->GetText() != 0)
    	{
    		strncpy(mszsnap[code].regularShare, trim(text).c_str(), sizeof(mszsnap[code].regularShare));
    	}
    	else if (name == "QualificationFlag" && nodeChild->GetText() != 0)
    	{
    		strncpy(mszsnap[code].qualificationFlag, trim(text).c_str(), sizeof(mszsnap[code].qualificationFlag));
    	}
    	//stock
    	else if (name == "IndustryClassification" && nodeChild->GetText() != 0)
    	{
    		if (mszsnap[code].securityType == 1 || mszsnap[code].securityType == 2 ||
    				mszsnap[code].securityType == 3 || mszsnap[code].securityType == 4)
    		{
    			strncpy(mszsnap[code].industryClassification, trim(text).c_str(), sizeof(mszsnap[code].industryClassification));
    		}
    		else
    		{
    			ERROR("IndustryClassification is not in the sock!");
    			exit(1);
    		}
    	}
    	else if (name == "PreviousYearProfitPerShare" && nodeChild->GetText() != 0)
    	{
    		if (mszsnap[code].securityType == 1 || mszsnap[code].securityType == 2 ||
    				mszsnap[code].securityType == 3 || mszsnap[code].securityType == 4)
    		{
    			ps.parse(text);
    			mszsnap[code].previousYearProfitPerShare = ps.getMantissa() * pow(10.0L, 4 + ps.getExponent());
    		}
    		else
    		{
    			ERROR("CurrentYearProfitPerShare is not in the sock!");
    			exit(1);
    		}
    	}
    	else if (name == "CurrentYearProfitPerShare" && nodeChild->GetText() != 0)
    	{
    		if (mszsnap[code].securityType == 1 || mszsnap[code].securityType == 2 ||
    				mszsnap[code].securityType == 3 || mszsnap[code].securityType == 4)
    		{
    			ps.parse(text);
    			mszsnap[code].currentYearProfitShare = ps.getMantissa() * pow(10.0L, 4 + ps.getExponent());
    		}
    		else
    		{
    			ERROR("CurrentYearProfitPerShare is not in the sock!");
    			exit(1);
    		}
    	}
    	else if (name == "OfferingFlag" && nodeChild->GetText() != 0)
    	{
    		if (mszsnap[code].securityType == 1 || mszsnap[code].securityType == 2 ||
    				mszsnap[code].securityType == 3 || mszsnap[code].securityType == 4 ||
    				mszsnap[code].securityType == 5 || mszsnap[code].securityType == 6 ||
    				mszsnap[code].securityType == 7 || mszsnap[code].securityType == 8 ||
    				mszsnap[code].securityType == 9 || mszsnap[code].securityType == 10 ||
    				mszsnap[code].securityType == 11 || mszsnap[code].securityType == 33 ||
    				mszsnap[code].securityType == 34 || mszsnap[code].securityType == 35)
    		{
    			strncpy(mszsnap[code].offeringFlag, trim(text).c_str(), sizeof(mszsnap[code].offeringFlag));
    		}
    		else
    		{
    			ERROR("OfferingFlag is not in the sock, bond, preferred stock!");
    			exit(1);
    		}
    	}
    	//fund
    	else if (name == "NAV" && nodeChild->GetText() != 0)
    	{
    		if (mszsnap[code].securityType == 14 || mszsnap[code].securityType == 15 ||
    				mszsnap[code].securityType == 16 || mszsnap[code].securityType == 17 ||
    				mszsnap[code].securityType == 18 || mszsnap[code].securityType == 19 ||
    				mszsnap[code].securityType == 20 || mszsnap[code].securityType == 23 ||
    				mszsnap[code].securityType == 24 || mszsnap[code].securityType == 25 ||
    				mszsnap[code].securityType == 26)
    		{
    			ps.parse(text);
    			mszsnap[code].nav = ps.getMantissa() * pow(10.0L, 4 + ps.getExponent());
    		}
    		else
    		{
    			ERROR("NAV is not in the fund!");
    			exit(1);
    		}
    	}
    	//bond
    	else if (name == "CouponRate" && nodeChild->GetText() != 0)
    	{
    		if (mszsnap[code].securityType == 5 || mszsnap[code].securityType == 6 ||
    				mszsnap[code].securityType == 7 || mszsnap[code].securityType == 8 ||
    				mszsnap[code].securityType == 9 || mszsnap[code].securityType == 10 ||
    				mszsnap[code].securityType == 11 || mszsnap[code].securityType == 34 ||
    				mszsnap[code].securityType == 35)
    		{
    			ps.parse(text);
    			mszsnap[code].couponRate = ps.getMantissa() * pow(10.0L, 4 + ps.getExponent());
    		}
    		else
    		{
    			ERROR("CouponRate is not in the bond!");
    			exit(1);
    		}
    	}
    	else if (name == "IssuePrice" && nodeChild->GetText() != 0)
    	{
    		if (mszsnap[code].securityType == 5 || mszsnap[code].securityType == 6 ||
    				mszsnap[code].securityType == 7 || mszsnap[code].securityType == 8 ||
    				mszsnap[code].securityType == 9 || mszsnap[code].securityType == 10 ||
    				mszsnap[code].securityType == 11 || mszsnap[code].securityType == 34 ||
    				mszsnap[code].securityType == 35)
    		{
    			ps.parse(text);
    			mszsnap[code].issuePrice = ps.getMantissa() * pow(10.0L, 4 + ps.getExponent());
    		}
    		else
    		{
    			ERROR("CouponRate is not in the bond!");
    			exit(1);
    		}
    	}
    	else if (name == "Interest" && nodeChild->GetText() != 0)
    	{
    		if (mszsnap[code].securityType == 5 || mszsnap[code].securityType == 6 ||
    				mszsnap[code].securityType == 7 || mszsnap[code].securityType == 8 ||
    				mszsnap[code].securityType == 9 || mszsnap[code].securityType == 10 ||
    				mszsnap[code].securityType == 11 || mszsnap[code].securityType == 34 ||
    				mszsnap[code].securityType == 35)
    		{
    			ps.parse(text);
    			mszsnap[code].interest = ps.getMantissa() * pow(10.0L, 8 + ps.getExponent());
    		}
    		else if (mszsnap[code].securityType == 33)
    		{
       			ps.parse(text);
        		mszsnap[code].Interest = ps.getMantissa() * pow(10.0L, 8 + ps.getExponent());
    		}
    		else
    		{
    			ERROR("Interest is not in the bond or preferred stock!");
    			exit(1);
    		}
    	}
    	else if (name == "InterestAccuralDate" && nodeChild->GetText() != 0)
    	{
       		if (mszsnap[code].securityType == 5 || mszsnap[code].securityType == 6 ||
        			mszsnap[code].securityType == 7 || mszsnap[code].securityType == 8 ||
        			mszsnap[code].securityType == 9 || mszsnap[code].securityType == 10 ||
        			mszsnap[code].securityType == 11 || mszsnap[code].securityType == 34 ||
        			mszsnap[code].securityType == 35)
        	{
        		ps.parse(text);
        		mszsnap[code].interest = ps.getMantissa() * pow(10.0L, 8 + ps.getExponent());
        	}
    		else
    		{
    			ERROR("InterestAccuralDate is not in the bond!");
    			exit(1);
    		}
    	}
    	else if (name == "MarturiyDate" && nodeChild->GetText() != 0)
    	{
      		if (mszsnap[code].securityType == 5 || mszsnap[code].securityType == 6 ||
        			mszsnap[code].securityType == 7 || mszsnap[code].securityType == 8 ||
        			mszsnap[code].securityType == 9 || mszsnap[code].securityType == 10 ||
        			mszsnap[code].securityType == 11 || mszsnap[code].securityType == 34 ||
        			mszsnap[code].securityType == 35)
        	{
        		mszsnap[code].maturityDate = atoi(text.c_str());
        	}
    		else
    		{
    			ERROR("MarturiyDate is not in the bond!");
    			exit(1);
    		}
    	}
    	//warrant
    	else if (name == "ExercisePrice" && nodeChild->GetText() != 0)
    	{
    		if (mszsnap[code].securityType == 28 || mszsnap[code].securityType == 29 || mszsnap[code].securityType == 30)
    		{
        		ps.parse(text);
        		mszsnap[code].exercisePrice = ps.getMantissa() * pow(10.0L, 4 + ps.getExponent());
    		}
    		else
    		{
    			ERROR("ExercisePrice is not in the warrant or option!");
    			exit(1);
    		}
    	}
    	else if (name == "ExerciseRatio" && nodeChild->GetText() != 0)
    	{
    		if (mszsnap[code].securityType == 28)
    		{
        		ps.parse(text);
        		mszsnap[code].exerciseRatio = ps.getMantissa() * pow(10.0L, 4 + ps.getExponent());
    		}
    		else
    		{
    			ERROR("ExerciseRatio is not in the warrant!");
    			exit(1);
    		}
    	}
    	else if (name == "ExerciseBeginDate" && nodeChild->GetText() != 0)
    	{
    		if (mszsnap[code].securityType == 28 || mszsnap[code].securityType == 29 || mszsnap[code].securityType == 30)
    		{
    			mszsnap[code].exerciseBeginDate = atoi(text.c_str());
    		}
    		else
    		{
    			ERROR("ExerciseBeginDate is not in the warrant or option!");
    			exit(1);
    		}
    	}
    	else if (name == "ExerciseEndDate" && nodeChild->GetText() != 0)
    	{
    		if (mszsnap[code].securityType == 28 || mszsnap[code].securityType == 29 || mszsnap[code].securityType == 30)
    		{
    			mszsnap[code].exerciseEndDate = atoi(text.c_str());
    		}
    		else
    		{
    			ERROR("ExerciseEndDate is not in the warrant or option!");
    			exit(1);
    		}
    	}
    	else if (name == "CallOrPut" && nodeChild->GetText() != 0)
    	{
    		if (mszsnap[code].securityType == 28 || mszsnap[code].securityType == 29 || mszsnap[code].securityType == 30)
    		{
    			strncpy(mszsnap[code].callOrPut, trim(text).c_str(), sizeof(mszsnap[code].callOrPut));
    		}
    		else
    		{
    			ERROR("CallOrPut is not in the warrant or option!");
    			exit(1);
    		}
    	}
    	else if (name == "DeliveryType" && nodeChild->GetText() != 0)
    	{
    		if (mszsnap[code].securityType == 28 || mszsnap[code].securityType == 29 || mszsnap[code].securityType == 30)
    		{
    			strncpy(mszsnap[code].deliveryType, trim(text).c_str(), sizeof(mszsnap[code].deliveryType));
    		}
    		else
    		{
    			ERROR("DeliveryType is not in the warrant or option!");
    			exit(1);
    		}
    	}
    	else if (name == "ClearingPrice" && nodeChild->GetText() != 0)
    	{
    		if (mszsnap[code].securityType == 28)
    		{
        		ps.parse(text);
        		mszsnap[code].clearingPrice = ps.getMantissa() * pow(10.0L, 4 + ps.getExponent());
    		}
    		else
    		{
    			ERROR("ClearingPrice is not in the warrant!");
    			exit(1);
    		}
    	}
    	else if (name == "ExerciseType" && nodeChild->GetText() != 0)
    	{
    		if (mszsnap[code].securityType == 28 || mszsnap[code].securityType == 29 || mszsnap[code].securityType == 30)
    		{
    			strncpy(mszsnap[code].exerciseType, trim(text).c_str(), sizeof(mszsnap[code].exerciseType));
    		}
    		else
    		{
    			ERROR("ExerciseType is not in the warrant or option!");
    			exit(1);
    		}
    	}
    	else if (name == "LastTradeDay" && nodeChild->GetText() != 0)
    	{
    		if (mszsnap[code].securityType == 28)
    		{
    			mszsnap[code].lastTradeDay = atoi(text.c_str());
    		}
    		else
    		{
    			ERROR("LastTradeDay is not in the warrant!");
    			exit(1);
    		}
    	}
    	//repo
    	else if (name == "ExpirationDays" && nodeChild->GetText() != 0)
    	{
    		if (mszsnap[code].securityType == 12)
    		{
    			mszsnap[code].expirationDays = atoi(text.c_str());
    		}
    		else
    		{
    			ERROR("ExpirationDays is not in the repo!");
    			exit(1);
    		}
    	}
    	//option
    	else if (name == "ListType" && nodeChild->GetText() != 0)
    	{
    		if (mszsnap[code].securityType == 29 || mszsnap[code].securityType == 30)
    		{
    			mszsnap[code].listType = atoi(text.c_str());
    		}
    		else
    		{
    			ERROR("ListType is not in the option!");
    			exit(1);
    		}
    	}
    	else if (name == "DeliveryDay" && nodeChild->GetText() != 0)
    	{
    		if (mszsnap[code].securityType == 29 || mszsnap[code].securityType == 30)
    		{
    			mszsnap[code].deliveryDay = atoi(text.c_str());
    		}
    		else
    		{
    			ERROR("DeliveryDay is not in the option!");
    			exit(1);
    		}
    	}
    	else if (name == "DeliveryMonth" && nodeChild->GetText() != 0)
    	{
    		if (mszsnap[code].securityType == 29 || mszsnap[code].securityType == 30)
    		{
    			mszsnap[code].deliveryMonth = atoi(text.c_str());
    		}
    		else
    		{
    			ERROR("DeliveryMonth is not in the option!");
    			exit(1);
    		}
    	}
    	else if (name == "LastTradeDay" && nodeChild->GetText() != 0)
    	{
    		if (mszsnap[code].securityType == 29 || mszsnap[code].securityType == 30)
    		{
    			mszsnap[code].lastTradeDay = atoi(text.c_str());
    		}
    		else
    		{
    			ERROR("LastTradeDay is not in the option!");
    			exit(1);
    		}
    	}
    	else if (name == "AdjustTimes" && nodeChild->GetText() != 0)
    	{
    		if (mszsnap[code].securityType == 29 || mszsnap[code].securityType == 30)
    		{
    			mszsnap[code].adjustTimes = atoi(text.c_str());
    		}
    		else
    		{
    			ERROR("AdjustTimes is not in the option!");
    			exit(1);
    		}
    	}
    	else if (name == "ContractUnit" && nodeChild->GetText() != 0)
    	{
    		if (mszsnap[code].securityType == 29 || mszsnap[code].securityType == 30)
    		{
        		ps.parse(text);
        		mszsnap[code].contractUnit = ps.getMantissa() * pow(10.0L, 2 + ps.getExponent());
    		}
    		else
    		{
    			ERROR("ContractUnit is not in the option!");
    			exit(1);
    		}
    	}
    	else if (name == "PrevClearingPrice" && nodeChild->GetText() != 0)
    	{
    		if (mszsnap[code].securityType == 29 || mszsnap[code].securityType == 30)
    		{
        		ps.parse(text);
        		mszsnap[code].contractUnit = ps.getMantissa() * pow(10.0L, 4 + ps.getExponent());
    		}
    		else
    		{
    			ERROR("PrevClearingPrice is not in the option!");
    			exit(1);
    		}
    	}
    	else if (name == "ContractPosition" && nodeChild->GetText() != 0)
    	{
    		if (mszsnap[code].securityType == 29 || mszsnap[code].securityType == 30)
    		{
        		ps.parse(text);
        		mszsnap[code].contractPosition = ps.getMantissa() * pow(10.0L, 2 + ps.getExponent());
    		}
    		else
    		{
    			ERROR("ContractPosition is not in the option!");
    			exit(1);
    		}
    	}
    	//index
    	else if (name == "PrevCloseIdx" && nodeChild->GetText() != 0)
    	{
			ps.parse(text);
			mszsnap[code].prevClosePx = ps.getMantissa() * pow(10.0L, 5 + ps.getExponent());
			mszsnap[code].securityType = -1;
    	}
    	//cash auction
    	else if (name == "BuyQtyUpperLimit" && nodeChild->GetText() != 0)
    	{
			ps.parse(text);
			mszsnap[code].buyQtyUpperLimit = ps.getMantissa() * pow(10.0L, 2 + ps.getExponent());
    	}
    	else if (name == "SellQtyUpperLimit" && nodeChild->GetText() != 0)
    	{
			ps.parse(text);
			mszsnap[code].sellQtyUpperLimit = ps.getMantissa() * pow(10.0L, 2 + ps.getExponent());
    	}
    	else if (name == "BuyQtyUnit" && nodeChild->GetText() != 0)
    	{
			ps.parse(text);
			mszsnap[code].buyQtyUnit = ps.getMantissa() * pow(10.0L, 2 + ps.getExponent());
    	}
    	else if (name == "SellQtyUnit" && nodeChild->GetText() != 0)
    	{
			ps.parse(text);
			mszsnap[code].sellQtyUnit = ps.getMantissa() * pow(10.0L, 2 + ps.getExponent());
    	}
    	else if (name == "PriceTick" && nodeChild->GetText() != 0)
    	{
			ps.parse(text);
			mszsnap[code].priceTick = ps.getMantissa() * pow(10.0L, 4 + ps.getExponent());
    	}
    	else if (name == "Setting")
    	{
    		setting = width;
    	}
    	else if (name == "Type" && nodeChild->GetText() != 0)
    	{
    		strncpy(mszsnap[code].priceLimitSetting[setting - 1].type, trim(text).c_str(), sizeof(mszsnap[code].priceLimitSetting[setting - 1].type));
    	}
    	else if (name == "HasPriceLimit" && nodeChild->GetText() != 0)
    	{
    		strncpy(mszsnap[code].priceLimitSetting[setting - 1].hasPriceLimit, trim(text).c_str(),
    				sizeof(mszsnap[code].priceLimitSetting[setting - 1].hasPriceLimit));
    	}
    	else if (name == "ReferPriceType" && nodeChild->GetText() != 0)
    	{
    		strncpy(mszsnap[code].priceLimitSetting[setting - 1].referPriceType, trim(text).c_str(),
    				sizeof(mszsnap[code].priceLimitSetting[setting - 1].referPriceType));
    	}
    	else if (name == "LimitType" && nodeChild->GetText() != 0)
    	{
    		strncpy(mszsnap[code].priceLimitSetting[setting - 1].limitType, trim(text).c_str(),
    				sizeof(mszsnap[code].priceLimitSetting[setting - 1].limitType));
    	}
    	else if (name == "LimitUpRate" && nodeChild->GetText() != 0)
    	{
			ps.parse(text);
			mszsnap[code].priceLimitSetting[setting - 1].limitUpRate = ps.getMantissa() * pow(10.0L, 3 + ps.getExponent());
    	}
    	else if (name == "LimitDownRate" && nodeChild->GetText() != 0)
    	{
			ps.parse(text);
			mszsnap[code].priceLimitSetting[setting - 1].limitDownRate = ps.getMantissa() * pow(10.0L, 3 + ps.getExponent());
    	}
    	else if (name == "LimitUpAbsolute" && nodeChild->GetText() != 0)
    	{
			ps.parse(text);
			mszsnap[code].priceLimitSetting[setting - 1].limitUpAbolute = ps.getMantissa() * pow(10.0L, 4 + ps.getExponent());
    	}
    	else if (name == "LimitDownAbsolute" && nodeChild->GetText() != 0)
    	{
			ps.parse(text);
			mszsnap[code].priceLimitSetting[setting - 1].limitDownAbolute = ps.getMantissa() * pow(10.0L, 4 + ps.getExponent());
    	}
    	else if (name == "HasAuctionLimit" && nodeChild->GetText() != 0)
    	{
    		strncpy(mszsnap[code].priceLimitSetting[setting - 1].hasAuctionLimit, trim(text).c_str(),
    				sizeof(mszsnap[code].priceLimitSetting[setting - 1].hasAuctionLimit));
    	}
    	else if (name == "AuctionLimitType" && nodeChild->GetText() != 0)
    	{
    		strncpy(mszsnap[code].priceLimitSetting[setting - 1].auctionLimitType, trim(text).c_str(),
    				sizeof(mszsnap[code].priceLimitSetting[setting - 1].auctionLimitType));
    	}
    	else if (name == "AuctionUpDownRate" && nodeChild->GetText() != 0)
    	{
			ps.parse(text);
			mszsnap[code].priceLimitSetting[setting - 1].auctionUpDownRate = ps.getMantissa() * pow(10.0L, 3 + ps.getExponent());
    	}
    	else if (name == "AuctionUpDownAbsolute" && nodeChild->GetText() != 0)
    	{
			ps.parse(text);
			mszsnap[code].priceLimitSetting[setting - 1].auctionUpDownAbsolute = ps.getMantissa() * pow(10.0L, 4 + ps.getExponent());
    	}
    	else if (name == "MarketMakerFlag" && nodeChild->GetText() != 0)
    	{
    		strncpy(mszsnap[code].marketMakerFlag, trim(text).c_str(), sizeof(mszsnap[code].marketMakerFlag));
    	}
    	nodeParse(nodeChild, depth, 1);
    	nodeChild = nodeChild->NextSiblingElement();
    	++ width;
    }
    -- depth;
}

bool Sz1v5Reference::parseStaticFile(char* zqjcxx,char* gbsj,char* cwzy,char* cwzb,char* qxsj)
{
	Staticparse staticparse;
	if(!staticparse.zqjcxxStaticParse(zqjcxx, mBasicInfo))
	{
		ERROR("zqjcxx parse failed");
		return false;
	}
	INFO("zqjcxx parse success");
	if(!staticparse.gbsjStaticParse(gbsj, mCapitalData))
	{
		ERROR("gbsj parse failed");
		return false;
	}
	INFO("gbsj parse success");
	if(!staticparse.cwzyStaticParse(cwzy, mFinancialDigest))
	{
		ERROR("cwzy parse failed");
		return false;
	}
	INFO("cwzy parse success");
	if(!staticparse.cwzbStaticParse(cwzb, mFinancialIndex))
	{
		ERROR("cwzb parse failed");
		return false;
	}
	INFO("cwzb parse success");
	if(!staticparse.qxsjStaticParse(qxsj, mWeightData))
	{
		ERROR("qxsj parse failed");
		return false;
	}
	INFO("qxsj parse success");
	return true;
}

bool Sz1v5Reference::plateFileParse(char* path,const char flag)
{
	SZSNAP stock;
	int32_t length = getFileBufferLength(path);
	if (length == -1)
	{
		ERROR("get %s length failed!",path);
		return false;
	}
	FILE* fd = fopen(path, "rb+");
	char* buffer = new char[length];
	fread(buffer, 1, length, fd);
	fclose(fd);

	int row = 0,totalnum = 0,insertnum=0;
	string value;
	std::string buf(buffer);
	boost::char_separator<char> sep("\r\n");
	boost::tokenizer<boost::char_separator<char> > token(buf, sep);
	for (boost::tokenizer<boost::char_separator<char> >::const_iterator const_it = token.begin();
				const_it != token.end(); ++ const_it, ++ row)
	{
		boost::char_separator<char> separator("|");
		boost::tokenizer<boost::char_separator<char> > tkn(*const_it, separator);
		int i = 0;
		memset(&stock, 0, sizeof(SZSNAP));
		if(row == 0)
		{
			for (boost::tokenizer<boost::char_separator<char> >::const_iterator cit = tkn.begin();
						cit != tkn.end(); ++ cit, ++ i)
				{
					const char* p = (*cit).c_str();
					switch(i)
					{
					case 0:
						break;
					case 1:
						break;
					case 2:
						break;
					case 3:
						totalnum = atoi(p);
						break;
					case 4:
						break;
					case 5:
						break;
					case 6:
						if(date != atoi(p))
						{
							ERROR("%s date(%s) is not the current date",path);
							return false;
						}
						break;
					case 7:
						break;
					case 8:
						break;
					default:
						break;
					}
				}
		}
		if(row>0 && row <= totalnum )
		{
			for (boost::tokenizer<boost::char_separator<char> >::const_iterator cit = tkn.begin();
					cit != tkn.end(); ++ cit, ++ i)
			{
				const char* p = (*cit).c_str();
				if(flag != ' ')
				{
					switch(i)
					{
					case 0:
						strncpy(stock.securityID, p, sizeof(stock.securityID)-1);
							break;
					case 1:
						strncpy(stock.symbol, p, sizeof(stock.symbol)-1);
						//cv.convert(const_cast<char*>(p), (*cit).length(), stock.symbol,sizeof(stock.symbol));
						break;
					default:
						break;
					}
				}
				else
				{
					char *securities;
					switch(i)
					{
					case 0:
						strncpy(stock.securityID, p, sizeof(stock.securityID)-1);
							break;
					case 1:
						break;
					case 2:
						length = strlen(p);
						securities = new char[length+1];
						strncpy(securities, p, length);
						getmGnpz(stock.securityID, securities);
						delete[] securities;
						break;
					default:
						break;
					}
				}
			}
			stock.sectiontype = flag;
			insertnum++;
			if(flag != ' ')
				mszsnap[stock.securityID] = stock;
		}
		if(row == totalnum+1)
		{
			/*the last line*/
			/*check data*/
			for (boost::tokenizer<boost::char_separator<char> >::const_iterator cit = tkn.begin();
								cit != tkn.end(); ++ cit, ++ i)
			{
				const char* p = (*cit).c_str();
				switch(i)
				{
				case 0:
					if(strncmp(p, "TRAILER", 7) != 0)
					{
						ERROR("%s trailer format error!",path);
						delete[] buffer;
						return false;
					}
					break;
				case 1:
					break;
				default:
					break;
				}
			}
		}
		if(row > totalnum+1)
		{
			break;
		}
	}
	delete[] buffer;
	if(insertnum != totalnum)
	{
		ERROR("%s check record num failed!",path);
		return false;
	}
	return true;
}

void Sz1v5Reference::getmGnpz(char *conceptcode , char* securities)
{
	char code[16] = {0};
	char *ptr = strtok(securities,",");
	while(ptr != NULL)
	{
		if(strstr(ptr,"sz"))
		{
			strncpy(code, ptr, 6);
			mgnpz[code] += conceptcode;
			mgnpz[code] += ",";
		}
		ptr = strtok(NULL, ",");
	}
}
