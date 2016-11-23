/*
 * crossReference.cpp
 *
 *  Created on: 2016年10月21日
 *      Author: mds
 */

#include "cnReference.h"

cnReference::cnReference() {
	// TODO Auto-generated constructor stub

}

cnReference::~cnReference() {
	// TODO Auto-generated destructor stub
}

bool cnReference::initial(const char* referencePath, const char* shm)
{
	time_t tt = time(NULL);
	tm *time = localtime(&tt);
	date=(time->tm_year + 1900)*10000+ (time->tm_mon + 1)*100+ time->tm_mday;
	initime=time->tm_hour*10000+time->tm_min*100+time->tm_sec;

	char fileName_dqpz[256];
	memset(fileName_dqpz, 0, 256);
	char fileName_hypz[256];
	memset(fileName_hypz, 0, 256);
	char fileName_gnpz[256];
	memset(fileName_gnpz, 0, 256);

	sprintf(fileName_dqpz,"%s/DQPZ.txt",referencePath);
	sprintf(fileName_hypz,"%s/HYPZ.txt",referencePath);
	sprintf(fileName_gnpz,"%s/GNPZ.txt",referencePath);

	if(!plateFileParse(fileName_dqpz,'A'))
		return false;
	INFO("dqpz parse success");
	if(!plateFileParse(fileName_hypz,'I'))
		return false;
	INFO("hypz parse success");
	if(!plateFileParse(fileName_gnpz,'C'))
		return false;
	INFO("gnpz parse success");
	CShmEnv shmEnv;
	if (shmEnv.openShmEnv(shm)!=SUCCEED)
	{
		ERROR("openShmEnv(%s) error",shm);
		return false;
	}

	CrossSnap crosssnap;
	Market market("cross",&shmEnv,&crosssnap);
	if (!market.load(sizeof(SNAP),1,1))
	{
		ERROR("market load error");
		return false;
	}
	for (unsigned int i=1;i<vCrossSotck.size();++i)
		market.initSnap(i,(char*)&vCrossSotck[i]);

	vCrossSotck[0].date=date;
	vCrossSotck[0].time=initime;
	vCrossSotck[0].amount=vCrossSotck.size();
	market.initSnap(0,(char*)&vCrossSotck[0]);
	market.refresh();
	return true;
}
bool cnReference::plateFileParse(char* path, const char flag)
{
	CROSNAP stock;
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
		memset(&stock, 0, sizeof(CROSNAP));
		if(row == 0)
		{
			for (boost::tokenizer<boost::char_separator<char> >::const_iterator cit = tkn.begin();
						cit != tkn.end(); ++ cit, ++ i)
				{
					const char* p = (*cit).c_str();
					switch(i)
					{
					case 0:
						if(strncmp(p, "HEADER", 6) != 0)
						{
							ERROR("%s header format error!",path);
							delete[] buffer;
							return false;
						}
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
							ERROR("%s date is not the current date",p);
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
			stock.sectiontype = flag;
			//INFO("dqpz:[%s][%s]",stock.securityID,stock.symbol);
			insertnum++;
			vCrossSotck.push_back(stock);
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
						ERROR("file trailer format error!");
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
		ERROR("check record num failed!");
		return false;
	}
	return true;
}
