/*
 * HKPReference.cpp
 *
 *  Created on: Sep 15, 2015
 *      Author: level2
 */

#include "../../shhk/initial/ShhkReference.h"

#include "../../public/program/Logger.h"
#include "../../util/shm/CShmEnv.h"
#include <boost/tokenizer.hpp>
#include <boost/format.hpp>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <memory.h>
#include <time.h>
#include <sys/stat.h>
#include <stdio.h>
#include <iconv.h>

ShhkReference::ShhkReference() {
	totalNum = 0;
	// TODO Auto-generated constructor stub
}

ShhkReference::~ShhkReference() {
	// TODO Auto-generated destructor stub
}

bool ShhkReference::initial(const char* referencePath, const char* shm)
{
	time_t tt = time(NULL);
	tm *time = localtime(&tt);

	date=(time->tm_year + 1900)*10000+ (time->tm_mon + 1)*100+ time->tm_mday;
	initime=time->tm_hour*10000+time->tm_min*100+time->tm_sec;

	char filename_reff04[256] = {0};
	char filename_mktdt04[256] = {0};

	sprintf(filename_reff04,"%s/reff04%02d%02d.txt",referencePath,date%10000/100,date%10000%100);
	sprintf(filename_mktdt04,"%s/mktdt04.txt",referencePath);

	if(!mktdt04StaticParse(filename_mktdt04))
	{
		ERROR("mktdt04 parse error");
		return false;
	}
	INFO("mktdt04 parse success");
	if (!reff04StaticParse(filename_reff04))
	{
		ERROR("reff04 parse error");
		return false;
	}
	INFO("reff04 parse success");
	if (vHKP.empty())
		return false;
	CShmEnv shmEnv;
	if (shmEnv.openShmEnv(shm)!=SUCCEED)
	{
		ERROR("openShmEnv(%s) error",shm);
		return false;
	}
	HKPressSnap hksnap;
	Market market("shhk",&shmEnv,&hksnap);
	if (!market.load(sizeof(HKPSNAP),sizeof(HKPLINE),sizeof(HKPTRADE)))
	{
		ERROR("market load error");
		return false;
	}
	for (unsigned int i=1;i<vHKP.size();++i)
	{
		memset((char*)&vHKP[i].vcm, 0, sizeof(VCM));
		memset((char*)&vHKP[i].cas, 0, sizeof(CAS));
		market.initSnap(i,(char*)&vHKP[i]);
	}
	ST_SHM_ENV_INFO envInfo;
	SHM_OBJ_INFO_LIST objInfoList;
	if (shmEnv.getEnvInfo(envInfo) && shmEnv.getShmObjInfo(objInfoList))
	{
		vHKP[0].hand=envInfo.envSize*100/envInfo.envCapacity;
		vHKP[0].out=objInfoList.size();
	}
	vHKP[0].date=date;
	vHKP[0].time=initime;
	vHKP[0].amount=vHKP.size();
	market.initSnap(0,(char*)&vHKP[0]);
	market.refresh();
	return true;
}

