/*
 * FixedIncomeReference.cpp
 *
 *  Created on: Sep 29, 2016
 *      Author: level2
 */

#include "FixedIncomeReference.h"

FixedIncomeReference::FixedIncomeReference() {
	// TODO Auto-generated constructor stub

}

FixedIncomeReference::~FixedIncomeReference() {
	// TODO Auto-generated destructor stub
}

bool FixedIncomeReference::initial(const char* ref_path, const char* shm_path)
{
	time_t tt = time(NULL);
	tm *time = localtime(&tt);
	//current date and time
	date = (time->tm_year + 1900) * 10000 + (time->tm_mon + 1)*100 + time->tm_mday;
	initime = time->tm_hour * 10000 + time->tm_min*100 + time->tm_sec;

	char filename[256];
	sprintf(filename,"%s/se015zqxx%d001.txt",ref_path,date);

	if (getInitialSnap(filename))
		return false;

	if (vfiSnap.empty())
		return false;

	CShmEnv shmEnv;
	if (shmEnv.openShmEnv(shm_path) != SUCCEED)
	{
		ERROR("openShmEnv(%s) error", shm_path);
		return false;
	}

	FixedIncomeSnap fixIncomeSnap;
	Market2 market("shfi", &shmEnv, &fixIncomeSnap);
	if (!market.load(sizeof(SHFISNAP),sizeof(SHFILINE),sizeof(SHFITRADE), sizeof(TICK)))
	{
		ERROR("market load error");
		return false;
	}
	for (unsigned int i = 1;i < vfiSnap.size(); ++ i)
	{
		market.initSnap(i,(char*)&vfiSnap[i]);
	}
	ST_SHM_ENV_INFO envInfo;
	SHM_OBJ_INFO_LIST objInfoList;
	if (shmEnv.getEnvInfo(envInfo) && shmEnv.getShmObjInfo(objInfoList))
	{
		vfiSnap[0].hand = envInfo.envSize*100/envInfo.envCapacity;
		vfiSnap[0].out = objInfoList.size();
	}
	vfiSnap[0].date = date;
	vfiSnap[0].time = initime;
	vfiSnap[0].amount = vfiSnap.size();
	market.initSnap(0,(char*)&vfiSnap[0]);
	market.refresh();
	return true;
}

