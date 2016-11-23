/*
 * OptionReference.cpp
 *
 *  Created on: Sep 14, 2016
 *      Author: level2
 */

#include "OptionReference.h"
#include <sys/stat.h>
#include <stdio.h>

OptionReference::OptionReference(): IInitial(), rate(0.0) {
	// TODO Auto-generated constructor stub

}

OptionReference::~OptionReference() {
	// TODO Auto-generated destructor stub
}

bool OptionReference::initial(const char* referencePath, const char* shm)
{
	char rateFileName[256];
	sprintf(rateFileName, "%s/rate.txt", referencePath);
	if (!getRate(rateFileName))
		return false;

	time_t tt = time(NULL);
	tm *time = localtime(&tt);

	date = (time->tm_year + 1900) * 10000+ (time->tm_mon + 1) * 100+ time->tm_mday;
	initime = time->tm_hour * 10000 + time->tm_min * 100 + time->tm_sec;

	char filename[256];
	sprintf(filename,"%s/reff03%02d%02d.txt", referencePath, date % 10000 / 100, date % 10000 % 100);
	if (!getInitialSnap(filename))
		return false;

	CShmEnv shmEnv;
	if (shmEnv.openShmEnv(shm)!=SUCCEED)
	{
		ERROR("openShmEnv(%s) error", shm);
		return false;
	}
	OptionSnap optionsnap;
	Market market("sho", &shmEnv, &optionsnap);
	if (!market.load(sizeof(SHOSNAP), sizeof(SHOLINE), sizeof(SHOTRADE)))
	{
		ERROR("market load error");
		return false;
	}
	for (unsigned int i=1 ; i<voptSnap.size(); ++i)
	{
		market.initSnap(i, (char*)&voptSnap[i]);
	}
	ST_SHM_ENV_INFO envInfo;
	SHM_OBJ_INFO_LIST objInfoList;
	if (shmEnv.getEnvInfo(envInfo) && shmEnv.getShmObjInfo(objInfoList))
	{
		voptSnap[0].hand = envInfo.envSize*100/envInfo.envCapacity;
		voptSnap[0].out = objInfoList.size();
	}
	voptSnap[0].date = date;
	voptSnap[0].time = initime;
	voptSnap[0].amount = voptSnap.size();
	market.initSnap(0, (char*)&voptSnap[0]);
	market.refresh();
	return true;
}

int32_t OptionReference::RemainingDay(int32_t begin, int32_t end)
{
	time_t t0, t1;
	struct tm tmBegin, tmEnd;
	memset(&tmBegin, 0, sizeof(tmBegin));
	memset(&tmEnd, 0, sizeof(tmEnd));
	tmBegin.tm_year = begin / 10000 - 1900;
	tmBegin.tm_mon = (begin % 10000) / 100 - 1;
	tmBegin.tm_mday = begin % 100;
	tmEnd.tm_year = end / 10000 - 1900;
	tmEnd.tm_mon = (end % 10000) / 100 - 1;
	tmEnd.tm_mday = end % 100;
	t0 = mktime(&tmBegin);
	t1 = mktime(&tmEnd);
	return (t1 - t0) / (24 * 3600);
}

bool OptionReference::getRate(const char* ratePath)
{
	int32_t length = getFileBufferLength(ratePath);
	if (length == -1)
	{
		ERROR("get rate file length failed!");
		return false;
	}
	FILE* fd = fopen(ratePath, "rb+");
	char* buffer =  new char[length];
	fread(buffer, 1, length, fd);
	rate = atof(buffer);
	fclose(fd);
	delete[] buffer;
	if(fabs(rate) < 0.000001)
	{
		ERROR("get rate failed");
		return false;
	}
	return true;
}

