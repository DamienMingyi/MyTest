/*
 * SettlePrice.cpp
 *
 *  Created on: Jun 23, 2015
 *      Author: level2
 */

#include "SettlePrice.h"

SettlePrice::SettlePrice():date(0), closeTime(0) {
	// TODO Auto-generated constructor stub
}

SettlePrice::~SettlePrice() {
	// TODO Auto-generated destructor stub
}

bool SettlePrice::close(char* close_ref_path, char* shm_path)
{
	time_t tt = time(NULL);
	tm *time = localtime(&tt);

	date=(time->tm_year + 1900)*10000+ (time->tm_mon + 1)*100+ time->tm_mday;
	closeTime=time->tm_hour*10000+time->tm_min*100+time->tm_sec;

	char filename[256];
	sprintf(filename,"%s/clpr03%02d%02d.txt", close_ref_path, date%10000/100,date%10000%100);
	getSettlePrice(filename);
	CShmEnv shmEnv;
	if (shmEnv.openShmEnv(shm_path)!=SUCCEED)
	{
		ERROR("openShmEnv(%s) error",shm_path);
		return false;
	}
	CShmVector* snapVector = shmEnv.getShmVector("sho/snap", sizeof(SHOSNAP));

	Market* shoMarket = createMarket("sho", shm_path, new OptionSnap());
	int snapCount = shoMarket->getSnapCount();
	for (int i = 0; i < snapCount; ++ i)
	{
		std::string code(((SHOSNAP*)snapVector->at(i))->securityID);
		std::map<std::string, int>::const_iterator cit = settleMap.find(code);
		if (cit != settleMap.end())
		{
			((SHOSNAP*)snapVector->at(i))->settlePx = cit->second;
		}
		else
			WARN("missing %s settle price", code.c_str());
	}
	return true;
}

void SettlePrice::getSettlePrice(char* path)
{
	ifstream infile;
	infile.open(path, ios::binary|ios::in);
	if(infile == false)
	{
		ERROR("open reference error:%s",path);
		exit(1);
	}

	settleMap["00000000"] = 0;
	while(infile.peek() != EOF)
	{
		string streference;
		if(!getline(infile, streference))
		{
			ERROR("read reference error!");
			exit(1);
		}
		boost::char_separator<char> sep("|");
		boost::tokenizer<boost::char_separator<char> > token(streference, sep);
		int32_t i = 0;
		std::string securityID;
		for (boost::tokenizer<boost::char_separator<char> >::iterator const_it = token.begin();
				const_it != token.end(); ++ const_it, ++ i)
		{
			switch(i)
			{
			case 0:
				break;
			case 1:
				securityID = *const_it;
				break;
			case 2:
				break;
			case 3:
				ps.parse(*const_it);
				settleMap[securityID] = ps.getMantissa() * pow(10.0L, 4 + ps.getExponent());;
				break;
			}
		}
	}
}
