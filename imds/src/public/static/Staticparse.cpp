/*
 * Staticparse.cpp
 *
 *  Created on: 2016年11月10日
 *      Author: mds
 */

#include "Staticparse.h"

Staticparse::Staticparse() {
	// TODO Auto-generated constructor stub
	time_t tt = time(NULL);
	tm *time = localtime(&tt);
	date=(time->tm_year + 1900)*10000+ (time->tm_mon + 1)*100+ time->tm_mday;
}

Staticparse::~Staticparse() {
	// TODO Auto-generated destructor stub
}

bool Staticparse::cpxxStaticParse(const char* refpath,map<string, CPXX> &mCpxx)
{
	int32_t length = getFileBufferLength(refpath);
	if (length == -1)
	{
		ERROR("get cpxx length failed!");
		return false;
	}
	FILE* fd = fopen(refpath, "rb+");
	char* buffer = new char[length];
	fread(buffer, 1, length, fd);
	fclose(fd);

	char code[8] = {0};
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
				strncpy(code, p, sizeof(code)-1);
				break;
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				strncpy(mCpxx[code].basicSecurityCode, p, sizeof(mCpxx[code].basicSecurityCode)-1);
				break;
			case 6:
				strncpy(mCpxx[code].marketType, p, sizeof(mCpxx[code].marketType)-1);
				break;
			case 7:
				strncpy(mCpxx[code].type, p, sizeof(mCpxx[code].type)-1);
				break;
			case 8:
				strncpy(mCpxx[code].subtype, p, sizeof(mCpxx[code].subtype)-1);
				break;
			case 9:
				strncpy(mCpxx[code].currency, p, sizeof(mCpxx[code].currency)-1);
				break;
			case 10:
				if (*cit == "               ")
					mCpxx[code].parValue = 0;
				else
				{
					ps.parse(*cit);
					mCpxx[code].parValue = ps.getMantissa() * pow(10.0L, 3+ps.getExponent());
				}
				break;
			case 11:
				break;
			case 12:
				mCpxx[code].lastTradingDate = atoi(p);
				break;
			case 13:
				mCpxx[code].listingDate	= atoi(p);
				break;
			case 14:
				break;
			case 15:
				mCpxx[code].buyNum = atol(p);
				break;
			case 16:
				mCpxx[code].sellNum = atol(p);
				break;
			case 17:
				mCpxx[code].applyUpimit = atol(p);
				break;
			case 18:
				mCpxx[code].applyDownlimit = atol(p);
				break;
			case 19:
				break;
			case 20:
				ps.parse(*cit);
				mCpxx[code].priceLevel = ps.getMantissa() * pow(10.0L, 3+ps.getExponent());
				break;
			case 21:
				strncpy(mCpxx[code].upDownLimitType, p, sizeof(mCpxx[code].upDownLimitType)-1);
				break;
			case 22:
				ps.parse(*cit);
				mCpxx[code].upLimitPx = ps.getMantissa() * pow(10.0L, 3+ps.getExponent());
				break;
			case 23:
				ps.parse(*cit);
				mCpxx[code].downLimitPx = ps.getMantissa() * pow(10.0L, 3+ps.getExponent());
				break;
			case 24:
				if (*cit == "           ")
					mCpxx[code].XRRate = 0;
				else
				{
					ps.parse(*cit);
					mCpxx[code].XRRate = ps.getMantissa() * pow(10.0L, 6+ps.getExponent());
				}
				break;
			case 25:
				if (*cit == "           ")
					mCpxx[code].XDAmount = 0;
				else
				{
					ps.parse(*cit);
					mCpxx[code].XDAmount = ps.getMantissa() * pow(10.0L, 6+ps.getExponent());
				}
				break;
			case 26:
				strncpy(mCpxx[code].financialTag, p, sizeof(mCpxx[code].financialTag)-1);
				break;
			case 27:
				strncpy(mCpxx[code].securityLendingTag, p, sizeof(mCpxx[code].securityLendingTag)-1);
				break;
			case 28:
				strncpy(mCpxx[code].productStatus, p, sizeof(mCpxx[code].productStatus)-1);
				break;
			case 29:
				break;
			default:
				break;
				}
			}
		/*
		INFO("cpxx:[%s][%s][%s][%ld][%d][%d][%s][%ld][%ld][%d][%d][%s][%s][%s]",\
				code,\
				mCpxx[code].marketType,\
				mCpxx[code].type,\
				mCpxx[code].parValue,\
				mCpxx[code].lastTradingDate,\
				mCpxx[code].listingDate,\
				mCpxx[code].upDownLimitType,\
				mCpxx[code].upLimitPx,\
				mCpxx[code].downLimitPx,\
				mCpxx[code].XRRate,\
				mCpxx[code].XDAmount,\
				mCpxx[code].financialTag,\
				mCpxx[code].securityLendingTag,\
				mCpxx[code].productStatus);
		*/
	}
	delete[] buffer;
	return true;
}