bool ShhkReference::mktdt04StaticParse(char* ref_path)
{
	int32_t length = getFileBufferLength(ref_path);
	if (length == -1)
	{
		ERROR("get mktdt04 length failed!");
		return false;
	}
	FILE* fd = fopen(ref_path, "rb+");
	char* buffer = new char[length];
	fread(buffer, 1, length, fd);
	fclose(fd);

	int row = 0;
	char code[8] = {0};
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
					totalNum = atoi((*cit).c_str());
					break;
				case 4:
					break;
				case 5:
					break;
				case 6:
					snprintf(tmpdate, sizeof(tmpdate), "%d", date);
					if(strncmp(p,tmpdate,8))
					{
						ERROR("mktdt04's data is not the current data");
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
		else
		{
			for (boost::tokenizer<boost::char_separator<char> >::const_iterator cit = tkn.begin();
					cit != tkn.end(); ++ cit, ++ i)
			{
				const char* p = (*cit).c_str();
				switch(i)
				{
				case 0:
					if(strncmp(p, "TRAILER", 7) == 0||strncmp(p,"MD401",5) != 0)
						goto MKTDT04END;
					break;
				case 1:
					strncpy(code, p, sizeof(code)-1);
					break;
				case 2:
					//cv.convert(const_cast<char*>(p), (*cit).length(), mMktdt04[code].Symbol,
					//								sizeof(mMktdt04[code].Symbol));
					break;
				case 3:
					strncpy(mMktdt04[code].SymbolEn, p, sizeof(mMktdt04[code].SymbolEn)-1);
					break;
				case 4:
					mMktdt04[code].TradeVolume = atoi(p);
					break;
				case 5:
					if(p)
					{
						ps.parse(*cit);
						mMktdt04[code].TotalValueTraded = ps.getMantissa() * pow(10.0L, 3+ps.getExponent());
					}
					break;
				case 6:
					if(p)
					{
						ps.parse(*cit);
						mMktdt04[code].PreClosePx = ps.getMantissa() * pow(10.0L, 3+ps.getExponent());
					}
					break;
				case 7:
					if(p)
					{
						ps.parse(*cit);
						mMktdt04[code].NominalPrice = ps.getMantissa() * pow(10.0L, 3+ps.getExponent());
					}
					break;
				case 8:
					if(p)
					{
						ps.parse(*cit);
						mMktdt04[code].HighPrice = ps.getMantissa() * pow(10.0L, 3+ps.getExponent());
					}
					break;
				case 9:
					if(p)
					{
						ps.parse(*cit);
						mMktdt04[code].LowPrice = ps.getMantissa() * pow(10.0L, 3+ps.getExponent());
					}
					break;
				case 10:
					if(p)
					{
						ps.parse(*cit);
						mMktdt04[code].TradePrice = ps.getMantissa() * pow(10.0L, 3+ps.getExponent());
					}
					break;
				case 11:
					if(p)
					{
						ps.parse(*cit);
						mMktdt04[code].BuyPrice1 = ps.getMantissa() * pow(10.0L, 3+ps.getExponent());
					}
					break;
				case 12:
					mMktdt04[code].BuyVolume1 = atoi(p);
					break;
				case 13:
					if(p)
					{
						ps.parse(*cit);
						mMktdt04[code].SellPrice1 = ps.getMantissa() * pow(10.0L, 3+ps.getExponent());
					}
					break;
				case 14:
					mMktdt04[code].SellVolume1 = atoi(p);
					break;
				case 15:
					strncpy(mMktdt04[code].SecTradingStatus, p, sizeof(mMktdt04[code].SecTradingStatus)-1);
					break;
				case 16:
					strncpy(mMktdt04[code].Timestamp, p, 2);
					strncpy(mMktdt04[code].Timestamp+2, p+3, 2);
					strncpy(mMktdt04[code].Timestamp+4, p+6, 2);
					break;
				default:
					break;
				}
			}
/*
			INFO("mktdt04:[%s][%d][%ld]",mMktdt04[code].SymbolEn,mMktdt04[code].TradeVolume,mMktdt04[code].TotalValueTraded);
			INFO("[%ld][%ld][%ld]",mMktdt04[code].PreClosePx,mMktdt04[code].NominalPrice,mMktdt04[code].HighPrice);
			INFO("[%ld][%ld][%ld]",mMktdt04[code].LowPrice,mMktdt04[code].TradePrice,mMktdt04[code].BuyPrice1);
			INFO("[%d][%ld][%d]",mMktdt04[code].BuyVolume1,mMktdt04[code].SellPrice1,mMktdt04[code].SellVolume1);
			INFO("[%s][%s]",mMktdt04[code].SecTradingStatus,mMktdt04[code].Timestamp);
*/
		}
	}
MKTDT04END:
	delete[] buffer;
	/*
	if (mMktdt04.size() != totalNum)
	{
		ERROR("check recorder number failed");
		return false;
	}
	*/
	return true;
}
bool ShhkReference::reff04StaticParse(char* path)
{
	//get size
	struct stat buf;
	int statValue = stat(path, &buf);
	if (statValue == -1)
	{
		ERROR("Get initial file size error!");
		return false;
	}
	char* buffer = new char[buf.st_size];
	if (buffer == NULL)
	{
		ERROR("create buffer(%d bytes) failed", buf.st_size);
		return false;
	}
	//read file
	FILE* fd = fopen(path, "r");
	while (buf.st_size != fread(buffer, 1, buf.st_size, fd))
	{
		ERROR("read %s failed", path);
		delete[] buffer;
		buffer = NULL;
		exit(1);
	}
	fclose(fd);

	std::string reference(buffer);
	delete[] buffer;
	buffer = NULL;
	HKPSNAP hkpsnap;
	memset(&hkpsnap, 0, sizeof(HKPSNAP));
	strcpy(hkpsnap.securityID,"00000");
	vHKP.push_back(hkpsnap);
	boost::char_separator<char> sep("\r\n");
	boost::tokenizer<boost::char_separator<char> > token(reference, sep);
	for (boost::tokenizer<boost::char_separator<char> >::const_iterator const_it = token.begin();
			const_it != token.end(); ++ const_it)
	{
		if (!split(*const_it))
			return false;
	}
	return true;
}

