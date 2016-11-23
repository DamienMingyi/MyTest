/*
 * Sh2Reference.cpp
 *
 *  Created on: Apr 8, 2014
 *      Author: level2
 */

/**
 * @file Sh2Reference.cpp
 *
 * @brief sh2 reference file
 * @note read the reference file and store them into shared memory
 *
 * @author ygao@sse.com.cn
 * @author jiachen@sse.com.cn
 *
 * @version 1.0
 * @date 2014-5-21
 * @bug
 * @warning
 */
#include "Sh2Reference.h"
#include <iostream>
#include <fstream>
#include <memory.h>
#include <time.h>
#include <iconv.h>
#include "../../util/shm/CShmEnv.h"
#include "../../public/program/Logger.h"
/**
 * Sh2Reference constructor
 */
Sh2Reference::Sh2Reference() {
	// TODO Auto-generated constructor stub
}
/**
 * Sh2Reference destructor
 */
Sh2Reference::~Sh2Reference() {
	// TODO Auto-generated destructor stub
}

/**
 * push the vector storing the reference data into shared memory
 * @param [in] path the path where the reference file exists
 * @param [in] shm_name �����ڴ��ʶ�ļ�·�����ļ���
 *
 * @return bool
 * -true initialization successful
 * -false initialization failed
 *
 * @bug
 */