bool Staticparse::gzlxStaticParse(const char* refpath,map<string, GZLX>& mGzlx)
{
	ifstream infile;
	SGZLX sGzlx;
	char code[8] = {0};
	char tmpvalue[32] = {0};
	string value;

	infile.open(refpath, ios::in);
	if (infile == false)
	{
		ERROR("open gzlx error:%s", refpath);
		return false;
	}

	infile.seekg(193);
	while(infile.peek() != EOF)
	{
		memset(&sGzlx, 0, sizeof(SGZLX));

		infile.read((char *)&sGzlx, sizeof(SGZLX));
		if(infile.bad()) {
			ERROR("read gzlx error:%s",refpath);
			return false;
		}

		strncpy(code, sGzlx.gzdm, sizeof(sGzlx.gzdm));
		mGzlx[code].interestDate = atoi(sGzlx.interestDate);
		strncpy(tmpvalue, sGzlx.interestPerHundred, sizeof(sGzlx.interestPerHundred));
		ps.parse(tmpvalue);
		mGzlx[code].interestPerHundred = ps.getMantissa() * pow(10.0L, 8+ps.getExponent());
		mGzlx[code].daysOfInterest = atoi(sGzlx.daysOfInterest);
		memset(tmpvalue, 0, sizeof(tmpvalue));
		strncpy(tmpvalue, sGzlx.couponRate, sizeof(sGzlx.couponRate));
		ps.parse(tmpvalue);
		mGzlx[code].couponRate = ps.getMantissa() * pow(10.0L, 4+ps.getExponent());
		/*
		INFO("gzlx:[%s][%d][%ld][%d][%ld]",\
				code,\
				mGzlx[code].interestDate,\
				mGzlx[code].interestPerHundred,\
				mGzlx[code].daysOfInterest,\
				mGzlx[code].couponRate);
		*/
		memset(&sGzlx, 0x00, sizeof(SGZLX));
	}
	infile.close();
	return true;
}

bool Staticparse::dbpStaticParse(const char* refpath, map<string, DBP>& mDbp)
{
	int32_t length = getFileBufferLength(refpath);
	if (length == -1)
	{
		ERROR("get dbp length failed!");
		return false;
	}
	FILE* fd = fopen(refpath, "rb+");
	char* buffer = new char[length];
	fread(buffer, 1, length, fd);
	fclose(fd);

	char code[8] = {0},type[4] = {0};
	string value;
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
					strncpy(code, p, sizeof(code)-1);
					break;
			case 1:
				strncpy(type, p, sizeof(type)-1);
				break;
			case 2:
				if(0 == memcmp(type, "001", sizeof(type)-1))
				{
					mDbp[code].finance = atol(p);
				}
				else if(0 == memcmp(type, "002", sizeof(type)-1))
				{
					mDbp[code].securityLending = atol(p);
				}
				else
				{
					mDbp[code].finance = 0;
					mDbp[code].securityLending = 0;
				}
				break;
			default:
				break;
			}
		}
		/*
		INFO("dbp:[%s][%s][%ld][%ld][%ld]",\
						code,type,mDbp[code].finance,mDbp[code].securityLending,mDbp[code].securityLending);
						*/
	}
	delete[] buffer;
	return true;
}