bool OptionReference::getInitialSnap(const char* reff03Path)
{
	int32_t length = getFileBufferLength(reff03Path);
	if (length == -1)
	{
		ERROR("get reff03 length failed!");
		return false;
	}
	FILE* fd = fopen(reff03Path, "rb+");
	char* buffer = new char[length];
	fread(buffer, 1, length, fd);
	fclose(fd);
	SHOSNAP shosnap;
	memset(&shosnap, 0, sizeof(SHOSNAP));
	strncpy(shosnap.securityID, "00000000", sizeof(shosnap.securityID));
	voptSnap.push_back(shosnap);
	std::string buf(buffer);
	int j = 0;
	boost::char_separator<char> sep("\r\n");
	boost::tokenizer<boost::char_separator<char> > token(buf, sep);
	for (boost::tokenizer<boost::char_separator<char> >::const_iterator const_it = token.begin();
				const_it != token.end(); ++ const_it, ++ j)
	{
		int i = 0;
		memset(&shosnap, 0, sizeof(SHOSNAP));
		boost::char_separator<char> separator("|");
		boost::tokenizer<boost::char_separator<char> > tkn(*const_it, separator);
		for (boost::tokenizer<boost::char_separator<char> >::const_iterator cit = tkn.begin(); cit != tkn.end(); ++ cit, ++ i)
		{
			const char* p = (*cit).c_str();
			switch(i)
			{
			case 0:
				strncpy(shosnap.RFStreamID, p, sizeof(shosnap.RFStreamID));
				break;
			case 1:
				strncpy(shosnap.securityID, p, sizeof(shosnap.securityID));
				break;
			case 2:
				strncpy(shosnap.contractID, p, sizeof(shosnap.contractID));
				break;
			case 3:
				cv.convert(const_cast<char*>(p), (*cit).length(), shosnap.symbol, sizeof(shosnap.symbol));
				break;
			case 4:
				strncpy(shosnap.underlyingSecurityID, p, sizeof(shosnap.underlyingSecurityID));
				break;
			case 5:
				cv.convert(const_cast<char*>(p), (*cit).length(), shosnap.underlyingSymbol, sizeof(shosnap.underlyingSymbol));
				break;
			case 6:
				strncpy(shosnap.underlyingType, p, sizeof(shosnap.underlyingType));
				break;
			case 7:
				strncpy(shosnap.optionType, p, sizeof(shosnap.optionType));
				break;
			case 8:
				strncpy(shosnap.callOrPut, p, sizeof(shosnap.callOrPut));
				break;
			case 9:
				shosnap.contractMultiplierUnit = atoi(p);
				break;
			case 10:
				ps.parse(*cit);
				shosnap.exercisePrice = ps.getMantissa() * pow(10.0L, 4 + ps.getExponent());
				break;
			case 11:
				shosnap.startDate = atoi(p);
				break;
			case 12:
				shosnap.endDate = atoi(p);
				break;
			case 13:
				shosnap.exerciseDate = atoi(p);
				break;
			case 14:
				shosnap.deliveryDate = atoi(p);
				break;
			case 15:
				shosnap.expireDate = atoi(p);
				break;
			case 16:
				strncpy(shosnap.updateVersion, p, sizeof(shosnap.updateVersion));
				break;
			case 17:
				shosnap.totalLongPositon = atoi(p);
				break;
			case 18:
				ps.parse(*cit);
				shosnap.prevClosePx = ps.getMantissa() * pow(10.0L, 4 + ps.getExponent());
				break;
			case 19:
				ps.parse(*cit);
				shosnap.prevSettlePrice = ps.getMantissa() * pow(10.0L, 4 + ps.getExponent());
				break;
			case 20:
				ps.parse(*cit);
				shosnap.underlyingClosePx = ps.getMantissa() * pow(10.0L, 4 + ps.getExponent());
				break;
			case 21:
				strncpy(shosnap.priceLimitType, p, sizeof(shosnap.priceLimitType));
				break;
			case 22:
				ps.parse(*cit);
				shosnap.dailyPriceUpLimit = ps.getMantissa() * pow(10.0L, 4 + ps.getExponent());
				break;
			case 23:
				ps.parse(*cit);
				shosnap.dailyPriceDownLimit = ps.getMantissa() * pow(10.0L, 4 + ps.getExponent());
				break;
			case 24:
				ps.parse(*cit);
				shosnap.marginUnit = ps.getMantissa() * pow(10.0L, 2 + ps.getExponent());
				break;
			case 25:
				ps.parse(*cit);
				shosnap.marginRatioParam1 = ps.getMantissa() * pow(10.0L, 2 + ps.getExponent());
				break;
			case 26:
				ps.parse(*cit);
				shosnap.marginRatioParam2 = ps.getMantissa() * pow(10.0L, 2 + ps.getExponent());
				break;
			case 27:
				shosnap.roundLot = atoi(p);
				break;
			case 28:
				shosnap.lmtOrdMinFloor = atoi(p);
				break;
			case 29:
				shosnap.lmtOrdMaxFloor = atoi(p);
				break;
			case 30:
				shosnap.mktOrdMinFloor = atoi(p);
				break;
			case 31:
				shosnap.mktOrdMaxFloor = atoi(p);
				break;
			case 32:
				ps.parse(*cit);
				shosnap.tickSize = ps.getMantissa() * pow(10.0L, 4 + ps.getExponent());
				break;
			case 33:
				strncpy(shosnap.securityStatusFlag, p, sizeof(shosnap.securityStatusFlag));
				break;
			default:
				break;
			}
		}
		int32_t remainingDays = RemainingDay(date, shosnap.endDate);
		if (remainingDays < 0)
		{
			ERROR("end date error, initial failed!");
			return false;
		}
		shosnap.remainingDays = remainingDays;
		SHOSNAP::riskFreeInterestRate = rate;
		voptSnap.push_back(shosnap);
	}
	delete[] buffer;
	if(!voptSnap.empty() && strcmp(voptSnap[0].securityID, "00000000") != 0)
		voptSnap.clear();
	return true;
}