bool Sh2Reference::initial(const char* referencePath, const char* shm)
{
	time_t tt = time(NULL);
	tm *time = localtime(&tt);
	//current date and time
	date=(time->tm_year + 1900)*10000+ (time->tm_mon + 1)*100+ time->tm_mday;
	initime=time->tm_hour*10000+time->tm_min*100+time->tm_sec;

	char fileName_cpxx[256];
	memset(fileName_cpxx, 0, 256);
	char fileName_mktdt00[256];
	memset(fileName_mktdt00, 0, 256);
	char fileName_fjy[256];
	memset(fileName_fjy, 0, 256);
	char fileName_dbp[256];
	memset(fileName_dbp, 0, 256);
	char fileName_gzlx[256];
	memset(fileName_gzlx, 0, 256);
	char fileName_zqjcxx[256];
	memset(fileName_zqjcxx, 0, 256);
	char fileName_gbsj[256];
	memset(fileName_gbsj, 0, 256);
	char fileName_cwzy[256];
	memset(fileName_cwzy, 0, 256);
	char fileName_cwzb[256];
	memset(fileName_cwzb, 0, 256);
	char fileName_qxsj[256];
	memset(fileName_qxsj, 0, 256);
	char fileName_dqpz[256];
	memset(fileName_dqpz, 0, 256);
	char fileName_hypz[256];
	memset(fileName_hypz, 0, 256);
	char fileName_gnpz[256];
	memset(fileName_gnpz, 0, 256);

	sprintf(fileName_cpxx, "%s/cpxx%02d%02d.txt", referencePath, date % 10000 / 100, date % 100);
	sprintf(fileName_mktdt00,"%s/mktdt00.txt",referencePath);
	sprintf(fileName_fjy,"%s/fjy%04d%02d%02d.txt",referencePath, date % 100000000 / 10000, date % 10000 / 100, date % 100);
	sprintf(fileName_dbp,"%s/dbp%02d%02d.txt",referencePath, date % 10000 / 100, date % 100);
	sprintf(fileName_gzlx,"%s/gzlx.%x%02d",referencePath,date % 10000 / 100, date % 100);
	sprintf(fileName_zqjcxx,"%s/SH_ZQJCXX.txt",referencePath);
	sprintf(fileName_gbsj,"%s/SH_GBSJ.txt",referencePath);
	sprintf(fileName_cwzy,"%s/SH_CWZY.txt",referencePath);
	sprintf(fileName_cwzb,"%s/SH_CWZB.txt",referencePath);
	sprintf(fileName_qxsj,"%s/SH_QXSJ.txt",referencePath);
	sprintf(fileName_dqpz,"%s/DQPZ.txt",referencePath);
	sprintf(fileName_hypz,"%s/HYPZ.txt",referencePath);
	sprintf(fileName_gnpz,"%s/GNPZ.txt",referencePath);

	Staticparse staticparse;

	if(!staticparse.cpxxStaticParse(fileName_cpxx, mCpxx))
	{
		ERROR("cpxx parse failed");
		return false;
	}
	INFO("cpxx parse success");
	if(!staticparse.dbpStaticParse(fileName_dbp, mDbp))
	{
		ERROR("dbp parse failed");
		return false;
	}
	INFO("dbp parse success");
	if(!staticparse.gzlxStaticParse(fileName_gzlx, mGzlx))
	{
		ERROR("gzlx parse failed");
		return false;
	}
	INFO("gzlx parse success");
	if(!staticparse.zqjcxxStaticParse(fileName_zqjcxx, mBasicInfo))
	{
		ERROR("zqjcxx parse failed");
		return false;
	}
	INFO("zqjcxx parse success");
	if(!staticparse.gbsjStaticParse(fileName_gbsj, mCapitalData))
	{
		ERROR("gbsj parse failed");
		return false;
	}
	INFO("gbsj parse success");
	if(!staticparse.cwzyStaticParse(fileName_cwzy, mFinancialDigest))
	{
		ERROR("cwzy parse failed");
		return false;
	}
	INFO("cwzy parse success");
	if(!staticparse.cwzbStaticParse(fileName_cwzb, mFinancialIndex))
	{
		ERROR("cwzb parse failed");
		return false;
	}
	INFO("cwzb parse success");
	if(!staticparse.qxsjStaticParse(fileName_qxsj, mWeightData))
	{
		ERROR("qxsj parse failed");
		return false;
	}
	INFO("qxsj parse success");
	if(!plateFileParse(fileName_gnpz, ' '))
	{
		ERROR("gnpz parse failed");
		return false;
	}
	if(!fjyStaticParse(fileName_fjy))
	{
		ERROR("fjy parse failed");
		return false;
	}
	INFO("fjy parse success");
	if(!mktdt00StaticParse(fileName_mktdt00))
	{
		ERROR("mktdt00 parse failed");
		return false;
	}
	INFO("mktdt00 parse success");
	if(!plateFileParse(fileName_dqpz,'A'))
	{
		ERROR("dqpz parse failed");
		return false;
	}
	INFO("dqpz parse success");
	if(!plateFileParse(fileName_hypz,'I'))
	{
		ERROR("hypz parse success");
		return false;
	}
	INFO("hypz parse success");
	if(!plateFileParse(fileName_gnpz,'C'))
	{
		ERROR("gnpz parse failed");
		return false;
	}
	INFO("gnpz parse success");
	if(vStock.empty())
		return false;

	CShmEnv shmEnv;
	if (shmEnv.openShmEnv(shm)!=SUCCEED)
	{
		ERROR("openShmEnv(%s) error",shm);
		return false;
	}
	StockSnap2 stocksnap;
	Market market("sh2",&shmEnv,&stocksnap);
	if (!market.load(sizeof(STOCKSNAP2),sizeof(STOCKLINE2),sizeof(STOCKTRADE2), sizeof(STOCK2TRADE2)))
	{
		ERROR("market load error");
		return false;
	}
	for (unsigned int i=1;i<vStock.size();++i)
	{
		market.initSnap(i,(char*)&vStock[i]);
	}
	ST_SHM_ENV_INFO envInfo;
	SHM_OBJ_INFO_LIST objInfoList;
	if (shmEnv.getEnvInfo(envInfo) && shmEnv.getShmObjInfo(objInfoList))
	{
		vStock[0].hand=envInfo.envSize*100/envInfo.envCapacity;
		vStock[0].out=objInfoList.size();
	}
	vStock[0].date=date;
	vStock[0].time=initime;
	vStock[0].volume=vStock.size();
	market.initSnap(0,(char*)&vStock[0]);
	market.refresh();
	return true;
}

