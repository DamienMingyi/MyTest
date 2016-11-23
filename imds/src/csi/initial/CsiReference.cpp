/*
 * CSIPReference.cpp
 *
 *  Created on: Sep 1, 2015
 *      Author: level2
 */

#include "CsiReference.h"

#include "../../util/shm/CShmEnv.h"
#include "../../public/program/Logger.h"
#include <fstream>

CsiReference::CsiReference() {
	// TODO Auto-generated constructor stub
	recordNum=0;
}

CsiReference::~CsiReference() {
	// TODO Auto-generated destructor stub
}

bool CsiReference::initial(const char* referencePath, const char* shm)
{
	time_t tt = time(NULL);
	tm *time = localtime(&tt);
	//current date and time
	date=(time->tm_year + 1900)*10000+ (time->tm_mon + 1)*100+ time->tm_mday;
	initime=time->tm_hour*10000+time->tm_min*100+time->tm_sec;

	char fileName[256] = {0};

	sprintf(fileName,"%s/csi%04d%02d%02d.txt",referencePath,date % 100000000 / 10000, date % 10000 / 100, date % 100);

	if(!CsiStaticParse(fileName))
		return false;
	INFO("csi parse success");
	if(vIndex.empty())
		return false;

	CShmEnv shmEnv;
	if (shmEnv.openShmEnv(shm)!=SUCCEED)
	{
		ERROR("openShmEnv(%s) error",shm);
		return false;
	}
	IndexPressSnap indexsnap;
	Market market("csi",&shmEnv,&indexsnap);
	if (!market.load(sizeof(INDEXSNAP),sizeof(INDEXLINE),sizeof(INDEXTRADE)))
	{
		ERROR("market load error");
		return false;
	}
	for (unsigned int i=1;i< vIndex.size();++i)
		market.initSnap(i,(char*)&vIndex[i]);
	ST_SHM_ENV_INFO envInfo;
	SHM_OBJ_INFO_LIST objInfoList;
	if (shmEnv.getEnvInfo(envInfo) && shmEnv.getShmObjInfo(objInfoList))
	{
		vIndex[0].hand=envInfo.envSize*100/envInfo.envCapacity;
		vIndex[0].prevAmount=objInfoList.size();
	}
	vIndex[0].date=date;
	vIndex[0].time=initime;
	vIndex[0].amount=vIndex.size();
	market.initSnap(0,(char*)&vIndex[0]);
	market.refresh();
	return true;
}

bool CsiReference::CsiStaticParse(char* path)
{
	INDEXSNAP index;
	int32_t length = getFileBufferLength(path);
	if (length == -1)
	{
		ERROR("get csip length failed!");
		return false;
	}
	FILE* fd = fopen(path, "rb+");
	char* buffer = new char[length];
	fread(buffer, 1, length, fd);
	fclose(fd);

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
			memset(&index, 0, sizeof(INDEXSNAP));
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
					index.date = atoi(p);
					memcpy(index.securityID, "000000", 6);
					break;
				case 3:
					index.time = atoi(p);
					break;
				case 4:
					recordNum = atoi(p);
					break;
				default:
					break;
				}
			}
			vIndex.push_back(index);
		}
		else
		{
			memset(&index, 0, sizeof(INDEXSNAP));
			for (boost::tokenizer<boost::char_separator<char> >::const_iterator cit = tkn.begin();
					cit != tkn.end(); ++ cit, ++ i)
			{
				const char* p = (*cit).c_str();
				switch(i)
				{
				case 0:
					if(atoi(p)!=1)
					{
						goto QUOT_INFO_END;
					}
					break;
				case 1:
					break;
				case 2:
					memcpy(index.securityID, p, sizeof(index.securityID)-1);
					break;
				case 3:
					//strncpy(index.symbol, p ,sizeof(index.symbol)-1);
					cv.convert(const_cast<char*>(p), (*cit).length(), index.symbol,sizeof(index.symbol));
					break;
				case 4:
					memcpy(index.market, p, sizeof(index.market)-1);
					break;
				case 5:
					break;
				case 6:
					break;
				case 7:
					break;
				case 8:
					break;
				case 9:
					break;
				case 10:
					if(p)
					{
						ps.parse(*cit);
						index.prevClosePx = ps.getMantissa() * pow(10.0L, 4 + ps.getExponent());
					}
					break;
				case 11:
					break;
				case 12:
					break;
				case 13:
					break;
				case 14:
					break;
				case 15:
					if(p)
					{
						ps.parse(*cit);
						index.exchangeRate = ps.getMantissa() * pow(10.0L, 8 + ps.getExponent());
					}
					break;
				case 16:
					strncpy(index.currency, p, sizeof(index.currency)-1);
					break;
				case 17:
					index.indexSeq = atoi(p);
					break;
				case 18:
					break;
				case 19:
					break;
				default:
					break;
				}
			}
			/*
			INFO("[%s][%s][%s][%ld][%ld][%s][%ld]",
					index.securityID,index.symbol,index.market,index.prevClosePx,index.exchangeRate, index.currency, index.indexSeq);
			*/
			vIndex.push_back(index);
		}
	}
QUOT_INFO_END:
	delete[] buffer;
	return true;
}