bool ShhkReference::split(const std::string& record)
{
	uint32_t i = 0;
	HKPSNAP hkpsnap;
	memset(&hkpsnap, 0, sizeof(HKPSNAP));
	boost::char_separator<char> sep("|");
	boost::tokenizer<boost::char_separator<char> > token(record, sep);
	for (boost::tokenizer<boost::char_separator<char> >::const_iterator const_it = token.begin();
			const_it != token.end(); ++ const_it, ++ i)
	{
		if (i < REFF04_FIELD)
		{
			switch(i)
			{
			case 0:
				break;
			case 1:
				strncpy(hkpsnap.securityID, (*const_it).c_str(), (*const_it).length());
				break;
			case 2:
				strncpy(hkpsnap.basicinfo.isin, trim(*const_it).c_str(), trim(*const_it).length());
				break;
			case 3:
				//cv.convert((char*)(trim((*const_it)).c_str()), trim((*const_it)).length(), hkpsnap.symbol, sizeof(hkpsnap.symbol));
				break;
			case 4:
				strncpy(hkpsnap.basicinfo.englishName, trim(*const_it).c_str(), trim(*const_it).length());
				break;
			case 5:
				strncpy(hkpsnap.securityDesc, trim(*const_it).c_str(), trim(*const_it).length());
				break;
			case 6:
				strncpy(hkpsnap.underlyingSecurityID, (*const_it).c_str(), (*const_it).length());
				break;
			case 7:
				strncpy(hkpsnap.marketID, trim(*const_it).c_str(), trim(*const_it).length());
				break;
			case 8:
				strncpy(hkpsnap.basicinfo.securityType, (*const_it).c_str(), (*const_it).length());
				break;
			case 9:
				strncpy(hkpsnap.basicinfo.currency, (*const_it).c_str(), (*const_it).length());
				break;
			case 10:
				hkpsnap.amountTimes = atoi((*const_it).c_str());
				break;
			case 11:
				ps.parse(*const_it);
				hkpsnap.perValue = ps.getMantissa() * pow(10.0L, 8 + ps.getExponent());
				break;
			case 12:
				strncpy(hkpsnap.perValueCurrency, (*const_it).c_str(), (*const_it).length());
				break;
			case 13:
				ps.parse(*const_it);
				hkpsnap.interest = ps.getMantissa() * pow(10.0L, 8 + ps.getExponent());
				break;
			case 14:
				hkpsnap.issueDate = atoi((*const_it).c_str());
				break;
			case 15:
				hkpsnap.roundLot = atoi((*const_it).c_str());
				break;
			case 16:
				ps.parse(*const_it);
				hkpsnap.prevClosePx = ps.getMantissa() * pow(10.0L, 3 + ps.getExponent());
				break;
			case 17:
				strncpy(hkpsnap.text, (*const_it).c_str(), (*const_it).length());
				break;
			case 18:
				strncpy(hkpsnap.securityStatusFlag, (*const_it).c_str(), (*const_it).length());
				break;
			default:
				ERROR("split record error!");
				return false;
			}
		}
	}
	if(mMktdt04.find(hkpsnap.securityID) != mMktdt04.end())
	{
		strncpy(hkpsnap.secTradingStatus, mMktdt04[hkpsnap.securityID].SecTradingStatus, sizeof(mMktdt04[hkpsnap.securityID].SecTradingStatus));
		hkpsnap.nominalpx = mMktdt04[hkpsnap.securityID].NominalPrice;
		hkpsnap.MDTime = atoi(mMktdt04[hkpsnap.securityID].Timestamp);
/*
		INFO("reff04:[%s]-[%s]-[%s]",hkpsnap.securityID,hkpsnap.basicinfo.isin,hkpsnap.basicinfo.englishName);
		INFO("[%s]-[%s]-[%s]",hkpsnap.securityDesc,hkpsnap.underlyingSecurityID,hkpsnap.marketID);
		INFO("[%s]-[%s]-[%d]",hkpsnap.basicinfo.securityType,hkpsnap.basicinfo.currency,hkpsnap.amountTimes);
		INFO("[%ld]-[%s]-[%ld]",hkpsnap.perValue,hkpsnap.perValueCurrency,hkpsnap.interest);
		INFO("[%d]-[%d]-[%ld]",hkpsnap.issueDate,hkpsnap.roundLot,hkpsnap.prevClosePx);
		INFO("[%s]-[%s]-[%s]",hkpsnap.text,hkpsnap.securityStatusFlag,hkpsnap.secTradingStatus);
		INFO("[%ld]-[%d]-[%s]",hkpsnap.nominalpx,hkpsnap.MDTime,mMktdt04[hkpsnap.securityID].Timestamp);
*/
		vHKP.push_back(hkpsnap);
	}

	return true;
}

std::string ShhkReference::trim(const std::string str)
{
	return boost::trim_right_copy(str);
}

int32_t ShhkReference::getStockNum()
{
	return totalNum;
}