bool Sh2Reference::fjyStaticParse(char* path)
{
	int32_t length = getFileBufferLength(path);
	if (length == -1)
	{
		ERROR("get fjy length failed!");
		return false;
	}
	FILE* fd = fopen(path, "rb+");
	char* buffer = new char[length];
	fread(buffer, 1, length, fd);
	fclose(fd);

	STOCKSNAP2 stock;
	memset(&stock, 0, sizeof(STOCKSNAP2));
	strcpy(stock.securityID, "000000");
	vStock.push_back(stock);
	char code[16] = {0};
	std::string buf(buffer);

	boost::char_separator<char> sep("\r\n");
	boost::tokenizer<boost::char_separator<char> > token(buf, sep);
	for (boost::tokenizer<boost::char_separator<char> >::const_iterator const_it = token.begin();
				const_it != token.end(); ++ const_it)
	{
		boost::char_separator<char> separator("|");
		boost::tokenizer<boost::char_separator<char> > tkn(*const_it, separator);
		int i = 0;
		for (boost::tokenizer<boost::char_separator<char> >::const_iterator cit = tkn.begin();
				cit != tkn.end(); ++ cit, ++ i)
		{
			const char* p = (*cit).c_str();
			switch(i)
			{
			case 0:
				break;
			case 1:
				if(strcmp(code, p) == 0) goto MKTDT00END;
				strncpy(stock.securityID, p, sizeof(stock.securityID)-1);
				strncpy(code, p, sizeof(code) -1);
				break;
			case 2:
				cv.convert(const_cast<char*>(p), (*cit).length(), stock.symbol,sizeof(stock.symbol));
				//strncpy(stock.symbol, p, sizeof(stock.symbol)-1);
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				strncpy(stock.fjy.nonTradeType, p, sizeof(stock.fjy.nonTradeType)-1);
				break;
			case 6:
				stock.fjy.nonTradeOrderBeginDate = atoi(p);
				break;
			case 7:
				stock.fjy.nonTradeOrderEndDate = atoi(p);
				break;
			case 8:
				break;
			case 9:
				break;
			case 10:
				break;
			case 11:
				if(p)
				{
					ps.parse(*cit);
					stock.fjy.nonTradePx = ps.getMantissa() * pow(10.0L, 5+ps.getExponent());
				}
				break;
			case 12:
				stock.fjy.ipoQuantum = atol(p);
				break;
			case 13:
				strncpy(stock.fjy.ipoDistributeMethod, p, sizeof(stock.fjy.ipoDistributeMethod)-1);
				break;
			case 14:
				break;
			case 15:
				break;
			case 16:
				break;
			case 17:
				if(p)
				{
					ps.parse(*cit);
					stock.fjy.ipoDownLimitPx = ps.getMantissa() * pow(10.0L, 3+ps.getExponent());
				}
				break;
			case 18:
				if(p)
				{
					ps.parse(*cit);
					stock.fjy.ipoUpLimitPx = ps.getMantissa() * pow(10.0L, 3+ps.getExponent());
				}
				break;
			case 19:
				break;
			case 20:
				stock.fjy.registerDate = atoi(p);
				break;
			case 21:
				stock.fjy.DRDate = atoi(p);
				break;
			case 22:
				if(p)
				{
					ps.parse(*cit);
					stock.fjy.rightIssueRate = ps.getMantissa() * pow(10.0L, 6+ps.getExponent());
				}
				break;
			case 23:
				stock.fjy.rightIssueQuantum = atoi(p);
				break;
			case 24:
				if(p)
				{
					ps.parse(*cit);
					stock.fjy.t2iopv = ps.getMantissa() * pow(10.0L, 5+ps.getExponent());
				}
				break;
			case 25:
				if(p)
				{
					ps.parse(*cit);
					stock.fjy.t1iopv = ps.getMantissa() * pow(10.0L, 5+ps.getExponent());
				}
				break;
			case 26:
				strncpy(stock.fjy.offeringMethod, p, sizeof(stock.fjy.offeringMethod)-1);
				break;
			case 27:
				break;
			default:
				break;
				}
			}
/*
		INFO("[%s][%s][%d] [%d][%ld][%ld] [%s][%ld][%ld] [%d][%d][%ld][%d] [%ld][%ld][%s]",\
				stock.securityID,stock.fjy.nonTradeType,stock.fjy.nonTradeOrderBeginDate,\
				stock.fjy.nonTradeOrderEndDate,stock.fjy.nonTradePx,stock.fjy.ipoQuantum,\
				stock.fjy.ipoDistributeMethod,stock.fjy.ipoDownLimitPx,stock.fjy.ipoUpLimitPx,\
				stock.fjy.registerDate,stock.fjy.DRDate,stock.fjy.rightIssueRate,stock.fjy.rightIssueQuantum,\
				stock.fjy.t2iopv,stock.fjy.t1iopv,stock.fjy.offeringMethod);
*/
		vStock.push_back(stock);
MKTDT00END:
		memset(&stock, 0, sizeof(STOCKSNAP2));
	}
	delete[] buffer;
	return true;
}