bool FixedIncomeReference::getInitialSnap(const char* zqxxPath)
{
	int totalNumberInFile = 0;
	int dateInFile = 0;
	int32_t length = getFileBufferLength(zqxxPath);
	if (length == -1)
	{
		ERROR("get reff03 length failed!");
		return false;
	}
	FILE* fd = fopen(zqxxPath, "rb+");
	char* buffer = new char[length];
	fread(buffer, 1, length, fd);
	fclose(fd);
	SHFISNAP fixedIncome;
	memset(&fixedIncome, 0, sizeof(SHFISNAP));
	strncpy(fixedIncome.securityID, "000000", sizeof(fixedIncome.securityID));
	vfiSnap.push_back(fixedIncome);

	int row = 0;
	std::string buf(buffer);
	boost::char_separator<char> sep("\r\n");
	boost::tokenizer<boost::char_separator<char> > token(buf, sep);
	for (boost::tokenizer<boost::char_separator<char> >::const_iterator const_it = token.begin();
				const_it != token.end(); ++ const_it, ++ row)
	{
		boost::char_separator<char> separator("|");
		boost::tokenizer<boost::char_separator<char> > tkn(*const_it, separator);
		int i = 0;
		if (row == 0)
		{
			for (boost::tokenizer<boost::char_separator<char> >::const_iterator cit = tkn.begin();
					cit != tkn.end(); ++ cit, ++ i)
			{
				switch(i)
				{
				case 0:
					dateInFile = (*cit).substr(0, 4) * 10000 + (*cit).substr(5, 2) * 100 + (*cit).substr(8, 2);
					break;
				case 1:
					totalNumberInFile = atoi((*cit).c_str());
					break;
				default:
					break;
				}
			}
		}
		else
		{
			for (boost::tokenizer<boost::char_separator<char> >::const_iterator cit = tkn.begin();
					cit != tkn.end(); ++ cit, ++ i)
			{
				const char* p = (*cit).c_str();
				switch(i)
				{
				case 0:
					strncpy(fixedIncome.securityID, p, sizeof(fixedIncome.securityID));
					break;
				case 1:
					cv.convert(const_cast<char*>(p), (*cit).length(), fixedIncome.symbol,
							sizeof(fixedIncome.symbol));
					break;
				case 2:
					strncpy(fixedIncome.tradeProduct, p, sizeof(fixedIncome.tradeProduct));
					break;
				case 3:
					strncpy(fixedIncome.attribute, p, sizeof(fixedIncome.attribute));
					break;
				case 4:
					strncpy(fixedIncome.status, p, sizeof(fixedIncome.status));
					break;
				case 5:
					strncpy(fixedIncome.pledgeCode, p, sizeof(fixedIncome.pledgeCode));
					break;
				case 6:
					fixedIncome.openTime = atoi(p);
					break;
				case 7:
					fixedIncome.closeTime = atoi(p);
					break;
				case 8:
					strncpy(fixedIncome.offeringWay, p, sizeof(fixedIncome.offeringWay));
					break;
				case 9:
					fixedIncome.denomination = atoi(p);
					break;
				case 10:
					fixedIncome.offeringPx = atoi(p);
					break;
				case 11:
					strncpy(fixedIncome.interestType, p, sizeof(fixedIncome.interestType));
					break;
				case 12:
					strncpy(fixedIncome.payingFrequence, p, sizeof(fixedIncome.payingFrequence));
					break;
				case 13:
					fixedIncome.nomialInterstRate = atoi(p);
					break;
				case 14:
					fixedIncome.basicInterestRate = atoi(p);
					break;
				case 15:
					fixedIncome.basicInterestMargin = atoi(p);
					break;
				case 16:
					fixedIncome.deadline = atoi(p);
					break;
				case 17:
					fixedIncome.offeringVolume = atoi(p);
					break;
				case 18:
					fixedIncome.offeringBeginDate = atoi(p);
					break;
				case 19:
					fixedIncome.offeringEndDate = atoi(p);
					break;
				case 20:
					fixedIncome.listingDate = atoi(p);
					break;
				case 21:
					fixedIncome.expiringDate = atoi(p);
					break;
				case 22:
					strncpy(fixedIncome.bondType, p, sizeof(fixedIncome.bondType));
					break;
				case 23:
					strncpy(fixedIncome.offeringMethod, p, sizeof(fixedIncome.offeringMethod));
					break;
				case 24:
					strncpy(fixedIncome.transMarket, p, sizeof(fixedIncome.transMarket));
					break;
				case 25:
					strncpy(fixedIncome.sellShort, p, sizeof(fixedIncome.sellShort));
					break;
				case 26:
					fixedIncome.totalShortLimit = atoi(p);
					break;
				case 27:
					fixedIncome.traderShortLimit = atoi(p);
					break;
				case 28:
					fixedIncome.prevClose = atoi(p);
					break;
				case 29:
					fixedIncome.preCloseYield = atoi(p);
					break;
				default:
					break;
				}
			}
			vfiSnap.push_back(fixedIncome);
		}
	}

	delete[] buffer;

	if (vfiSnap.size() != totalNumberInFile + 1)
	{
		ERROR("check recorder number failed");
		return false;
	}
	if (dateInFile == initime)
	{
		ERROR("the date in file is not consistent with today!");
		return false;
	}
	if(!vfiSnap.empty() && strcmp(vfiSnap.front().securityID, "000000") != 0)
		vfiSnap.clear();
	return true;
}