bool Staticparse::zqjcxxStaticParse(const char* refpath,map<string, BASIC_INFO>& mBasicInfo)
{
	int32_t length = getFileBufferLength(refpath);
	if (length == -1)
	{
		ERROR("get zqjcxx length failed!");
		return false;
	}
	FILE* fd = fopen(refpath, "rb+");
	char* buffer = new char[length];
	fread(buffer, 1, length, fd);
	fclose(fd);

	int row = 0,totalnum=0,cnt=0;
	char code[8] = {0};
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
				switch(i)
				{
				case 0:
					if(strncmp(p, "HEADER", 6) != 0)
					{
						ERROR("zqjcxx's header format error!");
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
						ERROR("zqjcxx's data is not the current data");
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
					strncpy(code, p, sizeof(code)-1);
					break;
				case 1:
					strncpy(mBasicInfo[code].isin,p,sizeof(mBasicInfo[code].isin)-1);
					break;
				case 2:
					strncpy(mBasicInfo[code].phoneticize,p,sizeof(mBasicInfo[code].phoneticize)-1);
					break;
				case 3:
					strncpy(mBasicInfo[code].regionCode, p, sizeof(mBasicInfo[code].regionCode)-1);
					break;
				case 4:
					strncpy(mBasicInfo[code].industyCode, p, sizeof(mBasicInfo[code].industyCode)-1);
					break;
				default:
					break;
				}
			}
			/*
			INFO("zqjcxx:[%s][%s][%s][%s][%s]",
								code,\
								mBasicInfo[code].isin,\
								mBasicInfo[code].phoneticize,\
								mBasicInfo[code].regionCode,\
								mBasicInfo[code].industyCode);
			*/
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
						ERROR("zqjcxx's trailer format error!");
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

bool Staticparse::gbsjStaticParse(char* refpath,map<string, CAPITAL_DATA>& mCapitalData)
{
	int32_t length = getFileBufferLength(refpath);
	if (length == -1)
	{
		ERROR("get gbsj length failed!");
		return false;
	}
	FILE* fd = fopen(refpath, "rb+");
	char* buffer = new char[length];
	fread(buffer, 1, length, fd);
	fclose(fd);

	int row = 0,totalnum=0,cnt=0;
	char code[8] = {0};
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
					switch(i)
					{
					case 0:
						if(strncmp(p, "HEADER", 6) != 0)
						{
							ERROR("gbsj's header format error!");
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
							ERROR("gbsj's date is not the current date",p);
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
		if(row >0 && row <= totalnum)
		{
			cnt++;
			for (boost::tokenizer<boost::char_separator<char> >::const_iterator cit = tkn.begin();
					cit != tkn.end(); ++ cit, ++ i)
			{
				const char* p = (*cit).c_str();
				switch(i)
				{
				case 0:
					strncpy(code, p, sizeof(code)-1);
					break;
				case 1:
					break;
				case 2:
					break;
				case 3:
					break;
				case 4:
					mCapitalData[code].totalCapital = atol(p);
					break;
				case 5:
					mCapitalData[code].nonTradableShare = atol(p);
					break;
				case 6:
					mCapitalData[code].tradableShare = atol(p);
					break;
				case 7:
					mCapitalData[code].aTradableShare = atol(p);
					break;
				case 8:
					mCapitalData[code].bTradableShare = atol(p);
					break;
				case 9:
					mCapitalData[code].hTradableShare = atol(p);
					break;
				default:
					break;
				}
			}
			/*
			INFO("gbsj:[%s][%ld][%ld][%ld][%ld][%ld][%ld]",\
					code,\
					mCapitalData[code].totalCapital,\
					mCapitalData[code].nonTradableShare,\
					mCapitalData[code].tradableShare,\
					mCapitalData[code].aTradableShare,\
					mCapitalData[code].bTradableShare,\
					mCapitalData[code].hTradableShare);
			*/
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
						ERROR("gbsj's trailer format error!");
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

bool Staticparse::cwzyStaticParse(char* refpath,map<string, FINANCIAL_DIGEST>& mFinancialDigest) {
	int32_t length = getFileBufferLength(refpath);
	if (length == -1)
	{
		ERROR("get cwzy length failed!");
		return false;
	}
	FILE* fd = fopen(refpath, "rb+");
	char* buffer = new char[length];
	fread(buffer, 1, length, fd);
	fclose(fd);

	int row = 0,totalnum = 0, cnt=0;
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
							ERROR("cwzy's header format error!");
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
							ERROR("cwzy's date(%s) is not the current date",p);
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
					strncpy(code, p, sizeof(code)-1);
					break;
				case 1:
					break;
				case 2:
					break;
				case 3:
					if(*cit == " ")
						mFinancialDigest[code].totalAsset = 0;
					else
					{
						ps.parse(*cit);
						mFinancialDigest[code].totalAsset= ps.getMantissa() * pow(10.0L, 4+ps.getExponent());
					}
					break;
				case 4:
					if(*cit == " ")
						mFinancialDigest[code].liquidAsset = 0;
					else
					{
						ps.parse(*cit);
						mFinancialDigest[code].liquidAsset= ps.getMantissa() * pow(10.0L, 4+ps.getExponent());
					}
					break;
				case 5:
					if(*cit == " ")
						mFinancialDigest[code].fixedAsset = 0;
					else
					{
						ps.parse(*cit);
						mFinancialDigest[code].fixedAsset= ps.getMantissa() * pow(10.0L, 4+ps.getExponent());
					}
					break;
				case 6:
					if(*cit == " ")
						mFinancialDigest[code].intangibleAsset = 0;
					else
					{
						ps.parse(*cit);
						mFinancialDigest[code].intangibleAsset= ps.getMantissa() * pow(10.0L, 4+ps.getExponent());
					}
					break;
				case 7:
					if(*cit == " ")
						mFinancialDigest[code].longTermDebt = 0;
					else
					{
						ps.parse(*cit);
						mFinancialDigest[code].longTermDebt= ps.getMantissa() * pow(10.0L, 4+ps.getExponent());
					}
					break;
				case 8:
					if(*cit == " ")
						mFinancialDigest[code].currentLiabilities = 0;
					else
					{
						ps.parse(*cit);
						mFinancialDigest[code].currentLiabilities= ps.getMantissa() * pow(10.0L, 4+ps.getExponent());
					}
					break;
				case 9:
					if(*cit == " ")
						mFinancialDigest[code].shareHolderEquity = 0;
					else
					{
						ps.parse(*cit);
						mFinancialDigest[code].shareHolderEquity= ps.getMantissa() * pow(10.0L, 4+ps.getExponent());
					}
					break;
				case 10:
					if(*cit == " ")
						mFinancialDigest[code].capitalReserve = 0;
					else
					{
						ps.parse(*cit);
						mFinancialDigest[code].capitalReserve= ps.getMantissa() * pow(10.0L, 4+ps.getExponent());
					}
					break;
				case 11:
					if(*cit == " ")
						mFinancialDigest[code].mainBusinessIncome = 0;
					else
					{
						ps.parse(*cit);
						mFinancialDigest[code].mainBusinessIncome= ps.getMantissa() * pow(10.0L, 4+ps.getExponent());
					}
					break;
				case 12:
					if(*cit == " ")
						mFinancialDigest[code].mainBusinessProfit = 0;
					else
					{
						ps.parse(*cit);
						mFinancialDigest[code].mainBusinessProfit= ps.getMantissa() * pow(10.0L, 4+ps.getExponent());
					}
					break;
				case 13:
					if(*cit == " ")
						mFinancialDigest[code].totalProfit = 0;
					else
					{
						ps.parse(*cit);
						mFinancialDigest[code].totalProfit= ps.getMantissa() * pow(10.0L, 4+ps.getExponent());
					}
					break;
				case 14:
					if(*cit == " ")
						mFinancialDigest[code].netProfit = 0;
					else
					{
						ps.parse(*cit);
						mFinancialDigest[code].netProfit= ps.getMantissa() * pow(10.0L, 4+ps.getExponent());
					}
					break;
				case 15:
					if(*cit == " ")
						mFinancialDigest[code].undistributedProfit = 0;
					else
					{
						ps.parse(*cit);
						mFinancialDigest[code].undistributedProfit= ps.getMantissa() * pow(10.0L, 4+ps.getExponent());
					}
					break;
				case 16:
					if(*cit == " ")
						mFinancialDigest[code].operatingNetCashFlow = 0;
					else
					{
						ps.parse(*cit);
						mFinancialDigest[code].operatingNetCashFlow= ps.getMantissa() * pow(10.0L, 4+ps.getExponent());
					}
					break;
				case 17:
					if(*cit == " ")
						mFinancialDigest[code].investingNetCashFlow = 0;
					else
					{
						ps.parse(*cit);
						mFinancialDigest[code].investingNetCashFlow= ps.getMantissa() * pow(10.0L, 4+ps.getExponent());
					}
					break;
				case 18:
					if(*cit == " ")
						mFinancialDigest[code].financingNetCashFlow = 0;
					else
					{
						ps.parse(*cit);
						mFinancialDigest[code].financingNetCashFlow= ps.getMantissa() * pow(10.0L, 4+ps.getExponent());
					}
					break;
				case 19:
					if(*cit == " ")
						mFinancialDigest[code].cashFlowIncrease = 0;
					else
					{
						ps.parse(*cit);
						mFinancialDigest[code].cashFlowIncrease= ps.getMantissa() * pow(10.0L, 4+ps.getExponent());
					}
					break;
				default:
					break;
				}
			}
			/*
			INFO("cwzy:[%s][%ld][%ld][%ld][%ld][%ld][%ld][%ld][%ld][%ld][%ld][%ld][%ld][%ld][%ld][%ld][%ld][%ld]",
					code,\
					mFinancialDigest[code].totalAsset,\
					mFinancialDigest[code].liquidAsset,\
					mFinancialDigest[code].fixedAsset,\
					mFinancialDigest[code].intangibleAsset,\
					mFinancialDigest[code].longTermDebt,\
					mFinancialDigest[code].currentLiabilities,\
					mFinancialDigest[code].shareHolderEquity,\
					mFinancialDigest[code].capitalReserve,\
					mFinancialDigest[code].mainBusinessIncome,\
					mFinancialDigest[code].mainBusinessProfit,\
					mFinancialDigest[code].totalProfit,\
					mFinancialDigest[code].netProfit,\
					mFinancialDigest[code].undistributedProfit,\
					mFinancialDigest[code].operatingNetCashFlow,\
					mFinancialDigest[code].investingNetCashFlow,\
					mFinancialDigest[code].financingNetCashFlow,\
					mFinancialDigest[code].cashFlowIncrease);
			*/
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
						ERROR("cwzy's trailer format error!");
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

bool Staticparse::cwzbStaticParse(char* refpath,map<string, FINANCIAL_INDEX>& mFinancialIndex) {
	int32_t length = getFileBufferLength(refpath);
	if (length == -1)
	{
		ERROR("get cwzb length failed!");
		return false;
	}
	FILE* fd = fopen(refpath, "rb+");
	char* buffer = new char[length];
	fread(buffer, 1, length, fd);
	fclose(fd);

	int row = 0, totalnum=0, cnt=0;
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
							ERROR("cwzb's header format error!");
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
							ERROR("cwzb's date(%s) is not the current date",p);
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
					strncpy(code, p, sizeof(code)-1);
					break;
				case 1:
					break;
				case 2:
					if(*cit == " ")
						mFinancialIndex[code].earningPerShare = 0;
					else
					{
						ps.parse(*cit);
						mFinancialIndex[code].earningPerShare= ps.getMantissa() * pow(10.0L, 4+ps.getExponent());
					}
					break;
				case 3:
					if(*cit == " ")
						mFinancialIndex[code].netAssetPerShare = 0;
					else
					{
						ps.parse(*cit);
						mFinancialIndex[code].netAssetPerShare= ps.getMantissa() * pow(10.0L, 4+ps.getExponent());
					}
					break;
				case 4:
					if(*cit == " ")
						mFinancialIndex[code].operatingCashFlowPerShare = 0;
					else
					{
						ps.parse(*cit);
						mFinancialIndex[code].operatingCashFlowPerShare= ps.getMantissa() * pow(10.0L, 4+ps.getExponent());
					}
					break;
				case 5:
					if(*cit == " ")
						mFinancialIndex[code].undistributedProfitPerShare = 0;
					else
					{
						ps.parse(*cit);
						mFinancialIndex[code].undistributedProfitPerShare= ps.getMantissa() * pow(10.0L, 4+ps.getExponent());
					}
					break;
				case 6:
					if(*cit == " ")
						mFinancialIndex[code].operatingNetProfitRate = 0;
					else
					{
						ps.parse(*cit);
						mFinancialIndex[code].operatingNetProfitRate= ps.getMantissa() * pow(10.0L, 6+ps.getExponent());
					}
					break;
				case 7:
					if(*cit == " ")
						mFinancialIndex[code].operatingGrossProfitRate = 0;
					else
					{
						ps.parse(*cit);
						mFinancialIndex[code].operatingGrossProfitRate= ps.getMantissa() * pow(10.0L, 6+ps.getExponent());
					}
					break;
				case 8:
					if(*cit == " ")
						mFinancialIndex[code].mainBusinessProfitRate = 0;
					else
					{
						ps.parse(*cit);
						mFinancialIndex[code].mainBusinessProfitRate= ps.getMantissa() * pow(10.0L, 6+ps.getExponent());
					}
					break;
				case 9:
					if(*cit == " ")
						mFinancialIndex[code].netProfitRate = 0;
					else
					{
						ps.parse(*cit);
						mFinancialIndex[code].netProfitRate= ps.getMantissa() * pow(10.0L, 6+ps.getExponent());
					}
					break;
				case 10:
					if(*cit == " ")
						mFinancialIndex[code].netReturnOnAssets = 0;
					else
					{
						ps.parse(*cit);
						mFinancialIndex[code].netReturnOnAssets= ps.getMantissa() * pow(10.0L, 6+ps.getExponent());
					}
					break;
				case 11:
					if(*cit == " ")
						mFinancialIndex[code].currentRatio = 0;
					else
					{
						ps.parse(*cit);
						mFinancialIndex[code].currentRatio= ps.getMantissa() * pow(10.0L, 6+ps.getExponent());
					}
					break;
				case 12:
					if(*cit == " ")
						mFinancialIndex[code].quickRatio = 0;
					else
					{
						ps.parse(*cit);
						mFinancialIndex[code].quickRatio= ps.getMantissa() * pow(10.0L, 6+ps.getExponent());
					}
					break;
				case 13:
					if(*cit == " ")
						mFinancialIndex[code].debt2EquityRatio = 0;
					else
					{
						ps.parse(*cit);
						mFinancialIndex[code].debt2EquityRatio= ps.getMantissa() * pow(10.0L, 6+ps.getExponent());
					}
					break;
				case 14:
					if(*cit == " ")
						mFinancialIndex[code].equityRatio = 0;
					else
					{
						ps.parse(*cit);
						mFinancialIndex[code].equityRatio= ps.getMantissa() * pow(10.0L, 6+ps.getExponent());
					}
					break;
				case 15:
					if(*cit == " ")
						mFinancialIndex[code].turnoverOfAccountReceivable = 0;
					else
					{
						ps.parse(*cit);
						mFinancialIndex[code].turnoverOfAccountReceivable= ps.getMantissa() * pow(10.0L, 6+ps.getExponent());
					}
					break;
				case 16:
					if(*cit == " ")
						mFinancialIndex[code].stockTurnover = 0;
					else
					{
						ps.parse(*cit);
						mFinancialIndex[code].stockTurnover= ps.getMantissa() * pow(10.0L, 6+ps.getExponent());
					}
					break;
				case 17:
					if(*cit == " ")
						mFinancialIndex[code].mainRevenueGrowthRate = 0;
					else
					{
						ps.parse(*cit);
						mFinancialIndex[code].mainRevenueGrowthRate= ps.getMantissa() * pow(10.0L, 6+ps.getExponent());
					}
					break;
				case 18:
					if(*cit == " ")
						mFinancialIndex[code].netProfitGrowthRate = 0;
					else
					{
						ps.parse(*cit);
						mFinancialIndex[code].netProfitGrowthRate= ps.getMantissa() * pow(10.0L, 6+ps.getExponent());
					}
					break;
				case 19:
					if(*cit == " ")
						mFinancialIndex[code].EPSG = 0;
					else
					{
						ps.parse(*cit);
						mFinancialIndex[code].EPSG= ps.getMantissa() * pow(10.0L, 6+ps.getExponent());
					}
					break;
				case 20:
					if(*cit == " ")
						mFinancialIndex[code].growthRateOfShareholder = 0;
					else
					{
						ps.parse(*cit);
						mFinancialIndex[code].growthRateOfShareholder= ps.getMantissa() * pow(10.0L, 6+ps.getExponent());
					}
					break;
				case 21:
					if(*cit == " ")
						mFinancialIndex[code].MPIP = 0;
					else
					{
						ps.parse(*cit);
						mFinancialIndex[code].MPIP= ps.getMantissa() * pow(10.0L, 6+ps.getExponent());
					}
					break;
				case 22:
					if(*cit == " ")
						mFinancialIndex[code].totalAssetsGrowthRate = 0;
					else
					{
						ps.parse(*cit);
						mFinancialIndex[code].totalAssetsGrowthRate= ps.getMantissa() * pow(10.0L, 6+ps.getExponent());
					}
					break;
				default:
					break;
				}
			}
			/*
			INFO("cwzb:[%s][%ld][%ld][%ld][%ld][%ld][%ld][%ld][%ld][%ld][%ld][%ld][%ld][%ld][%ld][%ld][%ld][%ld][%ld]",
								code,\
								mFinancialIndex[code].earningPerShare,\
								mFinancialIndex[code].netAssetPerShare,\
								mFinancialIndex[code].operatingCashFlowPerShare,\
								mFinancialIndex[code].mainBusinessProfitRate,\
								mFinancialIndex[code].netProfitRate,\
								mFinancialIndex[code].netReturnOnAssets,\
								mFinancialIndex[code].currentRatio,\
								mFinancialIndex[code].quickRatio,\
								mFinancialIndex[code].debt2EquityRatio,\
								mFinancialIndex[code].equityRatio,\
								mFinancialIndex[code].turnoverOfAccountReceivable,\
								mFinancialIndex[code].stockTurnover,\
								mFinancialIndex[code].mainRevenueGrowthRate,\
								mFinancialIndex[code].netProfitGrowthRate,\
								mFinancialIndex[code].EPSG,\
								mFinancialIndex[code].growthRateOfShareholder,\
								mFinancialIndex[code].MPIP,\
								mFinancialIndex[code].totalAssetsGrowthRate
								);
			*/
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
						ERROR("cwzb's trailer format error!");
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

bool Staticparse::qxsjStaticParse(char* refpath,map<string, WEIGHT_DATA>& mWeightData)
{
	return true;
}
int32_t Staticparse::getFileBufferLength(const char* fileName)
{
	struct stat buf;
	int statValue = stat(fileName, &buf);
	if (statValue != -1)
		return buf.st_size;
	else
		return -1;
}