bool Sh2Reference::mktdt00StaticParse(char* path)
{
	STOCKSNAP2 stock;
	int32_t length = getFileBufferLength(path);
	if (length == -1)
	{
		ERROR("get mktdt00 length failed!");
		return false;
	}
	FILE* fd = fopen(path, "rb+");
	char* buffer = new char[length];
	fread(buffer, 1, length, fd);
	fclose(fd);

	int row = 0,totalnum = 0,cnt = 0;

	memset(&stock, 0, sizeof(STOCKSNAP2));
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
		if(row == 0)
		{
			for (boost::tokenizer<boost::char_separator<char> >::const_iterator cit = tkn.begin();
								cit != tkn.end(); ++ cit, ++ i)
			{
				const char* p = (*cit).c_str();
				char tmpdate[8+1] = {0};
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
					snprintf(tmpdate, sizeof(tmpdate), "%d", date);
					if(strncmp(p,tmpdate,8))
					{
						ERROR("mktdt00's data is not the current data");
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
		if(row > 0 && row <= totalnum)
		{
			cnt++;
			for (boost::tokenizer<boost::char_separator<char> >::const_iterator cit = tkn.begin();
					cit != tkn.end(); ++ cit, ++ i)
			{
				const char* p = (*cit).c_str();
				switch(i)
				{
				case 0:
						break;
				case 1:
						strncpy(stock.securityID, p, sizeof(stock.securityID)-1);
						break;
				case 2:
					cv.convert(const_cast<char*>(p), (*cit).length(), stock.symbol,sizeof(stock.symbol));
						break;
				case 3:
						break;
				case 4:
						break;
				case 5:
						break;
				case 6:
						break;
				default:
						break;
				}
			}

			if(mCpxx.find(stock.securityID) != mCpxx.end())
			{
				strncpy(stock.cpxx.basicSecurityCode, mCpxx[stock.securityID].basicSecurityCode, sizeof(mCpxx[stock.securityID].basicSecurityCode));
				strncpy(stock.cpxx.marketType, mCpxx[stock.securityID].marketType,sizeof(mCpxx[stock.securityID].marketType));
				strncpy(stock.cpxx.type, mCpxx[stock.securityID].type,sizeof(mCpxx[stock.securityID].type));
				strncpy(stock.cpxx.subtype, mCpxx[stock.securityID].subtype,sizeof(mCpxx[stock.securityID].subtype));
				strncpy(stock.cpxx.currency, mCpxx[stock.securityID].currency,sizeof(mCpxx[stock.securityID].currency));
				stock.cpxx.parValue = mCpxx[stock.securityID].parValue;
				stock.cpxx.lastTradingDate = mCpxx[stock.securityID].lastTradingDate;
				stock.cpxx.buyNum = mCpxx[stock.securityID].buyNum;
				stock.cpxx.sellNum = mCpxx[stock.securityID].sellNum;
				stock.cpxx.applyUpimit = mCpxx[stock.securityID].applyUpimit;
				stock.cpxx.applyDownlimit = mCpxx[stock.securityID].applyDownlimit;
				stock.cpxx.priceLevel = mCpxx[stock.securityID].priceLevel;
				stock.cpxx.listingDate = mCpxx[stock.securityID].listingDate;
				stock.cpxx.XRRate = mCpxx[stock.securityID].XRRate;
				stock.cpxx.XDAmount = mCpxx[stock.securityID].XDAmount;
				strncpy(stock.cpxx.financialTag, mCpxx[stock.securityID].financialTag,sizeof(mCpxx[stock.securityID].financialTag));
				strncpy(stock.cpxx.securityLendingTag, mCpxx[stock.securityID].securityLendingTag,sizeof(mCpxx[stock.securityID].securityLendingTag));
				strncpy(stock.cpxx.productStatus, mCpxx[stock.securityID].productStatus,sizeof(mCpxx[stock.securityID].productStatus));
				strncpy(stock.cpxx.upDownLimitType, mCpxx[stock.securityID].upDownLimitType,sizeof(mCpxx[stock.securityID].upDownLimitType));
				stock.cpxx.upLimitPx = mCpxx[stock.securityID].upLimitPx;
				stock.cpxx.downLimitPx = mCpxx[stock.securityID].downLimitPx;
			}
			if(mGzlx.find(stock.securityID) != mGzlx.end())
			{
				stock.gzlx.interestDate = mGzlx[stock.securityID].interestDate;
				stock.gzlx.interestPerHundred = mGzlx[stock.securityID].interestPerHundred;
				stock.gzlx.daysOfInterest = mGzlx[stock.securityID].daysOfInterest;
				stock.gzlx.couponRate = mGzlx[stock.securityID].couponRate;
			}
			if(mDbp.find(stock.securityID) != mDbp.end())
			{
				stock.dbp.finance = mDbp[stock.securityID].finance;
				stock.dbp.securityLending = mDbp[stock.securityID].securityLending;
			}
			if(mBasicInfo.find(stock.securityID) != mBasicInfo.end())
			{
				strncpy(stock.basicinfo.phoneticize,mBasicInfo[stock.securityID].phoneticize,sizeof(mBasicInfo[stock.securityID].phoneticize));
				strncpy(stock.basicinfo.isin,mBasicInfo[stock.securityID].isin,sizeof(mBasicInfo[stock.securityID].isin));
				strncpy(stock.basicinfo.industyCode,mBasicInfo[stock.securityID].industyCode,sizeof(mBasicInfo[stock.securityID].industyCode));
				strncpy(stock.basicinfo.regionCode,mBasicInfo[stock.securityID].regionCode,sizeof(mBasicInfo[stock.securityID].regionCode));
				//strncpy(stock.basicinfo.exchangeBoard,mBasicInfo[stock.securityID].exchangeBoard,sizeof(mBasicInfo[stock.securityID].exchangeBoard));
				//strncpy(stock.basicinfo.selfBoard,mBasicInfo[stock.securityID].selfBoard,sizeof(mBasicInfo[stock.securityID].selfBoard));
				//strncpy(stock.basicinfo.currency,mBasicInfo[stock.securityID].currency,sizeof(mBasicInfo[stock.securityID].currency));
			}
			if(mgnpz.find(stock.securityID) != mgnpz.end())
			{
				strncpy(stock.basicinfo.conceptCode, mgnpz[stock.securityID].c_str(), strlen(mgnpz[stock.securityID].c_str())-2);
			}
			if(mCapitalData.find(stock.securityID) != mCapitalData.end())
			{
				stock.capitaldata.adjustingDate = mCapitalData[stock.securityID].adjustingDate;
				stock.capitaldata.totalCapital = mCapitalData[stock.securityID].totalCapital;
				stock.capitaldata.nonTradableShare = mCapitalData[stock.securityID].nonTradableShare;
				stock.capitaldata.tradableShare = mCapitalData[stock.securityID].tradableShare;
				stock.capitaldata.aTradableShare = mCapitalData[stock.securityID].aTradableShare;
				stock.capitaldata.bTradableShare = mCapitalData[stock.securityID].bTradableShare;
				stock.capitaldata.hTradableShare = mCapitalData[stock.securityID].hTradableShare;
			}
			if(mFinancialDigest.find(stock.securityID) != mFinancialDigest.end())
			{
				stock.financialDigest.totalAsset = mFinancialDigest[stock.securityID].totalAsset;
				stock.financialDigest.liquidAsset = mFinancialDigest[stock.securityID].liquidAsset;
				stock.financialDigest.fixedAsset = mFinancialDigest[stock.securityID].fixedAsset;
				//stock.financialDigest.longTermAsset = mFinancialDigest[stock.securityID].longTermAsset;
				stock.financialDigest.intangibleAsset = mFinancialDigest[stock.securityID].intangibleAsset;
				stock.financialDigest.longTermDebt = mFinancialDigest[stock.securityID].longTermDebt;
				stock.financialDigest.currentLiabilities = mFinancialDigest[stock.securityID].currentLiabilities;
				stock.financialDigest.shareHolderEquity = mFinancialDigest[stock.securityID].shareHolderEquity;
				stock.financialDigest.capitalReserve = mFinancialDigest[stock.securityID].capitalReserve;
				stock.financialDigest.mainBusinessIncome = mFinancialDigest[stock.securityID].mainBusinessIncome;
				stock.financialDigest.mainBusinessProfit = mFinancialDigest[stock.securityID].mainBusinessProfit;
				stock.financialDigest.totalProfit = mFinancialDigest[stock.securityID].totalProfit;
				stock.financialDigest.netProfit = mFinancialDigest[stock.securityID].netProfit;
				stock.financialDigest.undistributedProfit = mFinancialDigest[stock.securityID].undistributedProfit;
				stock.financialDigest.operatingNetCashFlow = mFinancialDigest[stock.securityID].operatingNetCashFlow;
				stock.financialDigest.investingNetCashFlow = mFinancialDigest[stock.securityID].investingNetCashFlow;
				stock.financialDigest.financingNetCashFlow = mFinancialDigest[stock.securityID].financingNetCashFlow;
				stock.financialDigest.cashFlowIncrease = mFinancialDigest[stock.securityID].cashFlowIncrease;
				//stock.financialDigest.shareholderNumber = mFinancialDigest[stock.securityID].shareholderNumber;
			}
			if(mFinancialIndex.find(stock.securityID) != mFinancialIndex.end())
			{
				stock.financialIndex.earningPerShare = mFinancialIndex[stock.securityID].earningPerShare;
				stock.financialIndex.netAssetPerShare = mFinancialIndex[stock.securityID].netAssetPerShare;
				stock.financialIndex.operatingCashFlowPerShare = mFinancialIndex[stock.securityID].operatingCashFlowPerShare;
				stock.financialIndex.undistributedProfitPerShare = mFinancialIndex[stock.securityID].undistributedProfitPerShare;
				stock.financialIndex.operatingNetProfitRate = mFinancialIndex[stock.securityID].operatingNetProfitRate;
				stock.financialIndex.operatingGrossProfitRate = mFinancialIndex[stock.securityID].operatingGrossProfitRate;
				stock.financialIndex.mainBusinessProfitRate = mFinancialIndex[stock.securityID].mainBusinessProfitRate;
				stock.financialIndex.netProfitRate = mFinancialIndex[stock.securityID].netProfitRate;
				stock.financialIndex.netReturnOnAssets = mFinancialIndex[stock.securityID].netReturnOnAssets;
				stock.financialIndex.currentRatio = mFinancialIndex[stock.securityID].currentRatio;
				stock.financialIndex.quickRatio = mFinancialIndex[stock.securityID].quickRatio;
				stock.financialIndex.assetLiabilityRatio = mFinancialIndex[stock.securityID].assetLiabilityRatio;
				//stock.financialIndex.debt2EquityRatio = mFinancialIndex[stock.securityID].debt2EquityRatio;
				stock.financialIndex.equityRatio = mFinancialIndex[stock.securityID].equityRatio;
				stock.financialIndex.turnoverOfAccountReceivable = mFinancialIndex[stock.securityID].turnoverOfAccountReceivable;
				stock.financialIndex.stockTurnover = mFinancialIndex[stock.securityID].stockTurnover;
				stock.financialIndex.mainRevenueGrowthRate = mFinancialIndex[stock.securityID].mainRevenueGrowthRate;
				stock.financialIndex.netProfitGrowthRate = mFinancialIndex[stock.securityID].netProfitGrowthRate;
				stock.financialIndex.EPSG = mFinancialIndex[stock.securityID].EPSG;
				stock.financialIndex.growthRateOfShareholder = mFinancialIndex[stock.securityID].growthRateOfShareholder;
				stock.financialIndex.MPIP = mFinancialIndex[stock.securityID].MPIP;
				stock.financialIndex.totalAssetsGrowthRate = mFinancialIndex[stock.securityID].totalAssetsGrowthRate;
			}
		/*
		if(mWeightData.find(stock.securityID) != mWeightData.end())
		{
			stock.weightedData.date = mWeightData[stock.securityID].date;
			strncpy(stock.weightedData.type, mWeightData[stock.securityID].type,sizeof(mWeightData[stock.securityID].type));
			stock.weightedData.rate = mWeightData[stock.securityID].rate;
			stock.weightedData.price = mWeightData[stock.securityID].price;
		}
		 */
		int code=atoi(stock.securityID);
		if ((code>=90000 && code<=99999)||
			(code>=99000 && code<=99999)||
			(code>=104000 && code<=107999))
			continue;
		vStock.push_back(stock);
		memset(&stock, 0, sizeof(STOCKSNAP2));
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
						ERROR("mktdt00's trailer format error!");
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
	if(cnt != totalnum)
	{
		ERROR("check record num(%d) failed!",cnt);
		return false;
	}
	return true;
}

bool Sh2Reference::plateFileParse(char* path,const char flag)
{
	STOCKSNAP2 stock;
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
		memset(&stock, 0, sizeof(STOCKSNAP2));
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
				vStock.push_back(stock);
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

void Sh2Reference::getmGnpz(char *conceptcode , char* securities)
{
	char code[16] = {0};
	char *ptr = strtok(securities,",");
	while(ptr != NULL)
	{
		if(strstr(ptr,"sh"))
		{
			strncpy(code, ptr, 6);
			mgnpz[code] += conceptcode;
			mgnpz[code] += ",";
		}
		ptr = strtok(NULL, ",");
	}
}
